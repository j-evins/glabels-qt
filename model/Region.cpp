//  Region.cpp
//
//  Copyright (C) 2013-2016  Jaye Evins <evins@snaught.com>
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


#include "Region.hpp"


namespace glabels::model
{

        ///
        /// Constructor
        ///
        Region::Region()
                : mX1(0),
                  mY1(0),
                  mX2(0),
                  mY2(0)
        {
                // empty
        }


        ///
        /// Constructor
        ///
        Region::Region( Distance x1,
                        Distance y1,
                        Distance x2,
                        Distance y2 )
                : mX1(x1),
                  mY1(y1),
                  mX2(x2),
                  mY2(y2)
        {
                // empty
        }


        ///
        /// Get x1
        ///
        Distance Region::x1() const
        {
                return mX1;
        }


        ///
        /// Set x1
        ///
        void Region::setX1( Distance value )
        {
                mX1 = value;
        }


        ///
        /// Get y1
        ///
        Distance Region::y1() const
        {
                return mY1;
        }


        ///
        /// Set y1
        ///
        void Region::setY1( Distance value )
        {
                mY1 = value;
        }


        ///
        /// Get x2
        ///
        Distance Region::x2() const
        {
                return mX2;
        }


        ///
        /// Set x2
        ///
        void Region::setX2( Distance value )
        {
                mX2 = value;
        }


        ///
        /// Get y2
        ///
        Distance Region::y2() const
        {
                return mY2;
        }


        ///
        /// Set y2
        ///
        void Region::setY2( Distance value )
        {
                mY2 = value;
        }


        ///
        /// Convert to a QRectF
        ///
        QRectF Region::rect() const
        {
                QRectF r;

                r.setX( min( mX1, mX2 ).pt() );
                r.setY( min( mY1, mY2 ).pt() );
                r.setWidth( fabs( mX2 - mX1 ).pt() );
                r.setHeight( fabs( mY2 - mY1 ).pt() );

                return r;
        }

}
