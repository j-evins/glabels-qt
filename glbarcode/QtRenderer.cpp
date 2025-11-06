/*  QtRenderer.cpp
 *
 *  Copyright (C) 2017  Jaye Evins <evins@snaught.com>
 *
 *  This file is part of glbarcode++.
 *
 *  glbarcode++ is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  glbarcode++ is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with glbarcode++.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "QtRenderer.h"

#include <QFont>
#include <QFontMetrics>
#include <QString>
#include <QTextLayout>
#include <QtDebug>


namespace
{
	const double FONT_SCALE = 0.75;
	const double MIN_POINT_SIZE = 0.4; // Less than ~0.37 causes issues for QFontMetricsF
}


namespace glbarcode
{

	struct QtRenderer::PrivateData
	{
		QPainter* painter;
		QColor    color;
	};


	QtRenderer::QtRenderer()
	{
		d = new QtRenderer::PrivateData;

		d->painter = nullptr;
	}


	QtRenderer::QtRenderer( QPainter* painter )
	{
		d = new QtRenderer::PrivateData;

		setPainter( painter );
	}


	QtRenderer::~QtRenderer()
	{
		delete d;
	}


	QPainter* QtRenderer::painter( ) const
	{
		return d->painter;
	}


	QtRenderer& QtRenderer::setPainter( QPainter* painter )
	{
		d->painter = painter;

		return *this;
	}


	void QtRenderer::drawBegin( double w, double h )
	{
		if ( d->painter )
		{
			d->painter->save();
			d->color = d->painter->pen().color(); // Get current pen color
		}
	}


	void QtRenderer::drawEnd( )
	{
		if ( d->painter )
		{
			d->painter->restore();
		}
	}


	void QtRenderer::drawLine( double x, double y, double w, double h )
	{
		if ( d->painter )
		{
			double x1 = x + w/2; // Offset line origin by 1/2 line width.

			d->painter->setPen( QPen( d->color, w, Qt::SolidLine, Qt::FlatCap ) );
			d->painter->drawLine( QPointF(x1, y), QPointF(x1, y+h) );
		}
	}


	void QtRenderer::drawBox( double x, double y, double w, double h )
	{
		if ( d->painter )
		{
			d->painter->setPen( QPen( Qt::NoPen ) );
			d->painter->setBrush( QBrush( d->color ) );
		
			d->painter->drawRect( QRectF(x, y, w, h) );
		}
	}


	void QtRenderer::drawText( double x, double y, double size, const std::string& text, HAlign halign )
	{
		if ( d->painter )
		{
			d->painter->setPen( QPen( d->color ) );

			QFont font;
			font.setStyleHint( QFont::Monospace );
			font.setFamily( "monospace" );
			font.setPointSizeF( std::max( FONT_SCALE*size, MIN_POINT_SIZE ) );

			QFontMetricsF fm( font );
			double xCorner = x;
			if ( halign == H_ALIGN_CENTER )
			{
				xCorner -= fm.horizontalAdvance( QString::fromStdString(text) )/2.0;
			}
			else if ( halign == H_ALIGN_RIGHT )
			{
				xCorner -= fm.horizontalAdvance( QString::fromStdString(text) );
			}
			double yCorner = y - fm.ascent();
		
			QTextLayout layout( QString::fromStdString(text), font );
			layout.beginLayout();
			layout.createLine();
			layout.endLayout();
			layout.draw( d->painter, QPointF(xCorner, yCorner) );
		}
	}


	void QtRenderer::drawRing( double x, double y, double r, double w )
	{
		if ( d->painter )
		{
			d->painter->setPen( QPen( d->color, w ) );
			d->painter->setBrush( w ? Qt::NoBrush : QBrush( d->color ) );
		
			d->painter->drawEllipse( QPointF(x, y), r, r );
		}
	}


	void QtRenderer::drawHexagon( double x, double y, double h )
	{
		if ( d->painter )
		{
			d->painter->setPen( QPen( Qt::NoPen ) );
			d->painter->setBrush( QBrush( d->color ) );
		
			QPolygonF hexagon;
			hexagon << QPointF( x,           y          )
			        << QPointF( x + 0.433*h, y + 0.25*h )
			        << QPointF( x + 0.433*h, y + 0.75*h )
			        << QPointF( x,           y +      h )
			        << QPointF( x - 0.433*h, y + 0.75*h )
			        << QPointF( x - 0.433*h, y + 0.25*h );

			d->painter->drawPolygon( hexagon );
		}
	}


}
