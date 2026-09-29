//  ptouch/Device.hpp
//
//  High-level abstraction of a connected Brother P-Touch label printer.
//  Handles device initialisation, status querying, raster image printing,
//  and tape ejection.
//
//  Usage:
//
//    UsbContext ctx;
//    for (const auto& id : ctx.enumeratePtouchDevices()) {
//        QString err;
//        auto dev = Device::open(ctx, id, err);
//        if (dev) {
//            DeviceStatus st;
//            if (dev->queryStatus(st, err)) {
//                dev->printImage(image, st, err);
//                dev->eject(err);
//            }
//        }
//    }
//
//  Copyright (C) 2026  gLabels-qt contributors
//  SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "DeviceDatabase.hpp"
#include "UsbTransport.hpp"

#include <QImage>
#include <QString>

#include <cstdint>
#include <memory>
#include <optional>


namespace glabels::ptouch
{

// ─── DeviceStatus ─────────────────────────────────────────────────────────────

/// Parsed contents of the 32-byte status packet returned by the printer.
struct DeviceStatus
{
    uint16_t errorCode{};       ///< Raw error field from the status packet
    uint8_t  mediaWidthMm{};    ///< Physical tape width in mm (e.g. 12)
    uint8_t  mediaType{};       ///< Media type code
    uint8_t  tapeColor{};       ///< Tape background colour code
    uint8_t  textColor{};       ///< Text/ribbon colour code
    uint16_t tapePixelWidth{};  ///< Printable pixel columns (looked up from kKnownTapes)

    bool isValid() const noexcept { return tapePixelWidth > 0; }
    bool hasError() const noexcept { return errorCode != 0; }
};


// ─── Device ───────────────────────────────────────────────────────────────────

/// Represents a single open P-Touch device.  Not copyable; move is allowed
/// only via the factory method (which returns a unique_ptr).
class Device
{
public:
    // ── Factory ──────────────────────────────────────────────────────────────

    /// Open the device @p id within USB context @p ctx.
    ///
    /// On success returns a ready-to-use Device.  On failure returns nullptr
    /// and fills @p errorOut with a human-readable message.
    ///
    /// The returned Device has already been hardware-initialised (ESC @).
    static std::unique_ptr<Device> open(UsbContext&        ctx,
                                         const UsbDeviceId& id,
                                         QString&           errorOut);

    ~Device() = default;

    Device(const Device&)            = delete;
    Device& operator=(const Device&) = delete;

    // ── Accessors ────────────────────────────────────────────────────────────

    const DeviceDescriptor& descriptor() const noexcept { return *mDescriptor; }
    const UsbDeviceId&      deviceId()   const noexcept { return mId;          }

    /// User-facing display name (e.g. "PT-P700 (USB bus 1, device 5)").
    QString displayName() const;

    // ── Operations ───────────────────────────────────────────────────────────

    /// Query the printer for its current status (tape width, error state, etc.).
    /// Must be called before printImage() to obtain a valid DeviceStatus.
    bool queryStatus(DeviceStatus& statusOut, QString& errorOut);

    /// Print one label image.
    ///
    /// @p image should be a monochrome or greyscale QImage.  Any format is
    /// accepted; pixels with luminance < 128 are treated as "print" (dark).
    ///
    /// Image orientation: the image width is the label length in the tape-feed
    /// direction; the image height is the print width across the tape.
    ///
    /// The image height must not exceed the device's maxPixelWidth (print-head
    /// capacity).  Images narrower than the tape are centred automatically by
    /// the hardware; images wider than the printable tape area but within
    /// maxPixelWidth will print with content on the non-printable margins.
    ///
    /// @p status must have been obtained from a recent queryStatus() call.
    bool printImage(const QImage& image, const DeviceStatus& status, QString& errorOut);

    /// Advance and cut the tape (send 0x1A).
    bool eject(QString& errorOut);

    /// Advance the tape without cutting (send 0x0C).
    bool formFeed(QString& errorOut);

private:
    Device(std::unique_ptr<UsbTransport> transport,
           const DeviceDescriptor*       descriptor,
           UsbDeviceId                   id);

    bool sendBytes(const QByteArray& data, QString& errorOut);

    std::unique_ptr<UsbTransport> mTransport;
    const DeviceDescriptor*       mDescriptor;
    UsbDeviceId                   mId;
};

} // namespace glabels::ptouch
