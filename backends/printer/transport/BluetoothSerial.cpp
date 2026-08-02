//  Printer/Transport/BluetoothSerial.cpp
//
//  Copyright (C) 2026  gLabels-qt contributors
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


#include "BluetoothSerial.hpp"

#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>
#include <QBluetoothUuid>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QTimer>


namespace glabels::printer::transport
{

        ///
        /// Constructor
        ///
        BluetoothSerial::BluetoothSerial( const QBluetoothAddress& address )
                : mAddress( address )
        {
        }


        ///
        /// Destructor
        ///
        BluetoothSerial::~BluetoothSerial()
        {
                close();
        }


        ///
        /// Open Bluetooth connection (tries multiple modes)
        ///
        /// Connection modes:
        ///   1. (Linux only) /dev/rfcomm* device - uses system-bound RFCOMM device
        ///   2. (Cross-platform) Service discovery - discovers SPP service first
        ///   3. (Cross-platform) Direct UUID - connects directly to SPP UUID
        ///
        /// On Windows, mode 1 is skipped and modes 2/3 are used.
        ///
        bool BluetoothSerial::open()
        {
                if ( isOpen() )
                {
                        return true;
                }

                // Mode 1: Try to use existing /dev/rfcomm* device (Linux only)
#ifdef Q_OS_LINUX
                qDebug() << "Phomemo: Attempting to connect via existing /dev/rfcomm* device...";
                if ( openViaRfcommDevice() )
                {
                        qDebug() << "Phomemo: Connected via /dev/rfcomm* device";
                        return true;
                }
#else
                qDebug() << "Phomemo: Skipping /dev/rfcomm* device mode (not supported on this platform)";
#endif

                // Mode 2: Try service discovery to find correct channel
                qDebug() << "Phomemo: Attempting to connect via service discovery...";
                if ( openViaServiceDiscovery() )
                {
                        qDebug() << "Phomemo: Connected via service discovery";
                        return true;
                }

                // Mode 3: Fall back to direct UUID connection
                qDebug() << "Phomemo: Attempting to connect via direct UUID...";
                if ( openViaBluetoothSocket() )
                {
                        qDebug() << "Phomemo: Connected via direct UUID";
                        return true;
                }

                // All modes failed
                qDebug() << "Phomemo: All connection modes failed";
                return false;
        }


        ///
        /// Close Bluetooth connection
        ///
        void BluetoothSerial::close()
        {
                if ( mPort )
                {
                        if ( mPort->isOpen() )
                        {
                                mPort->close();
                        }
                        mPort.reset();
                }

                if ( mSocket )
                {
                        if ( mSocket->isOpen() )
                        {
                                mSocket->close();
                        }
                        mSocket.reset();
                }
        }


        ///
        /// Check if connection is open
        ///
        bool BluetoothSerial::isOpen() const
        {
                return ( mPort && mPort->isOpen() ) || ( mSocket && mSocket->isOpen() );
        }


        ///
        /// Write data to Bluetooth device
        ///
        bool BluetoothSerial::write( const QByteArray& data )
        {
                qDebug() << "Phomemo: write() called with" << data.size() << "bytes";
                qDebug() << "Phomemo: isOpen():" << isOpen()
                         << "mPort:" << (mPort ? "exists" : "null")
                         << "mSocket:" << (mSocket ? "exists" : "null");

                if ( mSocket )
                {
                        qDebug() << "Phomemo: Socket state:" << mSocket->state()
                                 << "isOpen:" << mSocket->isOpen()
                                 << "error:" << mSocket->error();
                }

                if ( !isOpen() )
                {
                        mLastError = "Cannot write while not connected";
                        return false;
                }

                // Write via QSerialPort if using /dev/rfcomm* mode
                if ( mPort && mPort->isOpen() )
                {
                        qint64 bytesWritten = mPort->write( data );
                        if ( bytesWritten != data.size() )
                        {
                                mLastError = QString("Failed to write data: %1")
                                             .arg( mPort->errorString() );
                                return false;
                        }
                        return true;
                }

                // Write via QBluetoothSocket if using socket mode
                if ( mSocket && mSocket->isOpen() )
                {
                        qDebug() << "Phomemo: Attempting to write" << data.size() << "bytes via socket...";
                        qint64 bytesWritten = mSocket->write( data );
                        qDebug() << "Phomemo: mSocket->write() returned:" << bytesWritten << "bytes";

                        if ( bytesWritten != data.size() )
                        {
                                mLastError = QString("Failed to write data: %1")
                                             .arg( mSocket->errorString() );
                                qDebug() << "Phomemo: Write size mismatch! Expected:" << data.size() << "Actual:" << bytesWritten;
                                return false;
                        }

                        // Note: Don't use waitForBytesWritten() for Bluetooth sockets - it often times out
                        // even when data is being transmitted correctly. Just write and return.
                        qDebug() << "Phomemo: Write successful (queued" << bytesWritten << "bytes)";
                        return true;
                }

                qDebug() << "Phomemo: No connection available (shouldn't reach here)";
                mLastError = "No connection available";
                return false;
        }


        ///
        /// Flush data (ensure all data is sent)
        ///
        void BluetoothSerial::flush()
        {
                if ( mPort && mPort->isOpen() )
                {
                        mPort->flush();
                        mPort->waitForBytesWritten( 1000 );
                }

                // Note: For Bluetooth sockets, flush is implicit with write()
                // waitForBytesWritten() often doesn't work correctly with QBluetoothSocket
                if ( mSocket && mSocket->isOpen() )
                {
                        // Just ensure the socket processes events
                        QCoreApplication::processEvents();
                }
        }


        ///
        /// Get last error message
        ///
        QString BluetoothSerial::lastError() const
        {
                return mLastError;
        }


        ///
        /// Open via existing /dev/rfcomm* device (Linux only)
        ///
#ifdef Q_OS_LINUX
        bool BluetoothSerial::openViaRfcommDevice()
        {
                // Look for /dev/rfcomm0 through /dev/rfcomm99
                for ( int i = 0; i < 100; i++ )
                {
                        QString devicePath = findRfcommDevice(i);

                        // Skip if this index doesn't exist, continue to next
                        if ( devicePath.isEmpty() )
                        {
                                continue;  // Try next index
                        }

                        qDebug() << "Found RFCOMM device:" << devicePath;

                        mPort.reset( new QSerialPort( devicePath ) );
                        mPort->setBaudRate( QSerialPort::Baud9600 );
                        mPort->setDataBits( QSerialPort::Data8 );
                        mPort->setParity( QSerialPort::NoParity );
                        mPort->setStopBits( QSerialPort::OneStop );
                        mPort->setFlowControl( QSerialPort::NoFlowControl );

                        if ( !mPort->open( QIODevice::WriteOnly ) )
                        {
                                mLastError = QString("Failed to open %1: %2")
                                             .arg( devicePath )
                                             .arg( mPort->errorString() );
                                mPort.reset();
                                // Try next device
                                continue;
                        }
                        else
                        {
                                // Successfully opened!
                                mLastError.clear();
                                return true;
                        }
                }

                // Checked all 100 devices, none worked
                mLastError = QString("No /dev/rfcomm* device found for %1")
                             .arg( mAddress.toString() );
                return false;
        }


        ///
        /// Find /dev/rfcomm* device for this Bluetooth address
        ///
        QString BluetoothSerial::findRfcommDevice(int i)
        {

                QString devicePath = QString("/dev/rfcomm%1").arg(i);
                QString sysfsPath = QString("/sys/class/tty/rfcomm%1/address").arg(i);

                if ( !QFile::exists( devicePath ) )
                {
                        return "";
                }

                qDebug() << "Phomemo: Checking RFCOMM device:" << devicePath;

                // Verify this device corresponds to our Bluetooth address
                QFile sysfsFile( sysfsPath );
                if ( sysfsFile.open( QIODevice::ReadOnly | QIODevice::Text ) )
                {
                        QString deviceAddress = QString::fromUtf8( sysfsFile.readAll().trimmed() );
                        sysfsFile.close();

                        qDebug() << "Phomemo: Device" << devicePath << "has address" << deviceAddress
                                                << "looking for" << mAddress.toString();

                        if ( deviceAddress.compare( mAddress.toString(), Qt::CaseInsensitive ) == 0 )
                        {
                                qDebug() << "Phomemo: Found matching RFCOMM device:" << devicePath;
                                return devicePath;
                        }
                }
                else
                {
                        // Sysfs not available (might not be Linux), just use first available device
                        qDebug() << "Phomemo: Cannot verify address (sysfs unavailable), using:" << devicePath;
                        return devicePath;
                }
                return "";
        }
#endif // Q_OS_LINUX


        ///
        /// Open via service discovery (discovers SPP service first)
        ///
        bool BluetoothSerial::openViaServiceDiscovery()
        {
                qDebug() << "Phomemo: Starting service discovery on" << mAddress.toString();

                QBluetoothServiceDiscoveryAgent discoveryAgent;
                discoveryAgent.setRemoteAddress( mAddress );

                QEventLoop loop;
                QTimer timer;
                timer.setSingleShot( true );
                QList<QBluetoothServiceInfo> foundServices;
                bool discoveryFinished = false;
                bool discoveryCanceled = false;
                QString discoveryError;

                QObject::connect( &discoveryAgent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered,
                                  [&foundServices]( const QBluetoothServiceInfo& info ) {
                                          foundServices.append( info );
                                          qDebug() << "Phomemo: Found service:" << info.serviceName()
                                                   << "UUID:" << info.serviceUuid()
                                                   << "Device:" << info.device().name()
                                                   << "Protocol:" << info.socketProtocol();
                                  });

                QObject::connect( &discoveryAgent, &QBluetoothServiceDiscoveryAgent::finished,
                                  [&loop, &discoveryFinished]() {
                                          qDebug() << "Phomemo: Service discovery finished normally";
                                          discoveryFinished = true;
                                          loop.quit();
                                  });

                QObject::connect( &discoveryAgent, &QBluetoothServiceDiscoveryAgent::canceled,
                                  [&loop, &discoveryCanceled]() {
                                          qDebug() << "Phomemo: Service discovery was canceled";
                                          discoveryCanceled = true;
                                          loop.quit();
                                  });

                QObject::connect( &discoveryAgent, &QBluetoothServiceDiscoveryAgent::errorOccurred,
                                  [&loop, &discoveryError]( QBluetoothServiceDiscoveryAgent::Error error ) {
                                          qDebug() << "Phomemo: Service discovery error:" << error;
                                          discoveryError = QString("Discovery error: %1").arg( error );
                                          loop.quit();
                                  });

                QObject::connect( &timer, &QTimer::timeout, [&discoveryAgent, &loop]() {
                        qDebug() << "Phomemo: Service discovery timeout, stopping...";
                        discoveryAgent.stop();
                        loop.quit();
                });

                timer.start( 30000 );  // 30 second timeout for service discovery
                discoveryAgent.start( QBluetoothServiceDiscoveryAgent::FullDiscovery );
                qDebug() << "Phomemo: Service discovery agent started";
                loop.exec();

                qDebug() << "Phomemo: Service discovery complete."
                         << "Found" << foundServices.size() << "services"
                         << "Finished:" << discoveryFinished
                         << "Canceled:" << discoveryCanceled
                         << "Error:" << discoveryError;

                if ( !discoveryError.isEmpty() )
                {
                        mLastError = discoveryError;
                        return false;
                }

                // Find SPP service
                for ( const auto& service : foundServices )
                {
                        qDebug() << "Phomemo: Checking service:" << service.serviceName()
                                 << "UUID:" << service.serviceUuid()
                                 << "Class UUIDs:" << service.serviceClassUuids();

                        // Check if this is a Serial Port Profile service
                        if ( service.serviceUuid() == QBluetoothUuid::ServiceClassUuid::SerialPort ||
                             service.serviceClassUuids().contains( QBluetoothUuid::ServiceClassUuid::SerialPort ) )
                        {
                                qDebug() << "Phomemo: Found SPP service, attempting connection...";
                                qDebug() << "Phomemo: Service details:"
                                         << "Name:" << service.serviceName()
                                         << "Description:" << service.serviceDescription()
                                         << "Provider:" << service.serviceProvider();

                                mSocket.reset( new QBluetoothSocket( QBluetoothServiceInfo::RfcommProtocol ) );
                                mSocket->connectToService( service );

                                QEventLoop connLoop;
                                QTimer connTimer;
                                connTimer.setSingleShot( true );

                                QObject::connect( mSocket.get(), &QBluetoothSocket::connected,
                                                  [&connLoop]() {
                                                          qDebug() << "Phomemo: Socket connected successfully";
                                                          connLoop.quit();
                                                  });

                                QObject::connect( mSocket.get(), &QBluetoothSocket::errorOccurred,
                                                  [this, &connLoop]( QBluetoothSocket::SocketError error ) {
                                                          qDebug() << "Phomemo: Socket connection error:" << error
                                                                   << mSocket->errorString();
                                                          connLoop.quit();
                                                  });

                                QObject::connect( &connTimer, &QTimer::timeout,
                                                  [&connLoop]() {
                                                          qDebug() << "Phomemo: Socket connection timeout";
                                                          connLoop.quit();
                                                  });

                                connTimer.start( 10000 );
                                connLoop.exec();

                                if ( mSocket->isOpen() )
                                {
                                        qDebug() << "Phomemo: Successfully connected to SPP service";
                                        mLastError.clear();
                                        return true;
                                }
                                else
                                {
                                        qDebug() << "Phomemo: Failed to connect to SPP service:" << mSocket->errorString();
                                        mSocket.reset();
                                }
                        }
                }

                mLastError = QString("No SPP service found on device %1 (found %2 services)")
                             .arg( mAddress.toString() )
                             .arg( foundServices.size() );
                return false;
        }


        ///
        /// Open via QBluetoothSocket (creates new connection)
        ///
        bool BluetoothSerial::openViaBluetoothSocket()
        {
                mSocket.reset( new QBluetoothSocket( QBluetoothServiceInfo::RfcommProtocol ) );

                // Connect using Serial Port Profile UUID
                QBluetoothUuid sppUuid( QBluetoothUuid::ServiceClassUuid::SerialPort );
                qDebug() << "Connecting to SPP UUID on" << mAddress.toString();
                mSocket->connectToService( mAddress, sppUuid );

                // Wait for connection with 10 second timeout
                QEventLoop loop;
                QTimer timer;
                timer.setSingleShot( true );

                QObject::connect( mSocket.get(), &QBluetoothSocket::connected,
                                  &loop, &QEventLoop::quit );
                QObject::connect( mSocket.get(), &QBluetoothSocket::errorOccurred,
                                  &loop, &QEventLoop::quit );
                QObject::connect( &timer, &QTimer::timeout,
                                  &loop, &QEventLoop::quit );

                timer.start( 10000 );
                loop.exec();

                qDebug() << "Phomemo: After event loop - socket state:" << mSocket->state()
                         << "isOpen:" << mSocket->isOpen()
                         << "error:" << mSocket->error()
                         << "errorString:" << mSocket->errorString();

                if ( !mSocket->isOpen() )
                {
                        mLastError = QString("Failed to connect via Bluetooth socket: %1")
                                     .arg( mSocket->errorString() );
                        qDebug() << "Connection error:" << mSocket->error() << mSocket->errorString();
                        mSocket.reset();
                        return false;
                }

                qDebug() << "Phomemo: Socket is open, returning true";
                mLastError.clear();
                return true;
        }
}
