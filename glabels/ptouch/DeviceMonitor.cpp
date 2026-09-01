//  ptouch/DeviceMonitor.cpp
//
//  Copyright (C) 2026  gLabels-qt contributors
//  SPDX-License-Identifier: GPL-3.0-or-later

#include "DeviceMonitor.hpp"

#include <QMutexLocker>
#include <QtConcurrentRun>

#include <algorithm>
#include <chrono>


namespace glabels::ptouch
{

std::unique_ptr<DeviceMonitor> DeviceMonitor::mInstance;

DeviceMonitor::DeviceMonitor()
{
    using namespace std::chrono_literals;

    // Initial synchronous enumeration so the list is populated immediately.
    {
        auto v = mUsbContext.enumeratePtouchDevices();
        mCurrentDevices = QList<UsbDeviceId>(v.cbegin(), v.cend());
    }

    mTimer = std::make_unique<QTimer>(this);
    connect(mTimer.get(), &QTimer::timeout, this, &DeviceMonitor::onTimerTimeout);
    mTimer->start(std::chrono::milliseconds(5s).count());
}

DeviceMonitor* DeviceMonitor::instance()
{
    if (!mInstance)
    {
        mInstance.reset(new DeviceMonitor());
    }
    return mInstance.get();
}

QList<UsbDeviceId> DeviceMonitor::availableDevices() const
{
    QMutexLocker lock(&mDevicesMutex);
    return mCurrentDevices;
}

QStringList DeviceMonitor::availableDeviceNames() const
{
    QMutexLocker lock(&mDevicesMutex);
    QStringList names;
    names.reserve(mCurrentDevices.size());
    for (const auto& id : mCurrentDevices)
    {
        names.append(QString::fromStdString(id.displayName()));
    }
    return names;
}

std::optional<UsbDeviceId> DeviceMonitor::findByDisplayName(const QString& displayName) const
{
    QMutexLocker lock(&mDevicesMutex);
    for (const auto& id : mCurrentDevices)
    {
        if (QString::fromStdString(id.displayName()) == displayName)
        {
            return id;
        }
    }
    return std::nullopt;
}

void DeviceMonitor::onTimerTimeout()
{
    if (mPollFuture.isFinished())
    {
        mPollFuture = QtConcurrent::run(&DeviceMonitor::asyncPoll, this);
    }
}

void DeviceMonitor::asyncPoll()
{
    auto discovered = mUsbContext.enumeratePtouchDevices();

    QList<UsbDeviceId> newList(discovered.cbegin(), discovered.cend());

    // Take a snapshot of the current list under the mutex so the comparison
    // below is race-free (writes to mCurrentDevices also happen under the mutex).
    QList<UsbDeviceId> oldList;
    {
        QMutexLocker lock(&mDevicesMutex);
        oldList = mCurrentDevices;
    }

    // Compare by bus+address+vid+pid
    auto same = [](const UsbDeviceId& a, const UsbDeviceId& b) {
        return a.vid == b.vid && a.pid == b.pid
            && a.busNumber == b.busNumber
            && a.deviceAddress == b.deviceAddress;
    };

    bool changed = (newList.size() != oldList.size());
    if (!changed)
    {
        for (int i = 0; i < newList.size(); ++i)
        {
            if (!same(newList[i], oldList[i]))
            {
                changed = true;
                break;
            }
        }
    }

    if (changed)
    {
        {
            QMutexLocker lock(&mDevicesMutex);
            mCurrentDevices = newList;
        }
        emit availableDevicesChanged(newList);
    }
}

} // namespace glabels::ptouch
