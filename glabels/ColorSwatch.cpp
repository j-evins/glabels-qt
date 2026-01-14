//  ColorSwatch.cpp
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


#include "ColorSwatch.hpp"

#include <QPainter>


//
// Private
//
namespace
{
        const QColor  outlineColor( 0, 0, 0 );
        const double  outlineWidthPixels = 1;
}


namespace glabels
{

        ///
        /// Constructor
        ///
        ColorSwatch::ColorSwatch( int w, int h, const QColor& color )
                : QPixmap( w, h )
        {
                fill( Qt::transparent );

                QPainter painter(this );

                painter.setBackgroundMode( Qt::TransparentMode );

                QBrush brush( color );
                QPen pen( outlineColor );
                pen.setWidth( outlineWidthPixels );

                painter.setBrush( brush );
                painter.setPen( pen );
                painter.drawRect( 1, 1, w-2, h-2 );
        }

} // namespace glabels
