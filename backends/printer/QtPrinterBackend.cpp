//  Printer/QtPrinterBackend.cpp
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


#include "QtPrinterBackend.hpp"

#include "model/PageRenderer.hpp"

#include <QPrinter>
#include <QPrinterInfo>


namespace glabels::printer
{

        ///
        /// Constructor
        ///
        QtPrinterBackend::QtPrinterBackend( const QString& printerName )
                : mPrinterName( printerName )
        {
        }


        ///
        /// Get backend ID
        ///
        QString QtPrinterBackend::id() const
        {
                return mPrinterName;
        }


        ///
        /// Get backend name
        ///
        QString QtPrinterBackend::name() const
        {
                return mPrinterName;
        }


        ///
        /// Print using Qt Printer (CUPS)
        ///
        bool QtPrinterBackend::print( model::PageRenderer* renderer )
        {
                QPrinter printer( QPrinter::HighResolution );
                printer.setPrinterName( mPrinterName );
                printer.setColorMode( QPrinter::Color );

                try
                {
                        renderer->print( &printer );
                        mLastError.clear();
                        return true;
                }
                catch ( ... )
                {
                        mLastError = QString("Failed to print to %1").arg(mPrinterName);
                        return false;
                }
        }


        ///
        /// Check if printer is available
        ///
        bool QtPrinterBackend::isAvailable() const
        {
                auto printerInfo = QPrinterInfo::printerInfo( mPrinterName );
                return !printerInfo.isNull();
        }


        ///
        /// Get last error message
        ///
        QString QtPrinterBackend::lastError() const
        {
                return mLastError;
        }


        ///
        /// Factory create method
        ///
        Backend* QtPrinterBackend::create( const QString& id )
        {
                return new QtPrinterBackend( id );
        }

}
