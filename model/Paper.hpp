//  Paper.hpp
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

#ifndef model_Paper_hpp
#define model_Paper_hpp


#include "Distance.hpp"

#include <QString>


namespace glabels::model
{

        class Paper
        {
        public:
                Paper() = default;
                Paper( const QString& id,
                       const QString& name,
                       Distance       width,
                       Distance       height,
                       const QString& pwgSize );
                ~Paper() = default;

                QString id() const;
                QString name() const;

                /* Width */
                Distance width() const;

                /* Height */
                Distance height() const;

                /* PWG 5101.1-2002 size name */
                QString pwgSize() const;

                bool isSizeIso() const;
                bool isSizeUs() const;

        private:
                QString  mId;
                QString  mName;
                Distance mWidth;
                Distance mHeight;
                QString  mPwgSize;
        };

}


#endif // model_Paper_hpp
