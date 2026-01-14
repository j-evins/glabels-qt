//  Cursors.hpp
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

#ifndef Cursors_hpp
#define Cursors_hpp


#include <QCursor>


namespace glabels
{

        ///
        /// Glabels Cursors
        ///
        namespace Cursors
        {


                class Crosshair : public QCursor
                {
                public:
                        Crosshair();
                };


                class Move : public QCursor
                {
                public:
                        Move();
                };


                class Barcode : public QCursor
                {
                public:
                        Barcode();
                };


                class Box : public QCursor
                {
                public:
                        Box();
                };


                class Ellipse : public QCursor
                {
                public:
                        Ellipse();
                };


                class Image : public QCursor
                {
                public:
                        Image();
                };


                class Line : public QCursor
                {
                public:
                        Line();
                };


                class Text : public QCursor
                {
                public:
                        Text();
                };

        }

}


#endif // Cursors_hpp
