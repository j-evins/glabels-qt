//  ModelBoxObject.hpp
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

#ifndef model_ModelBoxObject_hpp
#define model_ModelBoxObject_hpp


#include "ModelShapeObject.hpp"


namespace glabels::model
{

        ///
        /// Label Model Box Object
        ///
        class ModelBoxObject : public ModelShapeObject
        {
                Q_OBJECT

                ///////////////////////////////////////////////////////////////
                // Lifecycle Methods
                ///////////////////////////////////////////////////////////////
        public:
                ModelBoxObject() = default;

                ModelBoxObject( Distance          x0,
                                Distance          y0,
                                Distance          w,
                                Distance          h,
                                bool              lockAspectRatio,
                                Distance          lineWidth,
                                const ColorNode&  lineColorNode,
                                const ColorNode&  fillColorNode,
                                const QTransform& matrix = QTransform(),
                                bool              shadowState = false,
                                Distance          shadowX = 0,
                                Distance          shadowY = 0,
                                double            shadowOpacity = 1.0,
                                const ColorNode&  shadowColorNode = ColorNode() );

                ModelBoxObject( const ModelBoxObject* object );

                virtual ~ModelBoxObject() = default;


                ///////////////////////////////////////////////////////////////
                // Object duplication
                ///////////////////////////////////////////////////////////////
                ModelBoxObject* clone() const override;


                ///////////////////////////////////////////////////////////////
                // Drawing operations
                ///////////////////////////////////////////////////////////////
        protected:
                void drawShadow( QPainter*            painter,
                                 bool                 inEditor,
                                 const merge::Record& record,
                                 const Variables&     variables ) const override;

                void drawObject( QPainter*            painter,
                                 bool                 inEditor,
                                 const merge::Record& record,
                                 const Variables&     variables ) const override;

                QPainterPath hoverPath( double scale ) const override;

        };

}


#endif // model_ModelBoxObject_hpp
