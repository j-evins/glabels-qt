/*  ModelShapeObject.cpp
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

#include "ModelShapeObject.h"

#include <QBrush>
#include <QPen>


namespace glabels
{
	namespace model
	{

		///
		/// Constructor
		///
		ModelShapeObject::ModelShapeObject()
		{
			mOutline = new Outline( this );

			mHandles << new HandleNorthWest( this );
			mHandles << new HandleNorth( this );
			mHandles << new HandleNorthEast( this );
			mHandles << new HandleEast( this );
			mHandles << new HandleSouthEast( this );
			mHandles << new HandleSouth( this );
			mHandles << new HandleSouthWest( this );
			mHandles << new HandleWest( this );

			mLineWidth       = 1.0;
			mLineColorNode   = ColorNode( QColor( 0, 0, 0 ) );
			mFillColorNode   = ColorNode( QColor( 0, 255, 0 ) );
		}


		///
		/// Constructor
		///
		ModelShapeObject::ModelShapeObject( const Distance&  x0,
		                                    const Distance&  y0,
		                                    const Distance&  w,
		                                    const Distance&  h,
		                                    bool             lockAspectRatio,
		                                    const Distance&  lineWidth,
		                                    const ColorNode& lineColorNode,
		                                    const ColorNode& fillColorNode,
		                                    const QTransform& matrix,
		                                    bool             shadowState,
		                                    const Distance&  shadowX,
		                                    const Distance&  shadowY,
		                                    double           shadowOpacity,
		                                    const ColorNode& shadowColorNode )
		: ModelObject( x0, y0, w, h, lockAspectRatio,
		               matrix,
		               shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode )
		{
			mOutline = new Outline( this );

			mHandles << new HandleNorthWest( this );
			mHandles << new HandleNorth( this );
			mHandles << new HandleNorthEast( this );
			mHandles << new HandleEast( this );
			mHandles << new HandleSouthEast( this );
			mHandles << new HandleSouth( this );
			mHandles << new HandleSouthWest( this );
			mHandles << new HandleWest( this );

			mLineWidth       = lineWidth;
			mLineColorNode   = lineColorNode;
			mFillColorNode   = fillColorNode;
		}

	
		///
		/// Copy constructor
		///
		ModelShapeObject::ModelShapeObject( const ModelShapeObject* object ) : ModelObject(object)
		{
			mLineWidth       = object->mLineWidth;
			mLineColorNode   = object->mLineColorNode;
			mFillColorNode   = object->mFillColorNode;
		}


		///
		/// Destructor
		///
		ModelShapeObject::~ModelShapeObject()
		{
			delete mOutline;

			foreach( Handle* handle, mHandles )
			{
				delete handle;
			}
			mHandles.clear();
		}


		///
		/// Line Width Property Getter
		///
		Distance ModelShapeObject::lineWidth() const
		{
			return mLineWidth;
		}


		///
		/// Line Width Property Setter
		///
		void ModelShapeObject::setLineWidth( const Distance& value )
		{
			if ( mLineWidth != value )
			{
				mLineWidth = value;
				emit changed();
			}
		}


		///
		/// Line Color Node Property Getter
		///
		ColorNode ModelShapeObject::lineColorNode() const
		{
			return mLineColorNode;
		}


		///
		/// Line Color Node Property Setter
		///
		void ModelShapeObject::setLineColorNode( const ColorNode& value )
		{
			if ( mLineColorNode != value )
			{
				mLineColorNode = value;
				emit changed();
			}
		}
		

		///
		/// Fill Color Node Property Getter
		///
		ColorNode ModelShapeObject::fillColorNode() const
		{
			return mFillColorNode;
		}


		///
		/// Fill Color Node Property Setter
		///
		void ModelShapeObject::setFillColorNode( const ColorNode& value )
		{
			if ( mFillColorNode != value )
			{
				mFillColorNode = value;
				emit changed();
			}
		}
		

		///
		/// Can Fill Capability Implementation
		///
		bool ModelShapeObject::canFill() const
		{
			return true;
		}


		///
		/// Can Line Color Capability Implementation
		///
		bool ModelShapeObject::canLineColor() const
		{
			return true;
		}


		///
		/// Can Line Width Capability Implementation
		///
		bool ModelShapeObject::canLineWidth() const
		{
			return true;
		}

	}
}
