/*  ModelLineObject.cpp
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

#include "ModelLineObject.h"

#include <QBrush>
#include <QPen>


namespace glabels
{
	namespace model
	{

		//
		// Private
		//
		namespace
		{
			const double slopPixels = 2;
		}


		///
		/// Constructor
		///
		ModelLineObject::ModelLineObject()
		{
			mOutline = nullptr;

			mHandles << new HandleP1( this );
			mHandles << new HandleP2( this );

			mLineWidth       = 1.0;
			mLineColorNode   = ColorNode( QColor( 0, 0, 0 ) );
		}


		///
		/// Constructor
		///
		ModelLineObject::ModelLineObject( const Distance&  x0,
		                                  const Distance&  y0,
		                                  const Distance&  dx,
		                                  const Distance&  dy,
		                                  const Distance&  lineWidth,
		                                  const ColorNode& lineColorNode,
		                                  const QTransform& matrix,
		                                  bool             shadowState,
		                                  const Distance&  shadowX,
		                                  const Distance&  shadowY,
		                                  double           shadowOpacity,
		                                  const ColorNode& shadowColorNode )
		: ModelObject( x0, y0, dx, dy, false /*lockAspectRatio*/,
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
		}

	
		///
		/// Copy constructor
		///
		ModelLineObject::ModelLineObject( const ModelLineObject* object )
			: ModelObject(object)
		{
			mLineWidth       = object->mLineWidth;
			mLineColorNode   = object->mLineColorNode;
		}


		///
		/// Destructor
		///
		ModelLineObject::~ModelLineObject()
		{
			foreach( Handle* handle, mHandles )
			{
				delete handle;
			}
			mHandles.clear();
		}


		///
		/// Clone
		///
		ModelLineObject* ModelLineObject::clone() const
		{
			return new ModelLineObject( this );
		}


		///
		/// Line Width Property Getter
		///
		Distance ModelLineObject::lineWidth() const
		{
			return mLineWidth;
		}


		///
		/// Line Width Property Setter
		///
		void ModelLineObject::setLineWidth( const Distance& value )
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
		ColorNode ModelLineObject::lineColorNode() const
		{
			return mLineColorNode;
		}


		///
		/// Line Color Node Property Setter
		///
		void ModelLineObject::setLineColorNode( const ColorNode& value )
		{
			if ( mLineColorNode != value )
			{
				mLineColorNode = value;
				emit changed();
			}
		}
		

		///
		/// Can Line Color Capability Implementation
		///
		bool ModelLineObject::canLineColor() const
		{
			return true;
		}


		///
		/// Can Line Width Capability Implementation
		///
		bool ModelLineObject::canLineWidth() const
		{
			return true;
		}


		///
		/// Draw shadow of object
		///
		void ModelLineObject::drawShadow( QPainter*      painter,
		                                  bool           inEditor,
		                                  merge::Record* record,
		                                  Variables*     variables ) const
		{
			QColor lineColor = mLineColorNode.color( record, variables );
			QColor shadowColor = mShadowColorNode.color( record, variables );

			shadowColor.setAlphaF( mShadowOpacity );

			if ( lineColor.alpha() )
			{
				painter->setPen( QPen( shadowColor, mLineWidth.pt() ) );
				painter->drawLine( 0, 0, mW.pt(), mH.pt() );
			}
		}

	
		///
		/// Draw object itself
		///
		void ModelLineObject::drawObject( QPainter*      painter,
		                                  bool           inEditor,
		                                  merge::Record* record,
		                                  Variables*     variables ) const
		{
			QColor lineColor = mLineColorNode.color( record, variables );

			painter->setPen( QPen( lineColor, mLineWidth.pt() ) );
			painter->drawLine( 0, 0, mW.pt(), mH.pt() );
		}


		///
		/// Path to test for hover condition
		///
		QPainterPath ModelLineObject::hoverPath( double scale ) const
		{
			QPainterPath path;

			if ( mLineColorNode.color().alpha() )
			{
				//
				// Build a thin rectangle representing line
				//
				double rPts = mLineWidth.pt()/2 + slopPixels / scale;

				double lengthPts = sqrt( mW.pt()*mW.pt() + mH.pt()*mH.pt() );
				double dx = mH.pt() / lengthPts; // horizontal pitch of perpendicular line
				double dy = mW.pt() / lengthPts; // vertical pitch of perpendicular line
	
				path.moveTo(           rPts*dx,         - rPts*dy );
				path.lineTo( mW.pt() + rPts*dx, mH.pt() - rPts*dy );
				path.lineTo( mW.pt() - rPts*dx, mH.pt() + rPts*dy );
				path.lineTo(         - rPts*dx,           rPts*dy );
		
				path.closeSubpath();
			}

			return path;
		}

	}
}
