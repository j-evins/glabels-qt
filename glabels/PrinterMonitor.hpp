//  PrinterMonitor.hpp
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

#ifndef PrinterMonitor_hpp
#define PrinterMonitor_hpp


#include <QFuture>
#include <QMutex>
#include <QObject>
#include <QStringList>
#include <QTimer>

#ifdef HAVE_BLUETOOTH_SUPPORT
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#endif

#include <memory>


namespace glabels
{

        ///
        /// Printer Monitor
        ///
        class PrinterMonitor : public QObject
        {
                Q_OBJECT

                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        private:
                PrinterMonitor();

        public:
                static PrinterMonitor* instance();


                /////////////////////////////////
                // Public methods
                /////////////////////////////////
        public:
                QStringList availablePrinters();


                /////////////////////////////////
                // Slots
                /////////////////////////////////
        private slots:
                void onTimerTimeout();

#ifdef HAVE_BLUETOOTH_SUPPORT
                void onBluetoothDeviceDiscovered( const QBluetoothDeviceInfo& device );
                void onBluetoothDiscoveryFinished();
                void onBluetoothDiscoveryError( QBluetoothDeviceDiscoveryAgent::Error error );
#endif


                /////////////////////////////////
                // Signals
                /////////////////////////////////
        signals:
                void availablePrintersChanged( QStringList availablePrinters );
                void bluetoothScanningChanged( bool isScanning, bool hasDevices );


                /////////////////////////////////
                // Private methods
                /////////////////////////////////
        private:
                void asyncPoll();

#ifdef HAVE_BLUETOOTH_SUPPORT
                void scanBluetoothDevices();
                bool isPhomemoDevice( const QString& name );
#endif


                /////////////////////////////////
                // Private Members
                /////////////////////////////////
        private:
                static std::unique_ptr<PrinterMonitor> mInstance;

                std::unique_ptr<QTimer> mTimer;

                QStringList mCurrentAvailablePrinters;
                QMutex      mCurrentAvailablePrintersMutex;

                QFuture<void> mPollStatus;

#ifdef HAVE_BLUETOOTH_SUPPORT
                std::unique_ptr<QBluetoothDeviceDiscoveryAgent> mBtAgent;
                QList<QBluetoothDeviceInfo>                     mBtDevices;
                QMutex                                          mBtDevicesMutex;
                bool                                            mBtScanning = false;
#endif
        };

}


#endif // PrinterMonitor_hpp
