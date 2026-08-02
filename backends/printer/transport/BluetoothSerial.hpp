//  Printer/Transport/BluetoothSerial.hpp
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

#ifndef printer_transport_BluetoothSerial_hpp
#define printer_transport_BluetoothSerial_hpp


#include <QBluetoothAddress>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothSocket>
#include <QSerialPort>
#include <QString>
#include <memory>


namespace glabels::printer::transport
{

        ///
        /// Bluetooth Serial Transport (RFCOMM)
        /// Supports two modes:
        /// 1. Use existing /dev/rfcomm* device (when connected via system Bluetooth manager)
        /// 2. Create new QBluetoothSocket connection (when not already connected)
        ///
        class BluetoothSerial
        {
                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        public:
                BluetoothSerial( const QBluetoothAddress& address );
                ~BluetoothSerial();


                /////////////////////////////////
                // Public Methods
                /////////////////////////////////
        public:
                bool open();
                void close();
                bool isOpen() const;

                bool write( const QByteArray& data );
                void flush();

                QString lastError() const;


                /////////////////////////////////
                // Private Methods
                /////////////////////////////////
        private:
                bool openViaRfcommDevice();
                bool openViaBluetoothSocket();
                bool openViaServiceDiscovery();
                QString findRfcommDevice(int i);


                /////////////////////////////////
                // Private Data
                /////////////////////////////////
        private:
                QBluetoothAddress                      mAddress;
                std::unique_ptr<QSerialPort>           mPort;          // Used for /dev/rfcomm* mode
                std::unique_ptr<QBluetoothSocket>      mSocket;        // Used for direct connection mode
                QString                                mLastError;
        };

}


#endif // printer_transport_BluetoothSerial_hpp
