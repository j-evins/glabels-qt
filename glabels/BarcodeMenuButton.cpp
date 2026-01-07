//  BarcodeMenuButton.cpp
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


#include "BarcodeMenuButton.hpp"

#include "BarcodeMenuItem.hpp"

#include "barcode/Backends.hpp"

#include <QDebug>


namespace glabels
{

        ///
        /// Constructor
        ///
        BarcodeMenuButton::BarcodeMenuButton( QWidget* parent )
                : QPushButton(parent)
        {
                mMenu = new BarcodeMenu();
                setMenu( mMenu );

                mBcStyle = barcode::Backends::defaultStyle();
                setText( mBcStyle.fullName() );

                connect( mMenu, SIGNAL(selectionChanged()), this, SLOT(onMenuSelectionChanged()) );
        }


        ///
        /// bcStyle getter
        ///
        barcode::Style BarcodeMenuButton::bcStyle() const
        {
                return mBcStyle;
        }


        ///
        /// bcStyle setter
        ///
        void BarcodeMenuButton::setBcStyle( const barcode::Style& bcStyle )
        {
                mBcStyle = bcStyle;
                setText( mBcStyle.fullName() );
        }


        ///
        /// onMenuStyleChanged slot
        ///
        void BarcodeMenuButton::onMenuSelectionChanged()
        {
                mBcStyle = mMenu->bcStyle();
                setText( mBcStyle.fullName() );

                emit selectionChanged();
        }

} // namespace glabels
