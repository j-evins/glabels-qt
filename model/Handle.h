/*  Handles.h
 *
 *  Copyright (C) 2013  Jaye Evins <evins@snaught.com>
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

#ifndef model_Handles_h
#define model_Handles_h


#include "Distance.h"

#include <QPainter>
#include <QPainterPath>


namespace glabels
{
	namespace model
	{

		// Forward References
		class ModelObject;


		///
		/// Handle Base Class
		///
		class Handle
		{
			////////////////////////////
			// Location enumeration
			////////////////////////////
		public:
			enum Location { NULL_HANDLE, NW, N, NE, E, SE, S, SW, W, P1, P2 };
		
		
			////////////////////////////
			// Lifecycle Methods
			////////////////////////////
		public:
			Handle() = default;
			Handle( ModelObject* owner, Location location );
			~Handle() = default;

		
			////////////////////////////
			// Attribute Methods
			////////////////////////////
			bool isNull() const;
			ModelObject* owner() const;
			Location location() const;
		

			////////////////////////////
			// Drawing Methods
			////////////////////////////
		public:
			void draw( QPainter* painter, double scale ) const;
			QPainterPath path( double scale ) const;

		private:
			void drawAt( QPainter* painter,
			             double    scale,
			             Distance  x,
			             Distance  y,
			             QColor    color ) const;
		
			QPainterPath pathAt( double   scale,
			                     Distance x,
			                     Distance y ) const;


			////////////////////////////
			// Private Data
			////////////////////////////
		protected:
			ModelObject* mOwner{ nullptr };
			Location     mLocation{ NULL_HANDLE };

		};

	}
}


#endif // model_Handles_h
