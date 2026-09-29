//  ptouch/DeviceDatabase.hpp
//
//  Compile-time tables of known Brother P-Touch USB devices and tape widths.
//
//  Copyright (C) 2026  gLabels-qt contributors
//  SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <array>
#include <cstdint>
#include <string_view>


namespace glabels::ptouch
{

// ─── Device capability flags ──────────────────────────────────────────────────

enum class DeviceFlags : uint32_t
{
    None           = 0,
    UnsupRaster    = 1u << 0,  ///< Non-standard raster format; device is unsupported
    RasterPackBits = 1u << 1,  ///< Raster rows must be sent PackBits-encoded
    PLite          = 1u << 2,  ///< Device is in P-Lite mode; printing is unsupported
    P700Init       = 1u << 3,  ///< Uses the PT-P700 raster-mode initialisation sequence
};

constexpr DeviceFlags operator|(DeviceFlags a, DeviceFlags b) noexcept
{
    return static_cast<DeviceFlags>( static_cast<uint32_t>(a) | static_cast<uint32_t>(b) );
}

/// Test whether @p set contains @p flag.
constexpr bool hasFlag(DeviceFlags set, DeviceFlags flag) noexcept
{
    return (static_cast<uint32_t>(set) & static_cast<uint32_t>(flag)) != 0;
}


// ─── Device descriptor ────────────────────────────────────────────────────────

/// Immutable compile-time descriptor for one supported P-Touch model.
struct DeviceDescriptor
{
    uint16_t         vid;           ///< USB Vendor ID
    uint16_t         pid;           ///< USB Product ID
    std::string_view name;          ///< Human-readable model name
    int              maxPixelWidth; ///< Print-head width in native-DPI pixels
    int              dpi;           ///< Native print resolution (dots per inch)
    DeviceFlags      flags;

    constexpr bool isValid()        const noexcept { return vid != 0; }
    constexpr bool isPLite()        const noexcept { return hasFlag(flags, DeviceFlags::PLite);          }
    constexpr bool isPackBits()     const noexcept { return hasFlag(flags, DeviceFlags::RasterPackBits); }
    constexpr bool isP700Init()     const noexcept { return hasFlag(flags, DeviceFlags::P700Init);       }
    constexpr bool isUnsupported()  const noexcept { return hasFlag(flags, DeviceFlags::UnsupRaster);    }
};


// ─── Tape descriptor ──────────────────────────────────────────────────────────

/// Maps a physical tape width (in mm, as reported by the printer status) to
/// its printable pixel count and the non-printable side margins.
struct TapeDescriptor
{
    uint8_t  widthMm;       ///< Physical tape width in millimetres
    uint16_t printWidthPx;  ///< Printable pixel columns across the tape
    double   sideMarginMm;  ///< Approximate non-printable margin on each lateral edge
};


// ─── Supported devices ────────────────────────────────────────────────────────

inline constexpr std::array<DeviceDescriptor, 14> kKnownDevices {{
    // vid     pid     name                          maxPx   dpi  flags
    { 0x04f9, 0x2007, "PT-2420PC",                  128,   180,  DeviceFlags::RasterPackBits                         },
    { 0x04f9, 0x202c, "PT-1230PC",                  128,   180,  DeviceFlags::None                                   },
    { 0x04f9, 0x202d, "PT-2430PC",                  128,   180,  DeviceFlags::None                                   },
    { 0x04f9, 0x2030, "PT-1230PC (PLite Mode)",     128,   180,  DeviceFlags::PLite                                  },
    { 0x04f9, 0x2031, "PT-2430PC (PLite Mode)",     128,   180,  DeviceFlags::PLite                                  },
    { 0x04f9, 0x2041, "PT-2730",                    128,   180,  DeviceFlags::None                                   },
    { 0x04f9, 0x205f, "PT-E500",                    128,   180,  DeviceFlags::RasterPackBits                         },
    { 0x04f9, 0x2061, "PT-P700",                    128,   180,  DeviceFlags::RasterPackBits | DeviceFlags::P700Init },
    { 0x04f9, 0x2064, "PT-P700 (PLite Mode)",       128,   180,  DeviceFlags::PLite                                  },
    { 0x04f9, 0x2062, "PT-P750W",                   128,   180,  DeviceFlags::RasterPackBits | DeviceFlags::P700Init },
    { 0x04f9, 0x2065, "PT-P750W (PLite Mode)",      128,   180,  DeviceFlags::PLite                                  },
    { 0x04f9, 0x2073, "PT-D450",                    128,   180,  DeviceFlags::RasterPackBits                         },
    { 0x04f9, 0x2074, "PT-D600",                    128,   180,  DeviceFlags::RasterPackBits                         },
    { 0,      0,      "",                           0,     0,    DeviceFlags::None                                   }, // sentinel
}};


// ─── Supported tape widths ────────────────────────────────────────────────────

inline constexpr std::array<TapeDescriptor, 7> kKnownTapes {{
    //  mm    px    margin
    {  6,    32,   1.0 },
    {  9,    52,   1.0 },
    { 12,    76,   2.0 },
    { 18,   120,   3.0 },
    { 24,   128,   3.0 },
    { 36,   192,   4.5 },
    {  0,     0,   0.0 }, // sentinel
}};


// ─── Lookup helpers ───────────────────────────────────────────────────────────

/// Find a device descriptor by VID/PID.  Returns nullptr if the device is unknown.
inline constexpr const DeviceDescriptor* findDevice(uint16_t vid, uint16_t pid) noexcept
{
    for (const auto& d : kKnownDevices)
    {
        if (d.vid == vid && d.pid == pid)  return &d;
    }
    return nullptr;
}

/// Find a tape descriptor by physical width (mm, as reported in the status packet).
/// Returns nullptr if the width is not in the table.
inline constexpr const TapeDescriptor* findTape(uint8_t widthMm) noexcept
{
    for (const auto& t : kKnownTapes)
    {
        if (t.widthMm == widthMm)  return &t;
    }
    return nullptr;
}

} // namespace glabels::ptouch
