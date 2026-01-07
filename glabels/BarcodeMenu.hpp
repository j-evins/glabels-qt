//  BarcodeMenu.hpp
//
//  Copyright (C) 2014  Jaye Evins <evins@snaught.com>
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

#ifndef BarcodeMenu_hpp
#define BarcodeMenu_hpp


#include "barcode/Style.hpp"

#include <QMenu>


namespace glabels
{

        ///
        /// Barcode Menu
        ///
        class BarcodeMenu : public QMenu
        {
                Q_OBJECT

                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        public:
                BarcodeMenu();


                /////////////////////////////////
                // Signals
                /////////////////////////////////
        signals:
                void selectionChanged();


                /////////////////////////////////
                // Properties
                /////////////////////////////////
        public:
                barcode::Style bcStyle() const;


                /////////////////////////////////
                // Slots
                /////////////////////////////////
        private slots:
                void onMenuItemActivated( const barcode::Style& bcStyle );


                /////////////////////////////////
                // Private Data
                /////////////////////////////////
        private:
                barcode::Style mBcStyle;

        };

}


#endif // BarcodeMenu_hpp
