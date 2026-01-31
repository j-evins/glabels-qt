//  Printer/Factory.cpp
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


#include "Factory.hpp"

#include "Backend.hpp"
#include "QtPrinterBackend.hpp"
#include "PhomemoBackend.hpp"


namespace glabels::printer
{

        //
        // Static data
        //
        QMap<QString,Factory::BackendEntry> Factory::mBackendMap;


        ///
        /// Constructor
        ///
        Factory::Factory()
        {
                // Register Qt Printer backend (standard CUPS printers)
                // Note: Currently not used - CUPS printers go through direct QPrinter path
                registerBackend( "QtPrinter",
                                 tr("Qt Printer System"),
                                 QT_PRINTER,
                                 &QtPrinterBackend::create );

                // Register Phomemo Bluetooth backend (pattern: "BT:*")
                registerBackend( "BT:",
                                 tr("Phomemo Bluetooth Printer"),
                                 BLUETOOTH_ESCPOS,
                                 &PhomemoBackend::create );
        }


        ///
        /// Initialize
        ///
        void Factory::init()
        {
                static Factory* singletonInstance = nullptr;
                if ( !singletonInstance )
                {
                        singletonInstance = new Factory();
                }
        }


        ///
        /// Create Backend object
        ///
        Backend* Factory::createBackend( const QString& id )
        {
                // Try exact match first
                if ( mBackendMap.contains( id ) )
                {
                        return mBackendMap[id].create( id );
                }

                // Try pattern matching (for Bluetooth devices: "BT:DeviceName:MAC")
                for ( auto it = mBackendMap.begin(); it != mBackendMap.end(); ++it )
                {
                        if ( id.startsWith( it->idPattern ) )
                        {
                                return it->create( id );
                        }
                }

                return nullptr;
        }


        ///
        /// Register backend
        ///
        void Factory::registerBackend( const QString&   idPattern,
                                       const QString&   name,
                                       BackendType      type,
                                       CreateFct        create )
        {
                BackendEntry backend;

                backend.idPattern = idPattern;
                backend.name      = name;
                backend.type      = type;
                backend.create    = create;

                mBackendMap[ idPattern ] = backend;
        }

}
