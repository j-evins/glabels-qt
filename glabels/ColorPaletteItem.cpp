/*  ColorPaletteItem.cpp
 *
 *  Copyright (C) 2014  Jaye Evins <evins@snaught.com>
 *
 *  This file is part of gLabels-qt.
 *
 *  gLabels-qt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  gLabels-qt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ColorPaletteItem.h"

#include <QMouseEvent>
#include <QPainter>


namespace glabels
{

	//
	// Private
	//
	namespace
	{
		const int     border = 4;
		const int     wSwatch = 25;
		const int     hSwatch = 25;

		const int     hoverBgOutlineWidthPixels = 1;

		const int     outlineWidthPixels = 1;
	}


	///
	/// Constructor From Data
	///
	ColorPaletteItem::ColorPaletteItem( int            id,
	                                    const QColor&  color,
	                                    const QString& tip,
	                                    QWidget*       parent )
		: QWidget(parent), mId(id), mColor(color), mTip(tip), mHover(false)
	{
		setSizePolicy( QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed ) );
		setMinimumSize( wSwatch+2*border+1, hSwatch+2*border+1 );
		setToolTip( tip );
	}


	///
	/// Color Property Setter
	///
	void ColorPaletteItem::setColor( int            id,
	                                 const QColor&  color,
	                                 const QString& tip )
	{
		mId    = id;
		mColor = color;
		mTip   = tip;

		setToolTip( tip );
		update();
	}


	///
	/// Paint Event
	///
	void ColorPaletteItem::paintEvent( QPaintEvent* event )
	{
		QPainter painter(this);

		//
		// Draw swatch
		//
		if ( isEnabled() )
		{
			if ( mHover )
			{
				QPen pen( palette().color( QPalette::Text ) );
				pen.setWidth( 2*outlineWidthPixels );
				pen.setJoinStyle( Qt::MiterJoin );
				painter.setPen( pen );
				painter.setBrush( QBrush( mColor ) );
				painter.drawRect( 1, 1, width()-2, height()-2 );
			}
			else
			{
				QPen pen( palette().color( QPalette::Text ) );
				pen.setWidth( outlineWidthPixels );
				painter.setPen( pen );
				painter.setBrush( QBrush( mColor ) );
				painter.drawRect( border, border, wSwatch, hSwatch );
			}

			
		}
		else
		{
			QPen pen( palette().color( QPalette::Disabled, QPalette::Text ) );
			pen.setWidth( outlineWidthPixels );
			painter.setPen( pen );
			painter.setBrush( QBrush( mColor ) );
			painter.drawRect( border, border, wSwatch, hSwatch );
		}

	}


	///
	/// Enter Event
	///
	void ColorPaletteItem::enterEvent( QEnterEvent* event )
	{
		mHover = true;
		update();
	}


	///
	/// Leave Event
	///
	void ColorPaletteItem::leaveEvent( QEvent* event )
	{
		mHover = false;
		update();
	}

	
	///
	/// Mouse Press Event
	///
	void ColorPaletteItem::mousePressEvent( QMouseEvent* event )
	{
		emit activated( mId );
	}

} // namespace glabels
