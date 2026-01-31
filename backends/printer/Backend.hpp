//  Printer/Backend.hpp
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

#ifndef printer_Backend_hpp
#define printer_Backend_hpp


#include <QString>


namespace glabels::model
{
        // Forward reference
        class PageRenderer;
}


namespace glabels::printer
{

        ///
        /// Printer Backend Abstract Base Class
        ///
        class Backend
        {
                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        public:
                virtual ~Backend() = default;


                /////////////////////////////////
                // Virtual Methods
                /////////////////////////////////
        public:
                // Identification
                virtual QString id() const = 0;
                virtual QString name() const = 0;

                // Print operation
                virtual bool print( model::PageRenderer* renderer ) = 0;

                // Availability
                virtual bool isAvailable() const = 0;
                virtual QString lastError() const = 0;
        };

}


#endif // printer_Backend_hpp
