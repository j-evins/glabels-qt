//  ptouch/UsbTransport.hpp
//
//  RAII wrappers around libusb-1.0 for P-Touch USB bulk-transfer I/O.
//
//  Neither header includes libusb directly; all libusb types are kept behind
//  void* handles so that downstream code can include this header without
//  depending on the libusb headers.  The actual libusb calls live in
//  UsbTransport.cpp, guarded by #ifdef HAVE_LIBUSB.
//
//  Copyright (C) 2026  gLabels-qt contributors
//  SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "DeviceDatabase.hpp"

#include <cstdint>
#include <memory>
#include <span>
#include <string>
#include <vector>


namespace glabels::ptouch
{

// ─── Discovered device identifier ─────────────────────────────────────────────

/// Identifies a physical USB device currently attached to the host.
/// Bus-number + device-address uniquely identify the device for opening.
struct UsbDeviceId
{
    uint16_t vid;
    uint16_t pid;
    uint8_t  busNumber;
    uint8_t  deviceAddress;

    /// Display string suitable for combo-boxes / log messages.
    std::string displayName() const;
};


// ─── UsbContext ───────────────────────────────────────────────────────────────

/// RAII owner of a libusb session.  Construct once; pass by reference.
///
/// Calls libusb_init() in the constructor and libusb_exit() in the destructor.
/// If initialisation fails, isValid() returns false and error() explains why.
class UsbContext
{
public:
    UsbContext();
    ~UsbContext();

    UsbContext(const UsbContext&)            = delete;
    UsbContext& operator=(const UsbContext&) = delete;

    bool               isValid() const noexcept { return mCtx != nullptr; }
    const std::string& error()   const noexcept { return mError; }

    /// Raw libusb_context* for use by UsbTransport (same translation unit).
    void* rawContext() const noexcept { return mCtx; }

    /// Enumerate all currently connected P-Touch devices that appear in
    /// kKnownDevices.  Returns an empty list if libusb is unavailable or
    /// if no matching device is found.
    std::vector<UsbDeviceId> enumeratePtouchDevices() const;

private:
    void* mCtx{ nullptr };  // libusb_context*
    std::string mError;
};


// ─── UsbTransport ─────────────────────────────────────────────────────────────

/// Open, RAII-managed connection to a single USB device.
///
/// On construction the kernel driver is detached if active (and re-attached on
/// destruction), and interface 0 is claimed (and released on destruction).
///
/// All I/O is synchronous bulk transfer.
class UsbTransport
{
public:
    /// Open the device identified by @p id within @p ctx.
    /// Returns nullptr on failure; @p errorOut receives a description.
    static std::unique_ptr<UsbTransport> open(UsbContext&        ctx,
                                               const UsbDeviceId& id,
                                               std::string&       errorOut);

    ~UsbTransport();

    UsbTransport(const UsbTransport&)            = delete;
    UsbTransport& operator=(const UsbTransport&) = delete;

    /// Bulk-write @p data to endpoint 0x02.
    /// Returns false and fills @p errorOut on failure.
    bool write(std::span<const uint8_t> data, std::string& errorOut);

    /// Bulk-read up to buffer.size() bytes from endpoint 0x81.
    /// @p bytesRead is set to the actual byte count on success.
    /// Returns false and fills @p errorOut on failure.
    bool read(std::span<uint8_t> buffer, int& bytesRead, std::string& errorOut);

private:
    explicit UsbTransport(void* handle, bool driverWasActive);

    void* mHandle;           // libusb_device_handle*
    bool  mDriverWasActive;
};

} // namespace glabels::ptouch
