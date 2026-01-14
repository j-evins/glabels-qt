//  BarcodeMenuItem.hpp
//
//  Copyright (C) 2014-2026  Jaye Evins <evins@snaught.com>
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

#ifndef BarcodeMenuItem_hpp
#define BarcodeMenuItem_hpp


#include "barcode/Style.hpp"

#include <QAction>


namespace glabels
{

        ///
        /// Barcode Menu Item
        ///
        class BarcodeMenuItem : public QAction
        {
                Q_OBJECT

                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        public:
                BarcodeMenuItem( const barcode::Style& bcStyle, QObject* parent = nullptr );


                /////////////////////////////////
                // Signals
                /////////////////////////////////
        signals:
                void activated( const barcode::Style& bcStyle );


                /////////////////////////////////
                // Properties
                /////////////////////////////////
        public:
                barcode::Style bcStyle() const;


                /////////////////////////////////
                // Slots
                /////////////////////////////////
        private slots:
                void onTriggered();


                /////////////////////////////////
                // Private Data
                /////////////////////////////////
        private:
                barcode::Style mBcStyle;

        };

}


#endif // BarcodeMenuItem_hpp
