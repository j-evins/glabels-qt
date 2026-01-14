//  Handles.cpp
//
//  Copyright (C) 2013-2026  Jaye Evins <evins@snaught.com>
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


#include "Handle.hpp"

#include "ModelObject.hpp"

#include <QColor>
#include <QDebug>


//
// Private
//
namespace
{
        const double handlePixels             = 7;
        const double handleOutlineWidthPixels = 1;

        const QColor handleFillColor( 0,  192,  0,  96 );
        const QColor originHandleFillColor( 192,  0,  0,  96 );
        const QColor handleOutlineColor( 0,  0,   0,  192 );
}


namespace glabels::model
{

        ///
        /// Handle Constructor
        ///
        Handle::Handle( ModelObject* owner, Location location )
                : mOwner(owner), mLocation(location)
        {
                // empty
        }


        ///
        /// Is Handle null?
        ///
        bool Handle::isNull() const
        {
                return mLocation == NULL_HANDLE;
        }


        ///
        /// Handle owner
        ///
        ModelObject* Handle::owner() const
        {
                return mOwner;
        }


        ///
        /// Handle location
        ///
        Handle::Location Handle::location() const
        {
                return mLocation;
        }


        ///
        /// Draw Handle
        ///
        void Handle::draw( QPainter* painter, double scale ) const
        {
                switch ( mLocation )
                {
                case NW:
                        drawAt( painter, scale, 0, 0, originHandleFillColor );
                        break;
                case N:
                        drawAt( painter, scale, mOwner->w()/2, 0, handleFillColor );
                        break;
                case NE:
                        drawAt( painter, scale, mOwner->w(), 0, handleFillColor );
                        break;
                case E:
                        drawAt( painter, scale, mOwner->w(), mOwner->h()/2, handleFillColor );
                        break;
                case SE:
                        drawAt( painter, scale, mOwner->w(), mOwner->h(), handleFillColor );
                        break;
                case S:
                        drawAt( painter, scale, mOwner->w()/2, mOwner->h(), handleFillColor );
                        break;
                case SW:
                        drawAt( painter, scale, 0, mOwner->h(), handleFillColor );
                        break;
                case W:
                        drawAt( painter, scale, 0, mOwner->h()/2, handleFillColor );
                        break;
                case P1:
                        drawAt( painter, scale, 0, 0, originHandleFillColor );
                        break;
                case P2:
                        drawAt( painter, scale, mOwner->w(), mOwner->h(), handleFillColor );
                        break;
                default:
                        qWarning() << "Unknown Handle location";
                        break;
                }
        }


        ///
        /// Handle Path
        ///
        QPainterPath Handle::path( double scale ) const
        {
                switch ( mLocation )
                {
                case NW:
                        return pathAt( scale, 0, 0 );
                        break;
                case N:
                        return pathAt( scale, mOwner->w()/2, 0 );
                        break;
                case NE:
                        return pathAt( scale, mOwner->w(), 0 );
                        break;
                case E:
                        return pathAt( scale, mOwner->w(), mOwner->h()/2 );
                        break;
                case SE:
                        return pathAt( scale, mOwner->w(), mOwner->h() );
                        break;
                case S:
                        return pathAt( scale, mOwner->w()/2, mOwner->h() );
                        break;
                case SW:
                        return pathAt( scale, 0, mOwner->h() );
                        break;
                case W:
                        return pathAt( scale, 0, mOwner->h()/2 );
                        break;
                case P1:
                        return pathAt( scale, 0, 0 );
                        break;
                case P2:
                        return pathAt( scale, mOwner->w(), mOwner->h() );
                        break;
                default:
                        qWarning() << "Unknown Handle location";
                        return QPainterPath(); // Empty
                        break;
                }
        }


        ///
        /// Draw Handle at x,y
        ///
        void Handle::drawAt( QPainter* painter,
                             double    scale,
                             Distance  x,
                             Distance  y,
                             QColor    color ) const
        {
                painter->save();

                painter->translate( x.pt(), y.pt() );

                double s = 1.0 / scale;

                QPen pen( handleOutlineColor );
                pen.setCosmetic( true );
                pen.setWidth( handleOutlineWidthPixels );

                painter->setPen( pen );
                painter->setBrush( color );

                painter->drawRect( QRectF( -s*handlePixels/2.0, -s*handlePixels/2.0,
                                           s*handlePixels, s*handlePixels ) );

                painter->restore();
        }


        ///
        /// Create Handle path at x,y
        ///
        QPainterPath Handle::pathAt( double   scale,
                                     Distance x,
                                     Distance y ) const
        {
                QPainterPath path;

                double s = 1/scale;

                path.addRect( -s*handlePixels/2, -s*handlePixels/2, s*handlePixels, s*handlePixels );
                path.translate( x.pt(), y.pt() );

                return path;
        }


}
