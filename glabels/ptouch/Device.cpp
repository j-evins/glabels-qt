//  ptouch/Device.cpp
//
//  Copyright (C) 2026  gLabels-qt contributors
//  SPDX-License-Identifier: GPL-3.0-or-later

#include "Device.hpp"
#include "Protocol.hpp"

#include <QDebug>
#include <QThread>

#include <algorithm>
#include <array>
#include <cstring>


namespace glabels::ptouch
{

namespace
{
    // Status packet constants
    constexpr int     kStatusPacketSize = 32;
    constexpr uint8_t kStatusHead0      = 0x80;
    constexpr uint8_t kStatusHead1      = 0x20;

    // Byte offsets within the 32-byte status packet
    constexpr int kOffError      = 8;   // 2 bytes, little-endian
    constexpr int kOffMediaWidth = 10;
    constexpr int kOffMediaType  = 11;
    constexpr int kOffTapeColor  = 24;
    constexpr int kOffTextColor  = 25;

    /// Set bit @p pixel (0 = rightmost) in raster line @p line.
    /// @p lineBytes is the number of bytes in the raster line (maxPixelWidth/8).
    inline void rasterSetPixel(uint8_t* line, int lineBytes, int pixel) noexcept
    {
        if (pixel < 0 || pixel >= lineBytes * 8)  return;
        // The bit layout matches the original libptouch convention:
        // bit 0 of the last byte is the first physical pixel position.
        line[(lineBytes - 1) - (pixel / 8)] |= static_cast<uint8_t>(1u << (pixel % 8));
    }

} // namespace


// ─── Device ───────────────────────────────────────────────────────────────────

Device::Device(std::unique_ptr<UsbTransport> transport,
               const DeviceDescriptor*       descriptor,
               UsbDeviceId                   id)
    : mTransport(std::move(transport))
    , mDescriptor(descriptor)
    , mId(id)
{}

std::unique_ptr<Device> Device::open(UsbContext&        ctx,
                                      const UsbDeviceId& id,
                                      QString&           errorOut)
{
    const DeviceDescriptor* desc = findDevice(id.vid, id.pid);
    if (!desc || !desc->isValid())
    {
        errorOut = QStringLiteral("Unknown P-Touch device (VID %1, PID %2)")
                   .arg(id.vid, 4, 16, QLatin1Char('0'))
                   .arg(id.pid, 4, 16, QLatin1Char('0'));
        return nullptr;
    }

    if (desc->isPLite())
    {
        errorOut = QStringLiteral(
            "%1 is in P-Lite mode, which is not supported for direct USB printing.\n"
            "Turn off P-Lite mode (flip the switch from EL to E, or hold the PLite "
            "button for ~2 seconds) and try again.").arg(QLatin1StringView(desc->name));
        return nullptr;
    }

    if (desc->isUnsupported())
    {
        errorOut = QStringLiteral(
            "%1 uses a raster format that is not yet supported.")
                   .arg(QLatin1StringView(desc->name));
        return nullptr;
    }

    std::string transportError;
    auto transport = UsbTransport::open(ctx, id, transportError);
    if (!transport)
    {
        errorOut = QString::fromStdString(transportError);
        return nullptr;
    }

    // Use new directly since the constructor is private.
    auto device = std::unique_ptr<Device>(
        new Device(std::move(transport), desc, id));

    // Hardware initialise: ESC @
    QString initError;
    if (!device->sendBytes(Protocol::init(), initError))
    {
        errorOut = QStringLiteral("Failed to initialise printer: ") + initError;
        return nullptr;
    }

    return device;
}

QString Device::displayName() const
{
    return QString::fromStdString(mId.displayName());
}

bool Device::sendBytes(const QByteArray& data, QString& errorOut)
{
    std::string err;
    bool ok = mTransport->write(
        std::span<const uint8_t>(
            reinterpret_cast<const uint8_t*>(data.constData()),
            static_cast<size_t>(data.size())),
        err);

    if (!ok)  errorOut = QString::fromStdString(err);
    return ok;
}

bool Device::queryStatus(DeviceStatus& statusOut, QString& errorOut)
{
    // Send the status request command
    if (!sendBytes(Protocol::statusRequest(), errorOut))  return false;

    // The printer takes a moment to respond; poll up to 10 times with 100 ms gaps
    std::array<uint8_t, kStatusPacketSize> buf{};
    int bytesRead = 0;

    for (int attempt = 0; attempt < 10; ++attempt)
    {
        QThread::msleep(100);

        std::string readErr;
        if (!mTransport->read(std::span<uint8_t>(buf.data(), buf.size()),
                               bytesRead, readErr))
        {
            errorOut = QStringLiteral("Status read failed: %1")
                       .arg(QString::fromStdString(readErr));
            return false;
        }

        if (bytesRead == kStatusPacketSize)  break;
    }

    if (bytesRead != kStatusPacketSize)
    {
        errorOut = QStringLiteral("Incomplete status packet (%1 of %2 bytes)")
                   .arg(bytesRead).arg(kStatusPacketSize);
        return false;
    }

    if (buf[0] != kStatusHead0 || buf[1] != kStatusHead1)
    {
        errorOut = QStringLiteral("Unexpected status packet header (0x%1 0x%2)")
                   .arg(buf[0], 2, 16, QLatin1Char('0'))
                   .arg(buf[1], 2, 16, QLatin1Char('0'));
        return false;
    }

    statusOut.errorCode    = static_cast<uint16_t>(buf[kOffError] | (buf[kOffError + 1] << 8));
    statusOut.mediaWidthMm = buf[kOffMediaWidth];
    statusOut.mediaType    = buf[kOffMediaType];
    statusOut.tapeColor    = buf[kOffTapeColor];
    statusOut.textColor    = buf[kOffTextColor];

    const TapeDescriptor* tape = findTape(statusOut.mediaWidthMm);
    statusOut.tapePixelWidth = tape ? tape->printWidthPx : 0;

    if (statusOut.tapePixelWidth == 0)
    {
        errorOut = QStringLiteral("Unknown tape width: %1 mm").arg(statusOut.mediaWidthMm);
        return false;
    }

    qDebug() << "P-Touch status: tape" << statusOut.mediaWidthMm << "mm ="
             << statusOut.tapePixelWidth << "px printable width"
             << "| error code" << Qt::hex << statusOut.errorCode;

    return true;
}

bool Device::printImage(const QImage& image, const DeviceStatus& status, QString& errorOut)
{
    if (!status.isValid())
    {
        errorOut = QStringLiteral("Invalid device status; call queryStatus() first");
        return false;
    }

    const int imgW  = image.width();   // tape-feed direction (label length)
    const int imgH  = image.height();  // across tape (label width)

    if (imgH > mDescriptor->maxPixelWidth)
    {
        errorOut = QStringLiteral(
            "Image height (%1 px) exceeds the print-head width (%2 px) on a %3")
                   .arg(imgH)
                   .arg(mDescriptor->maxPixelWidth)
                   .arg(QLatin1StringView(mDescriptor->name));
        return false;
    }

    const int lineBytes = mDescriptor->maxPixelWidth / 8;  // bytes per raster line (e.g. 16)

    // Centre the image vertically within the print head's pixel range.
    // The print head is always maxPixelWidth wide; the tape is narrower.
    // offset is the bit index of the bottom-most pixel of the image within
    // the raster line (bit 0 = physical edge the head scans from first).
    const int offset = (mDescriptor->maxPixelWidth / 2) - (imgH / 2);

    // ── Enter raster mode ───────────────────────────────────────────────────

    if (mDescriptor->isPackBits())
    {
        if (!sendBytes(Protocol::enablePackBits(), errorOut))  return false;
    }

    const QByteArray modeCmd = mDescriptor->isP700Init()
        ? Protocol::rasterModeP700()
        : Protocol::rasterModeStandard();

    if (!sendBytes(modeCmd, errorOut))  return false;

    // ── Send raster data ────────────────────────────────────────────────────
    //
    // For each column k along the tape-feed axis:
    //   • Build a raster line of lineBytes bytes (all zeros = blank column).
    //   • For each row i across the tape, set the corresponding bit if the
    //     image pixel at (k, i) is dark.
    //   • Send the line (or an empty-line command if the line is all-blank).

    // Work on a greyscale copy so pixel luminance is straightforward.
    QImage mono = image.convertToFormat(QImage::Format_Grayscale8);

    std::vector<uint8_t> rasterLine(static_cast<size_t>(lineBytes), 0);

    for (int k = 0; k < imgW; ++k)
    {
        std::fill(rasterLine.begin(), rasterLine.end(), uint8_t(0));

        bool anySet = false;
        for (int i = 0; i < imgH; ++i)
        {
            // Pixel luminance: 0 = black (print), 255 = white (no print).
            // qGray() on a Grayscale8 image returns the grey value directly.
            //
            // The print head scans from the bottom of the image upward, so
            // image row (imgH-1) maps to raster bit offset+0, and image row 0
            // maps to raster bit offset+(imgH-1).  This matches the libptouch /
            // ptouch-print convention: gdImageGetPixel(im, k, imgH-1-i).
            const int row = imgH - 1 - i;
            if (qGray(mono.pixel(k, row)) < 128)
            {
                rasterSetPixel(rasterLine.data(), lineBytes, offset + i);
                anySet = true;
            }
        }

        QByteArray pkt;
        if (!anySet)
        {
            pkt = Protocol::emptyRasterLine();
        }
        else
        {
            pkt = Protocol::rasterLine(
                std::span<const uint8_t>(rasterLine.data(), rasterLine.size()),
                mDescriptor->isPackBits());
        }

        if (!sendBytes(pkt, errorOut))  return false;
    }

    return true;
}

bool Device::eject(QString& errorOut)
{
    return sendBytes(Protocol::eject(), errorOut);
}

bool Device::formFeed(QString& errorOut)
{
    return sendBytes(Protocol::formFeed(), errorOut);
}

} // namespace glabels::ptouch
