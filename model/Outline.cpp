/*  Outline.cpp
 *
 *  Copyright (C) 2013-2016  Jaye Evins <evins@snaught.com>
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

#include "Outline.h"

#include "ModelObject.h"

#include <QColor>


namespace glabels
{
	namespace model
	{

		//
		// Private
		//
		namespace
		{
			const qreal dashSize = 2;

			const double slopPixels = 2;
			const double outlineWidthPixels = 1;
			const QColor outlineColor1(   0,   0,   0 );
			const QColor outlineColor2( 255, 255, 255 );
		}


		///
		/// Set owner and enable outline
		///
		void Outline::setOwner( ModelObject* owner )
		{
			mOwner = owner;

			mDashes << dashSize << dashSize;

			mPen1.setColor( outlineColor1 );
			mPen1.setWidth( outlineWidthPixels );
			mPen1.setCosmetic( true );
			mPen1.setCapStyle( Qt::FlatCap );
			mPen1.setDashPattern( mDashes );

			mPen2.setColor( outlineColor2 );
			mPen2.setWidth( outlineWidthPixels );
			mPen2.setCosmetic( true );
			mPen2.setCapStyle( Qt::FlatCap );
			mPen2.setDashPattern( mDashes );
			mPen2.setDashOffset( dashSize );
		}


		///
		/// Is enabled?  (I.e. not all objects have an outline (e.g. line objects))
		///
		bool Outline::isEnabled() const
		{
			return mOwner;
		}


		///
		/// Draw Outline
		///
		void Outline::draw( QPainter* painter ) const
		{
			if ( mOwner )
			{
				painter->save();

				painter->setBrush( Qt::NoBrush );

				painter->setPen( mPen1 );
				painter->drawRect( QRectF( 0, 0, mOwner->w().pt(), mOwner->h().pt() ) );

				painter->setPen( mPen2 );
				painter->drawRect( QRectF( 0, 0, mOwner->w().pt(), mOwner->h().pt() ) );

				painter->restore();
			}
		}


		///
		/// Create path for testing for hover condition
		///
		QPainterPath Outline::hoverPath( double scale ) const
		{
			QPainterPath path;

			if ( mOwner )
			{
				double s = 1 / scale;

				path.addRect( -s*slopPixels, -s*slopPixels,
				              mOwner->w().pt()+s*2*slopPixels, mOwner->h().pt()+s*2*slopPixels );
				path.closeSubpath();
				path.addRect( s*slopPixels, s*slopPixels,
				              mOwner->w().pt()-s*2*slopPixels, mOwner->h().pt()-s*2*slopPixels );
			}

			return path;
		}

	}
}
