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
			enum Location { NW, N, NE, E, SE, S, SW, W, P1, P2 };
		
		
			////////////////////////////
			// Lifecycle Methods
			////////////////////////////
		protected:
			Handle( ModelObject* owner, Location location );
		public:
			virtual ~Handle() = default;
			virtual Handle* clone( ModelObject* newOwner ) const = 0;
	
		
			////////////////////////////
			// Attribute Methods
			////////////////////////////
			ModelObject* owner() const;
			Location location() const;
		

			////////////////////////////
			// Drawing Methods
			////////////////////////////
		public:
			virtual void  draw( QPainter* painter, double scale ) const = 0;
			virtual QPainterPath path( double scale ) const = 0;
		protected:
			void drawAt( QPainter*       painter,
			             double          scale,
			             const Distance& x,
			             const Distance& y,
			             QColor          color ) const;
		
			QPainterPath pathAt( double          scale,
			                     const Distance& x,
			                     const Distance& y ) const;


			////////////////////////////
			// Protected Data
			////////////////////////////
		protected:
			ModelObject* mOwner;
			Location mLocation;

		};


		///
		/// HandleNorth Class
		///
		class HandleNorth : public Handle
		{
			////////////////////////////
			// Lifecycle Methods
			////////////////////////////
		public:
			HandleNorth( ModelObject* owner );
			~HandleNorth() = default;
			HandleNorth* clone( ModelObject* newOwner ) const override;


			////////////////////////////
			// Drawing Methods
			////////////////////////////
		public:
			void  draw( QPainter* painter, double scale ) const override;
			QPainterPath path( double scale ) const override;
		};


		///
		/// HandleNorthEast Class
		///
		class HandleNorthEast : public Handle
		{
			////////////////////////////
			// Lifecycle Methods
			////////////////////////////
		public:
			HandleNorthEast( ModelObject* owner );
			~HandleNorthEast() = default;
			HandleNorthEast* clone( ModelObject* newOwner ) const override;


			////////////////////////////
			// Drawing Methods
			////////////////////////////
		public:
			void  draw( QPainter* painter, double scale ) const override;
			QPainterPath path( double scale ) const override;
		};


		///
		/// HandleEast Class
		///
		class HandleEast : public Handle
		{
			////////////////////////////
			// Lifecycle Methods
			////////////////////////////
		public:
			HandleEast( ModelObject* owner );
			~HandleEast() = default;
			HandleEast* clone( ModelObject* newOwner ) const override;


			////////////////////////////
			// Drawing Methods
			////////////////////////////
		public:
			void  draw( QPainter* painter, double scale ) const override;
			QPainterPath path( double scale ) const override;
		};


		///
		/// HandleSouthEast Class
		///
		class HandleSouthEast : public Handle
		{
			////////////////////////////
			// Lifecycle Methods
			////////////////////////////
		public:
			HandleSouthEast( ModelObject* owner );
			~HandleSouthEast() = default;
			HandleSouthEast* clone( ModelObject* newOwner ) const override;


			////////////////////////////
			// Drawing Methods
			////////////////////////////
		public:
			void  draw( QPainter* painter, double scale ) const override;
			QPainterPath path( double scale ) const override;
		};


		///
		/// HandleSouth Class
		///
		class HandleSouth : public Handle
		{
			////////////////////////////
			// Lifecycle Methods
			////////////////////////////
		public:
			HandleSouth( ModelObject* owner );
			~HandleSouth() = default;
			HandleSouth* clone( ModelObject* newOwner ) const override;


			////////////////////////////
			// Drawing Methods
			////////////////////////////
		public:
			void  draw( QPainter* painter, double scale ) const override;
			QPainterPath path( double scale ) const override;
		};


		///
		/// HandleSouthWest Class
		///
		class HandleSouthWest : public Handle
		{
			////////////////////////////
			// Lifecycle Methods
			////////////////////////////
		public:
			HandleSouthWest( ModelObject* owner );
			~HandleSouthWest() = default;
			HandleSouthWest* clone( ModelObject* newOwner ) const override;


			////////////////////////////
			// Drawing Methods
			////////////////////////////
		public:
			void  draw( QPainter* painter, double scale ) const override;
			QPainterPath path( double scale ) const override;
		};


		///
		/// HandleWest Class
		///
		class HandleWest : public Handle
		{
			////////////////////////////
			// Lifecycle Methods
			////////////////////////////
		public:
			HandleWest( ModelObject* owner );
			~HandleWest() = default;
			HandleWest* clone( ModelObject* newOwner ) const override;


			////////////////////////////
			// Drawing Methods
			////////////////////////////
		public:
			void  draw( QPainter* painter, double scale ) const override;
			QPainterPath path( double scale ) const override;
		};


		///
		/// HandleNorthWest Class
		///
		class HandleNorthWest : public Handle
		{
			////////////////////////////
			// Lifecycle Methods
			////////////////////////////
		public:
			HandleNorthWest( ModelObject* owner );
			~HandleNorthWest() = default;
			HandleNorthWest* clone( ModelObject* newOwner ) const override;


			////////////////////////////
			// Drawing Methods
			////////////////////////////
		public:
			void  draw( QPainter* painter, double scale ) const override;
			QPainterPath path( double scale ) const override;
		};


		///
		/// HandleP1 Class
		///
		class HandleP1 : public Handle
		{
			////////////////////////////
			// Lifecycle Methods
			////////////////////////////
		public:
			HandleP1( ModelObject* owner );
			~HandleP1() = default;
			HandleP1* clone( ModelObject* newOwner ) const override;


			////////////////////////////
			// Drawing Methods
			////////////////////////////
		public:
			void  draw( QPainter* painter, double scale ) const override;
			QPainterPath path( double scale ) const override;
		};


		///
		/// HandleP2 Class
		///
		class HandleP2 : public Handle
		{
			////////////////////////////
			// Lifecycle Methods
			////////////////////////////
		public:
			HandleP2( ModelObject* owner );
			~HandleP2() = default;
			HandleP2* clone( ModelObject* newOwner ) const override;


			////////////////////////////
			// Drawing Methods
			////////////////////////////
		public:
			void  draw( QPainter* painter, double scale ) const override;
			QPainterPath path( double scale ) const override;
		};

	}
}


#endif // model_Handles_h
