//  FrameEllipse.cpp
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


#include "FrameEllipse.hpp"

#include "Constants.hpp"
#include "StrUtil.hpp"

#include <QDebug>


namespace glabels::model
{

        FrameEllipse::FrameEllipse( Distance       w,
                                    Distance       h,
                                    Distance       waste,
                                    const QString& id )
                : Frame(id),
                  mW(w),
                  mH(h),
                  mWaste(waste)
        {
                mPath.addEllipse( 0, 0, mW.pt(), mH.pt() );
                mClipPath.addEllipse( -mWaste.pt(), -mWaste.pt(), (mW+2*mWaste).pt(), (mH+2*mWaste).pt() );
        }


        std::unique_ptr<Frame> FrameEllipse::clone() const
        {
                return std::make_unique<FrameEllipse>( *this );
        }


        Distance FrameEllipse::w() const
        {
                return mW;
        }


        Distance FrameEllipse::h() const
        {
                return mH;
        }


        Distance FrameEllipse::waste() const
        {
                return mWaste;
        }


        QString FrameEllipse::sizeDescription( Units units ) const
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


        bool FrameEllipse::isSimilarTo( const Frame& other ) const
        {
                if ( auto* otherEllipse = dynamic_cast<const FrameEllipse*>(&other) )
                {
                        if ( (fabs( mW - otherEllipse->mW ) <= EPSILON) &&
                             (fabs( mH - otherEllipse->mH ) <= EPSILON) )
                        {
                                return true;
                        }
                }
                return false;
        }


        const QPainterPath& FrameEllipse::path() const
        {
                return mPath;
        }


        const QPainterPath& FrameEllipse::clipPath() const
        {
                return mClipPath;
        }


        QPainterPath FrameEllipse::marginPath( Distance xSize, Distance ySize ) const
        {
                // Note: ignore ySize, assume xSize == ySize
                Distance size = xSize;

                Distance w = mW - 2*size;
                Distance h = mH - 2*size;

                QPainterPath path;
                path.addEllipse( size.pt(), size.pt(), w.pt(), h.pt() );

                return path;
        }


        // Debugging support
        void FrameEllipse::print( QDebug& dbg ) const
        {
                dbg.nospace() << "FrameEllipse{ "
                              << id() << ","
                              << w() << ","
                              << h() << ","
                              << waste() << ","
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
