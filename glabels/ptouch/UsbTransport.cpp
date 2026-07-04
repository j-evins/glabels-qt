//  ptouch/UsbTransport.cpp
//
//  Copyright (C) 2026  gLabels-qt contributors
//  SPDX-License-Identifier: GPL-3.0-or-later

#include "UsbTransport.hpp"

#ifdef HAVE_LIBUSB
#  include <libusb-1.0/libusb.h>
#endif

#include <cstring>


namespace glabels::ptouch
{

namespace
{
    constexpr uint8_t kEndpointOut = 0x02;
    constexpr uint8_t kEndpointIn  = 0x81;
    constexpr int     kInterface   = 0;
    constexpr int     kTimeoutMs   = 5000;

#ifdef HAVE_LIBUSB
    // Convenience cast helpers to reduce visual noise.
    inline libusb_context*       asCtx(void* p)    { return static_cast<libusb_context*>(p);       }
    inline libusb_device_handle* asHnd(void* p)    { return static_cast<libusb_device_handle*>(p); }
#endif
}


// ─── UsbDeviceId ─────────────────────────────────────────────────────────────

std::string UsbDeviceId::displayName() const
{
    const auto* desc = findDevice(vid, pid);
    std::string name = desc ? std::string(desc->name) : "Unknown P-Touch";
    return name
           + " (USB bus "    + std::to_string(busNumber)
           + ", device "     + std::to_string(deviceAddress) + ")";
}


// ─── UsbContext ───────────────────────────────────────────────────────────────

UsbContext::UsbContext()
{
#ifdef HAVE_LIBUSB
    libusb_context* ctx = nullptr;
    int rc = libusb_init(&ctx);
    if (rc < 0)
    {
        mError = std::string("libusb_init: ") + libusb_error_name(rc);
        return;
    }
    mCtx = ctx;
#else
    mError = "libusb-1.0 support was not compiled in";
#endif
}

UsbContext::~UsbContext()
{
#ifdef HAVE_LIBUSB
    if (mCtx)
    {
        libusb_exit(asCtx(mCtx));
    }
#endif
}

std::vector<UsbDeviceId> UsbContext::enumeratePtouchDevices() const
{
    std::vector<UsbDeviceId> result;

#ifdef HAVE_LIBUSB
    if (!mCtx)  return result;

    libusb_device** devList  = nullptr;
    ssize_t         devCount = libusb_get_device_list(asCtx(mCtx), &devList);

    if (devCount < 0)  return result;

    for (ssize_t i = 0; i < devCount; ++i)
    {
        libusb_device_descriptor desc{};
        if (libusb_get_device_descriptor(devList[i], &desc) < 0)  continue;

        if (findDevice(desc.idVendor, desc.idProduct) != nullptr)
        {
            result.push_back({
                desc.idVendor,
                desc.idProduct,
                libusb_get_bus_number(devList[i]),
                libusb_get_device_address(devList[i])
            });
        }
    }

    libusb_free_device_list(devList, 1);
#endif

    return result;
}


// ─── UsbTransport ─────────────────────────────────────────────────────────────

UsbTransport::UsbTransport(void* handle, bool driverWasActive)
    : mHandle(handle), mDriverWasActive(driverWasActive)
{}

UsbTransport::~UsbTransport()
{
#ifdef HAVE_LIBUSB
    if (!mHandle)  return;

    libusb_release_interface(asHnd(mHandle), kInterface);

    if (mDriverWasActive)
    {
        libusb_attach_kernel_driver(asHnd(mHandle), kInterface);
    }

    libusb_close(asHnd(mHandle));
#endif
}

std::unique_ptr<UsbTransport> UsbTransport::open(UsbContext&        ctx,
                                                   const UsbDeviceId& id,
                                                   std::string&       errorOut)
{
#ifdef HAVE_LIBUSB
    if (!ctx.isValid())
    {
        errorOut = ctx.error();
        return nullptr;
    }

    libusb_device** devList  = nullptr;
    ssize_t         devCount = libusb_get_device_list(asCtx(ctx.rawContext()), &devList);

    if (devCount < 0)
    {
        errorOut = std::string("libusb_get_device_list: ")
                   + libusb_error_name(static_cast<int>(devCount));
        return nullptr;
    }

    libusb_device* target = nullptr;
    for (ssize_t i = 0; i < devCount && !target; ++i)
    {
        if (libusb_get_bus_number(devList[i])     == id.busNumber     &&
            libusb_get_device_address(devList[i]) == id.deviceAddress)
        {
            target = devList[i];
        }
    }

    if (!target)
    {
        libusb_free_device_list(devList, 1);
        errorOut = "P-Touch device is no longer connected";
        return nullptr;
    }

    libusb_device_handle* handle = nullptr;
    int rc = libusb_open(target, &handle);
    libusb_free_device_list(devList, 1);

    if (rc < 0)
    {
        errorOut = std::string("libusb_open: ") + libusb_error_name(rc);
        return nullptr;
    }

    // Detach the kernel driver (e.g. usblp) if it has claimed the interface.
    bool driverWasActive = false;
    if (libusb_kernel_driver_active(handle, kInterface) == 1)
    {
        driverWasActive = true;
        rc = libusb_detach_kernel_driver(handle, kInterface);
        if (rc < 0)
        {
            libusb_close(handle);
            errorOut = std::string("libusb_detach_kernel_driver: ") + libusb_error_name(rc);
            return nullptr;
        }
    }

    rc = libusb_claim_interface(handle, kInterface);
    if (rc < 0)
    {
        if (driverWasActive)  libusb_attach_kernel_driver(handle, kInterface);
        libusb_close(handle);
        errorOut = std::string("libusb_claim_interface: ") + libusb_error_name(rc);
        return nullptr;
    }

    // Use `new` directly because the constructor is private.
    return std::unique_ptr<UsbTransport>(new UsbTransport(handle, driverWasActive));

#else
    (void)ctx;  (void)id;
    errorOut = "libusb-1.0 support was not compiled in";
    return nullptr;
#endif
}

bool UsbTransport::write(std::span<const uint8_t> data, std::string& errorOut)
{
#ifdef HAVE_LIBUSB
    if (!mHandle)
    {
        errorOut = "USB device not open";
        return false;
    }

    // libusb_bulk_transfer takes a non-const pointer but does not modify the data.
    auto* buf = const_cast<uint8_t*>(data.data());
    int   transferred = 0;

    int rc = libusb_bulk_transfer(
        asHnd(mHandle),
        kEndpointOut,
        buf,
        static_cast<int>(data.size()),
        &transferred,
        kTimeoutMs);

    if (rc < 0)
    {
        errorOut = std::string("USB write: ") + libusb_error_name(rc);
        return false;
    }
    if (transferred != static_cast<int>(data.size()))
    {
        errorOut = "USB write: short write ("
                   + std::to_string(transferred) + " of "
                   + std::to_string(data.size()) + " bytes)";
        return false;
    }
    return true;

#else
    (void)data;
    errorOut = "libusb-1.0 support was not compiled in";
    return false;
#endif
}

bool UsbTransport::read(std::span<uint8_t> buffer, int& bytesRead, std::string& errorOut)
{
#ifdef HAVE_LIBUSB
    if (!mHandle)
    {
        errorOut = "USB device not open";
        bytesRead = 0;
        return false;
    }

    bytesRead = 0;
    int rc = libusb_bulk_transfer(
        asHnd(mHandle),
        kEndpointIn,
        buffer.data(),
        static_cast<int>(buffer.size()),
        &bytesRead,
        kTimeoutMs);

    if (rc < 0)
    {
        errorOut = std::string("USB read: ") + libusb_error_name(rc);
        return false;
    }
    return true;

#else
    (void)buffer;  (void)bytesRead;
    errorOut = "libusb-1.0 support was not compiled in";
    return false;
#endif
}

} // namespace glabels::ptouch
