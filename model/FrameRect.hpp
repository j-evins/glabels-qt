//  FrameRect.hpp
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

#ifndef model_FrameRect_hpp
#define model_FrameRect_hpp


#include "Frame.hpp"


namespace glabels::model
{

        class FrameRect : public Frame
        {
                Q_DECLARE_TR_FUNCTIONS(FrameRect)

        public:
                FrameRect( Distance       w,
                           Distance       h,
                           Distance       r,
                           Distance       xWaste,
                           Distance       yWaste,
                           const QString& id = "0" );

                FrameRect( const FrameRect& other ) = default;

                std::unique_ptr<Frame> clone() const override;

                Distance r() const;
                Distance xWaste() const;
                Distance yWaste() const;

                Distance w() const override;
                Distance h() const override;

                QString sizeDescription( Units units ) const override;

                bool isSimilarTo( const Frame& other ) const override;

                const QPainterPath& path() const override;
                const QPainterPath& clipPath() const override;
                QPainterPath marginPath( Distance xSize, Distance ySize ) const override;

                // Debugging support
                void print( QDebug& dbg ) const override;


        private:
                Distance mW;
                Distance mH;
                Distance mR;
                Distance mXWaste;
                Distance mYWaste;

                QPainterPath mPath;
                QPainterPath mClipPath;

        };

}


#endif // model_FrameRect_hpp
