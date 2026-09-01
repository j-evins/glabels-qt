//  ptouch/Protocol.hpp
//
//  Stateless functions that encode Brother P-Touch raster protocol commands
//  into QByteArray packets ready to be sent over USB.
//
//  The P-Touch raster protocol uses these key commands:
//    ESC @           – Reset / initialise printer
//    ESC i S         – Request 32-byte status response
//    M 0x02          – Enable PackBits compression mode
//    ESC i R 0x01    – Enter raster graphics mode  (most models)
//    ESC i a 0x01    – Switch to raster mode        (PT-P700 / PT-P750W)
//    0x47 <len> 0x00 <data>            – Raw raster line
//    0x47 <len+1> 0x00 <n-1> <data>   – PackBits-wrapped raster line
//    0x5A                              – Empty raster line (skip column)
//    0x0C                              – Form-feed (advance + no cut)
//    0x1A                              – Eject (advance + cut)
//
//  Copyright (C) 2026  gLabels-qt contributors
//  SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QByteArray>

#include <cstdint>
#include <span>


namespace glabels::ptouch::Protocol
{

// ─── Control commands ─────────────────────────────────────────────────────────

/// ESC @ – reset the printer to its power-on state.
inline QByteArray init()
{
    return QByteArray("\x1b\x40", 2);
}

/// ESC i S – request a 32-byte status packet from the printer.
inline QByteArray statusRequest()
{
    return QByteArray("\x1b\x69\x53", 3);
}

/// M 0x02 – enable PackBits (TIFF) compression for subsequent raster lines.
inline QByteArray enablePackBits()
{
    return QByteArray("\x4d\x02", 2);
}

/// ESC i R 0x01 – enter raster graphics transfer mode (standard models).
inline QByteArray rasterModeStandard()
{
    return QByteArray("\x1b\x69\x52\x01", 4);
}

/// ESC i a 0x01 – enter raster graphics mode (PT-P700 / PT-P750W variant).
inline QByteArray rasterModeP700()
{
    return QByteArray("\x1b\x69\x61\x01", 4);
}

/// 0x5A – send a blank raster line (advance the tape by one dot column without printing).
inline QByteArray emptyRasterLine()
{
    return QByteArray(1, '\x5a');
}

/// 0x0C – form-feed: advance the tape to the cut position without cutting.
inline QByteArray formFeed()
{
    return QByteArray(1, '\x0c');
}

/// 0x1A – eject: advance the tape and cut it.
inline QByteArray eject()
{
    return QByteArray(1, '\x1a');
}


// ─── Raster line encoding ─────────────────────────────────────────────────────

/// Build a single raster-transfer packet from @p rowData.
///
/// @p rowData must be exactly (device.maxPixelWidth / 8) bytes.
///
/// When @p packBits is false (older models), the packet is:
///   0x47  <len>  0x00  <data...>
///
/// When @p packBits is true (FLAG_RASTER_PACKBITS models), the row is wrapped
/// as a single uncompressed PackBits literal run:
///   0x47  <len+1>  0x00  <len-1>  <data...>
///
/// Both encodings result in one dot column being printed.
inline QByteArray rasterLine(std::span<const uint8_t> rowData, bool packBits)
{
    const auto len = static_cast<uint8_t>(rowData.size());

    QByteArray pkt;

    if (packBits)
    {
        // PackBits literal run header:
        //   count byte = 0x00 means "copy next N+1 bytes verbatim", so N = len-1
        pkt.reserve(4 + len);
        pkt.append('\x47');
        pkt.append(static_cast<char>(len + 1));   // payload size (run-header + data)
        pkt.append('\x00');                        // PackBits: literal run
        pkt.append(static_cast<char>(len - 1));   // N (N+1 bytes follow)
    }
    else
    {
        pkt.reserve(3 + len);
        pkt.append('\x47');
        pkt.append(static_cast<char>(len));        // raw payload size
        pkt.append('\x00');                        // padding byte
    }

    pkt.append(reinterpret_cast<const char*>(rowData.data()),
               static_cast<qsizetype>(rowData.size()));
    return pkt;
}

} // namespace glabels::ptouch::Protocol
