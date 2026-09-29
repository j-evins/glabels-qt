//  ptouch/DeviceMonitor.hpp
//
//  Singleton QObject that periodically enumerates attached P-Touch USB devices
//  and emits availableDevicesChanged() when the set changes.
//
//  Copyright (C) 2026  gLabels-qt contributors
//  SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "UsbTransport.hpp"

#include <QFuture>
#include <QList>
#include <QMutex>
#include <QObject>
#include <QTimer>

#include <memory>


namespace glabels::ptouch
{

class DeviceMonitor : public QObject
{
    Q_OBJECT

    /////////////////////////////////
    // Life Cycle
    /////////////////////////////////
private:
    DeviceMonitor();

public:
    static DeviceMonitor* instance();


    /////////////////////////////////
    // Public methods
    /////////////////////////////////
public:
    /// Return the most recently enumerated list of connected P-Touch devices.
    QList<UsbDeviceId> availableDevices() const;

    /// Display names for each available device (for combo-boxes).
    QStringList availableDeviceNames() const;

    /// Find the UsbDeviceId matching @p displayName, if present.
    std::optional<UsbDeviceId> findByDisplayName(const QString& displayName) const;

    /// Return the shared UsbContext (valid for the lifetime of this singleton).
    UsbContext& usbContext() { return mUsbContext; }


    /////////////////////////////////
    // Signals
    /////////////////////////////////
signals:
    void availableDevicesChanged(QList<UsbDeviceId> devices);


    /////////////////////////////////
    // Slots
    /////////////////////////////////
private slots:
    void onTimerTimeout();


    /////////////////////////////////
    // Private methods
    /////////////////////////////////
private:
    void asyncPoll();


    /////////////////////////////////
    // Private data
    /////////////////////////////////
private:
    static std::unique_ptr<DeviceMonitor> mInstance;

    UsbContext              mUsbContext;
    QList<UsbDeviceId>      mCurrentDevices;
    mutable QMutex          mDevicesMutex;
    std::unique_ptr<QTimer> mTimer;
    QFuture<void>           mPollFuture;
};

} // namespace glabels::ptouch
