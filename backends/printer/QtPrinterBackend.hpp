//  Printer/QtPrinterBackend.hpp
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

#ifndef printer_QtPrinterBackend_hpp
#define printer_QtPrinterBackend_hpp


#include "Backend.hpp"

#include <QString>


namespace glabels::printer
{

        ///
        /// Qt Printer Backend (wraps QPrinter for CUPS printers)
        ///
        class QtPrinterBackend : public Backend
        {
                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        public:
                QtPrinterBackend( const QString& printerName );


                /////////////////////////////////
                // Backend Implementation
                /////////////////////////////////
        public:
                QString id() const override;
                QString name() const override;
                bool print( model::PageRenderer* renderer ) override;
                bool isAvailable() const override;
                QString lastError() const override;


                /////////////////////////////////
                // Factory Methods
                /////////////////////////////////
        public:
                static Backend* create( const QString& id );


                /////////////////////////////////
                // Private Data
                /////////////////////////////////
        private:
                QString mPrinterName;
                QString mLastError;
        };

}


#endif // printer_QtPrinterBackend_hpp
