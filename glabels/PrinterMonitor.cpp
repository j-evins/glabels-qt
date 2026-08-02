//  PrinterMonitor.cpp
//
//  Copyright (C) 2025-2026  Jaye Evins <evins@snaught.com>
//
//  This file is part of gLabels-qt.
//
//  gLabels-qt is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  gLabels-qt is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
//


#include "PrinterMonitor.hpp"

#include <QDebug>
#include <QMutexLocker>
#include <QPrinterInfo>
#include <QtConcurrentRun>


namespace glabels
{

        ///
        /// Static data
        ///
        std::unique_ptr<PrinterMonitor> PrinterMonitor::mInstance;


        ///
        /// Constructor
        ///
        PrinterMonitor::PrinterMonitor()
        {
                using namespace std::chrono_literals;

                mCurrentAvailablePrinters = QPrinterInfo::availablePrinterNames();

#ifdef HAVE_BLUETOOTH_SUPPORT
                // Initialize Bluetooth device discovery
                mBtAgent.reset( new QBluetoothDeviceDiscoveryAgent( this ) );
                connect( mBtAgent.get(), &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
                         this, &PrinterMonitor::onBluetoothDeviceDiscovered );
                connect( mBtAgent.get(), &QBluetoothDeviceDiscoveryAgent::finished,
                         this, &PrinterMonitor::onBluetoothDiscoveryFinished );
                connect( mBtAgent.get(), QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::errorOccurred),
                         this, &PrinterMonitor::onBluetoothDiscoveryError );

                // Don't scan immediately - let the timer handle it to avoid blocking UI startup
#endif

                mTimer.reset( new QTimer( this ) );
                connect( mTimer.get(), SIGNAL(timeout()), this, SLOT(onTimerTimeout()) );
                mTimer->start( 10s );
        }


        ///
        /// Get singleton instance
        ///
        PrinterMonitor* PrinterMonitor::instance()
        {
                if ( !mInstance )
                {
                        mInstance.reset( new PrinterMonitor() );
                }

                return mInstance.get();
        }


        ///
        /// Get available printers
        ///
        QStringList PrinterMonitor::availablePrinters()
        {
                QMutexLocker mutex( &mCurrentAvailablePrintersMutex );
                return mCurrentAvailablePrinters;
        }


        ///
        /// On timer timeout
        ///
        void PrinterMonitor::onTimerTimeout()
        {
#ifdef HAVE_BLUETOOTH_SUPPORT
                // Trigger Bluetooth scan in main thread (Qt Bluetooth requires this)
                scanBluetoothDevices();
#endif

                // Make sure previous poll is complete before starting a new one
                if ( mPollStatus.isFinished() )
                {
                        mPollStatus = QtConcurrent::run( &PrinterMonitor::asyncPoll, this );
                }
        }


        ///
        /// Asynchronous poll
        ///
        void PrinterMonitor::asyncPoll()
        {
                // Get CUPS printers
                auto cupsPrinters = QPrinterInfo::availablePrinterNames();

                // Combine with Bluetooth printers
                QStringList combinedPrinters = cupsPrinters;

#ifdef HAVE_BLUETOOTH_SUPPORT
                // Add Bluetooth devices with "BT:DeviceName:MAC" format
                // (Bluetooth scanning happens in main thread via onTimerTimeout)
                QMutexLocker btMutex( &mBtDevicesMutex );
                for ( const auto& device : mBtDevices )
                {
                        QString deviceId = QString("BT:%1:%2")
                                           .arg( device.name() )
                                           .arg( device.address().toString() );
                        combinedPrinters.append( deviceId );
                }
#endif

                // Check if printer list changed
                if ( combinedPrinters != mCurrentAvailablePrinters )
                {
                        QMutexLocker mutex( &mCurrentAvailablePrintersMutex );

                        mCurrentAvailablePrinters = combinedPrinters;

                        emit availablePrintersChanged( mCurrentAvailablePrinters );
                }
        }



#ifdef HAVE_BLUETOOTH_SUPPORT
        ///
        /// Scan for Bluetooth devices
        ///
        void PrinterMonitor::scanBluetoothDevices()
        {
                if ( mBtAgent && !mBtAgent->isActive() )
                {
                        // Don't clear previous results - keep already discovered devices
                        // to avoid flickering in the UI

                        // Emit scanning started signal
                        mBtScanning = true;
                        emit bluetoothScanningChanged( true, !mBtDevices.isEmpty() );

                        // Start discovery for classic Bluetooth devices (not BLE)
                        mBtAgent->start( QBluetoothDeviceDiscoveryAgent::ClassicMethod );
                }
        }


        ///
        /// Bluetooth device discovered slot
        ///
        void PrinterMonitor::onBluetoothDeviceDiscovered( const QBluetoothDeviceInfo& device )
        {
                // Filter for Phomemo devices
                if ( isPhomemoDevice( device.name() ) )
                {
                        QMutexLocker mutex( &mBtDevicesMutex );

                        // Add if not already in list
                        bool found = false;
                        for ( const auto& existing : mBtDevices )
                        {
                                if ( existing.address() == device.address() )
                                {
                                        found = true;
                                        break;
                                }
                        }

                        if ( !found )
                        {
                                mBtDevices.append( device );
                                qDebug() << "Found Phomemo device:" << device.name()
                                         << "at" << device.address().toString();
                        }
                }
        }


        ///
        /// Check if device name indicates a Phomemo printer
        ///
        bool PrinterMonitor::isPhomemoDevice( const QString& name )
        {
                return name.startsWith( "Phomemo", Qt::CaseInsensitive ) ||
                       name.startsWith( "D30", Qt::CaseInsensitive ) ||
                       name.startsWith( "D35", Qt::CaseInsensitive ) ||
                       name.startsWith( "M02", Qt::CaseInsensitive );
        }


        ///
        /// Bluetooth discovery finished slot
        ///
        void PrinterMonitor::onBluetoothDiscoveryFinished()
        {
                mBtScanning = false;
                emit bluetoothScanningChanged( false, !mBtDevices.isEmpty() );

                qDebug() << "Bluetooth discovery finished. Found" << mBtDevices.size() << "Phomemo device(s).";
        }


        ///
        /// Bluetooth discovery error slot
        ///
        void PrinterMonitor::onBluetoothDiscoveryError( QBluetoothDeviceDiscoveryAgent::Error error )
        {
                mBtScanning = false;
                emit bluetoothScanningChanged( false, !mBtDevices.isEmpty() );

                qDebug() << "Bluetooth discovery error:" << error;
        }
#endif


} // namespace glabels
