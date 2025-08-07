/*  Point.h
 *
 *  Copyright (C) 2013-2016  Jaye Evins <evins@snaught.com>
 *
 *  This file is part of gLabels-qt.
 *
 *  gLabels-qt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  gLabels-qt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef model_Point_h
#define model_Point_h


#include "Distance.h"

#include <QMetaType>


namespace glabels
{
	namespace model
	{

		class Point
		{
		public:
			Point();
		
			Point( Distance x, Distance y );

			Distance x() const;
			Distance y() const;

			bool operator<( const Point &other ) const;


		private:
			Distance  mX;
			Distance  mY;
		};

	}
}


Q_DECLARE_METATYPE( glabels::model::Point )


#endif // model_Point_h
