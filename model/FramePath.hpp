//  FramePath.hpp
//
//  Copyright (C) 2018-2026  Jaye Evins <evins@snaught.com>
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

#ifndef model_FramePath_hpp
#define model_FramePath_hpp


#include "Frame.hpp"


namespace glabels::model
{

        class FramePath : public Frame
        {
                Q_DECLARE_TR_FUNCTIONS(FramePath)

        public:
                FramePath( const QPainterPath& path,
                           Distance            xWaste,
                           Distance            yWaste,
                           Units               originalUnits,
                           const QString&      id = "0" );

                FramePath( const FramePath& other ) = default;

                std::unique_ptr<Frame> clone() const override;

                Distance xWaste() const;
                Distance yWaste() const;

                Units        originalUnits() const;

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
                Distance mXWaste;
                Distance mYWaste;

                QPainterPath mPath;
                QPainterPath mClipPath;

                Units mOriginalUnits;
        };

}


#endif // model_FramePath_hpp
