//  Outline.hpp
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

#ifndef model_Outline_hpp
#define model_Outline_hpp


#include <QPainter>
#include <QPainterPath>

#include <memory>


namespace glabels::model
{

        // Forward references
        class ModelObject;


        ///
        /// Outline Class
        ///
        class Outline
        {
                ////////////////////////////
                // Lifecycle Methods
                ////////////////////////////
        public:
                Outline() = default;
                ~Outline() = default;


                ////////////////////////////
                // Ownership
                ////////////////////////////
                void setOwner( ModelObject* owner );
                bool isEnabled() const;


                ////////////////////////////
                // Drawing Methods
                ////////////////////////////
        public:
                void draw( QPainter* painter ) const;
                QPainterPath hoverPath( double scale ) const;


                ////////////////////////////
                // Private Data
                ////////////////////////////
        private:
                ModelObject*   mOwner{ nullptr };

                QVector<qreal> mDashes;
                QPen           mPen1;
                QPen           mPen2;

        };

}


#endif // model_Outline_hpp
