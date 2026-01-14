//  BarcodeMenuItem.cpp
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


#include "BarcodeMenuItem.hpp"

#include <QDebug>


namespace glabels
{

        ///
        /// Constructor From Data
        ///
        BarcodeMenuItem::BarcodeMenuItem( const barcode::Style& bcStyle, QObject* parent )
                : QAction(parent), mBcStyle(bcStyle)
        {
                setText( bcStyle.name() );

                connect( this, SIGNAL(triggered()), this, SLOT(onTriggered()) );
        }


        ///
        /// bcStyle Property Getter
        ///
        barcode::Style BarcodeMenuItem::bcStyle() const
        {
                return mBcStyle;
        }


        ///
        /// onTriggered slot
        ///
        void BarcodeMenuItem::onTriggered()
        {
                emit activated( mBcStyle );
        }

} // namespace glabels
