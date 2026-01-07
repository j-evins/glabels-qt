//  BarcodeMenu.cpp
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


#include "BarcodeMenu.hpp"

#include "BarcodeMenuItem.hpp"

#include "barcode/Backends.hpp"

#include <QDebug>


namespace glabels
{

        ///
        /// Constructor
        ///
        BarcodeMenu::BarcodeMenu()
        {
                for ( const barcode::Style& bcStyle : barcode::Backends::styleList() )
                {
                        if ( bcStyle.backendId() == "" )
                        {
                                auto* bcMenuItem = new BarcodeMenuItem( bcStyle );
                                connect( bcMenuItem, SIGNAL(activated(const barcode::Style&)),
                                         this, SLOT(onMenuItemActivated(const barcode::Style&)) );

                                addAction( bcMenuItem );
                        }
                }

                for ( const QString& backendId : barcode::Backends::backendList() )
                {
                        QMenu* subMenu = addMenu( barcode::Backends::backendName( backendId ) );

                        for ( const barcode::Style& bcStyle : barcode::Backends::styleList() )
                        {
                                if ( bcStyle.backendId() == backendId )
                                {
                                        auto* bcMenuItem = new BarcodeMenuItem( bcStyle );
                                        connect( bcMenuItem, SIGNAL(activated(const barcode::Style&)),
                                                 this, SLOT(onMenuItemActivated(const barcode::Style&)) );

                                        subMenu->addAction( bcMenuItem );
                                }
                        }
                }
        }


        ///
        /// bcStyle getter
        ///
        barcode::Style BarcodeMenu::bcStyle() const
        {
                return mBcStyle;
        }


        ///
        /// onMenuItemActivated slot
        ///
        void BarcodeMenu::onMenuItemActivated( const barcode::Style& bcStyle )
        {
                mBcStyle = bcStyle;

                emit selectionChanged();
        }

} // namespace glabels
