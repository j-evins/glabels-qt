//  FrameRect.cpp
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


#include "FrameRect.hpp"

#include "Constants.hpp"
#include "StrUtil.hpp"

#include <QDebug>


namespace glabels::model
{

        FrameRect::FrameRect( Distance       w,
                              Distance       h,
                              Distance       r,
                              Distance       xWaste,
                              Distance       yWaste,
                              const QString& id )
                : Frame(id),
                  mW(w),
                  mH(h),
                  mR(r),
                  mXWaste(xWaste),
                  mYWaste(yWaste)
        {
                mPath.addRoundedRect( 0, 0, mW.pt(), mH.pt(), mR.pt(), mR.pt() );

                mClipPath.addRoundedRect( -mXWaste.pt(), -mYWaste.pt(),
                                          mW.pt() + 2*mXWaste.pt(), mH.pt() + 2*mYWaste.pt(),
                                          mR.pt(), mR.pt() );
        }


        std::unique_ptr<Frame> FrameRect::clone() const
        {
                return std::make_unique<FrameRect>( *this );
        }


        Distance FrameRect::w() const
        {
                return mW;
        }


        Distance FrameRect::h() const
        {
                return mH;
        }


        Distance FrameRect::r() const
        {
                return mR;
        }


        Distance FrameRect::xWaste() const
        {
                return mXWaste;
        }


        Distance FrameRect::yWaste() const
        {
                return mYWaste;
        }


        QString FrameRect::sizeDescription( Units units ) const
        {
                if ( units.toEnum() == Units::IN )
                {
                        QString wStr = StrUtil::formatFraction( mW.in() );
                        QString hStr = StrUtil::formatFraction( mH.in() );

                        return QString("%1 x %2 %3").arg(wStr).arg(hStr).arg(units.toTrName());
                }
                else
                {
                        return QString("%1 x %2 %3").arg(mW.inUnits(units), 0, 'g', 5)
                                .arg(mH.inUnits(units), 0, 'g', 5)
                                .arg(units.toTrName());
                }
        }


        bool FrameRect::isSimilarTo( const Frame& other ) const
        {
                if ( auto *otherRect = dynamic_cast<const FrameRect*>(&other) )
                {
                        if ( (fabs( mW - otherRect->mW ) <= EPSILON) &&
                             (fabs( mH - otherRect->mH ) <= EPSILON) )
                        {
                                return true;
                        }
                }
                return false;
        }


        const QPainterPath& FrameRect::path() const
        {
                return mPath;
        }


        const QPainterPath& FrameRect::clipPath() const
        {
                return mClipPath;
        }


        QPainterPath FrameRect::marginPath( Distance xSize, Distance ySize ) const
        {
                Distance w = mW - 2*xSize;
                Distance h = mH - 2*ySize;
                Distance r = std::max( mR - std::min(xSize, ySize), Distance(0.0) );

                QPainterPath path;
                path.addRoundedRect( xSize.pt(), ySize.pt(), w.pt(), h.pt(), r.pt(), r.pt() );

                return path;
        }


        // Debugging support
        void FrameRect::print( QDebug& dbg ) const
        {
                dbg.nospace() << "FrameRect{ "
                              << id() << ","
                              << w() << ","
                              << h() << ","
                              << r() << ","
                              << xWaste() << ","
                              << yWaste() << ","
                              << "list{ ";
                for ( auto& layout : layouts() )
                {
                        dbg.nospace() << layout << ",";
                }
                dbg.nospace() << " }"
                              << "list{ ";
                for ( auto& markup : markups() )
                {
                        dbg.nospace() << *markup << ",";
                }
                dbg.nospace() << " }"
                              << " }";
        }


}
