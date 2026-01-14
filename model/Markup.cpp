//  Markup.cpp
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


#include "Markup.hpp"

#include "Frame.hpp"


namespace glabels::model
{

        QPainterPath Markup::path( const Frame& frame ) const
        {
                // Use cached path -- default does not depend on frame size
                return mPath;
        }


        MarkupMargin::MarkupMargin( Distance size )
                : mXSize(size),
                  mYSize(size)
        {
        }


        MarkupMargin::MarkupMargin( Distance xSize,
                                    Distance ySize )
                : mXSize(xSize),
                  mYSize(ySize)
        {
        }


        QPainterPath MarkupMargin::path( const Frame& frame ) const
        {
                // Re-calculate path -- frame size may have changed
                return frame.marginPath( mXSize, mYSize );
        }


        std::unique_ptr<Markup> MarkupMargin::clone() const
        {
                return std::make_unique<MarkupMargin>( mXSize, mYSize );
        }


        Distance MarkupMargin::xSize() const
        {
                return mXSize;
        }


        Distance MarkupMargin::ySize() const
        {
                return mYSize;
        }


        void MarkupMargin::print( QDebug& dbg ) const
        {
                dbg.nospace() << "MarkupMargin{ "
                              << mXSize << "," << mYSize
                              << " }";
        }


        MarkupLine::MarkupLine( Distance x1,
                                Distance y1,
                                Distance x2,
                                Distance y2 )
                : mX1(x1),
                  mY1(y1),
                  mX2(x2),
                  mY2(y2)
        {
                mPath.moveTo( x1.pt(), y1.pt() );
                mPath.lineTo( x2.pt(), y2.pt() );
        }


        std::unique_ptr<Markup> MarkupLine::clone() const
        {
                return std::make_unique<MarkupLine>( mX1, mY1, mX2, mY2 );
        }


        Distance MarkupLine::x1() const
        {
                return mX1;
        }


        Distance MarkupLine::y1() const
        {
                return mY1;
        }


        Distance MarkupLine::x2() const
        {
                return mX2;
        }


        Distance MarkupLine::y2() const
        {
                return mY2;
        }


        void MarkupLine::print( QDebug& dbg ) const
        {
                dbg.nospace() << "MarkupLine{ "
                              << mX1 << "," << mY1 << ","
                              << mX2 << "," << mY2
                              << " }";
        }


        MarkupRect::MarkupRect( Distance x1,
                                Distance y1,
                                Distance w,
                                Distance h,
                                Distance r )
                : mX1(x1),
                  mY1(y1),
                  mW(w),
                  mH(h),
                  mR(r)
        {
                mPath.addRoundedRect( x1.pt(), y1.pt(), w.pt(), h.pt(), r.pt(), r.pt() );
        }


        std::unique_ptr<Markup> MarkupRect::clone() const
        {
                return std::make_unique<MarkupRect>( mX1, mY1, mW, mH, mR );
        }


        Distance MarkupRect::x1() const
        {
                return mX1;
        }


        Distance MarkupRect::y1() const
        {
                return mY1;
        }


        Distance MarkupRect::w() const
        {
                return mW;
        }


        Distance MarkupRect::h() const
        {
                return mH;
        }


        Distance MarkupRect::r() const
        {
                return mR;
        }


        void MarkupRect::print( QDebug& dbg ) const
        {
                dbg.nospace() << "MarkupRect{ "
                              << mX1 << "," << mY1 << ","
                              << mW << "," << mH << ","
                              << mR
                              << " }";
        }


        MarkupEllipse::MarkupEllipse( Distance x1,
                                      Distance y1,
                                      Distance w,
                                      Distance h )
                : mX1(x1),
                  mY1(y1),
                  mW(w),
                  mH(h)
        {
                mPath.addEllipse( x1.pt(), y1.pt(), w.pt(), h.pt() );
        }


        std::unique_ptr<Markup> MarkupEllipse::clone() const
        {
                return std::make_unique<MarkupEllipse>( mX1, mY1, mW, mH );
        }


        Distance MarkupEllipse::x1() const
        {
                return mX1;
        }


        Distance MarkupEllipse::y1() const
        {
                return mY1;
        }


        Distance MarkupEllipse::w() const
        {
                return mW;
        }


        Distance MarkupEllipse::h() const
        {
                return mH;
        }


        void MarkupEllipse::print( QDebug& dbg ) const
        {
                dbg.nospace() << "MarkupEllipse{ "
                              << mX1 << "," << mY1 << ","
                              << mW << "," << mH
                              << " }";
        }


        MarkupCircle::MarkupCircle( Distance x0,
                                    Distance y0,
                                    Distance r )
                : mX0(x0),
                  mY0(y0),
                  mR(r)
        {
                mPath.addEllipse( (x0-r).pt(), (y0-r).pt(), 2*r.pt(), 2*r.pt() );
        }

        std::unique_ptr<Markup> MarkupCircle::clone() const
        {
                return std::make_unique<MarkupCircle>( mX0, mY0, mR );
        }


        Distance MarkupCircle::x0() const
        {
                return mX0;
        }


        Distance MarkupCircle::y0() const
        {
                return mY0;
        }


        Distance MarkupCircle::r() const
        {
                return mR;
        }


        void MarkupCircle::print( QDebug& dbg ) const
        {
                dbg.nospace() << "MarkupCircle{ "
                              << mX0 << "," << mY0 << ","
                              << mR
                              << " }";
        }

}


QDebug operator<<( QDebug dbg, const glabels::model::Markup& markup )
{
        QDebugStateSaver saver(dbg);

        markup.print( dbg );

        return dbg;
}

