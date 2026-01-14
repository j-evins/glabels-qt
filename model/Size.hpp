//  Size.hpp
//
//  Copyright (C) 2017-2026  Jaye Evins <evins@snaught.com>
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

#ifndef model_Size_hpp
#define model_Size_hpp


#include "Distance.hpp"

#include <QSizeF>


namespace glabels::model
{

        ///
        /// Size Type
        ///
        class Size
        {

                /////////////////////////////////
                // Constructors
                /////////////////////////////////
        public:
                Size() = default;
                Size( Distance w, Distance h );


                /////////////////////////////////
                // Properties
                /////////////////////////////////
        public:
                //
                // w Property
                //
                Distance w() const;
                void setW( Distance value );


                //
                // H Property
                //
                Distance h() const;
                void setH( Distance value );


                /////////////////////////////////
                // Methods
                /////////////////////////////////
        public:
                QSizeF qSizeF() const;


                /////////////////////////////////
                // Private Data
                /////////////////////////////////
        private:
                Distance mW{ 0 };
                Distance mH{ 0 };

        };

}


#endif // model_Size_hpp
