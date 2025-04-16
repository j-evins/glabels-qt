/*  ModelLineObject.h
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

#ifndef model_ModelLineObject_h
#define model_ModelLineObject_h


#include "ModelObject.h"


namespace glabels
{
	namespace model
	{

		///
		/// Label Model Line Object
		///
		class ModelLineObject : public ModelObject
		{
			Q_OBJECT

			///////////////////////////////////////////////////////////////
			// Lifecycle Methods
			///////////////////////////////////////////////////////////////
		public:
			ModelLineObject();

			ModelLineObject( const Distance&  x0,
			                 const Distance&  y0,
			                 const Distance&  w,
			                 const Distance&  h,
			                 const Distance&  lineWidth,
			                 const ColorNode& lineColorNode,
			                 const QTransform& matrix = QTransform(),
			                 bool             shadowState = false,
			                 const Distance&  shadowX = 0,
			                 const Distance&  shadowY = 0,
			                 double           shadowOpacity = 1.0,
			                 const ColorNode& shadowColorNode = ColorNode() );

			ModelLineObject( const ModelLineObject* object );
		
			~ModelLineObject() override;


			///////////////////////////////////////////////////////////////
			// Object duplication
			///////////////////////////////////////////////////////////////
			ModelLineObject* clone() const override;


			///////////////////////////////////////////////////////////////
			// Property Implementations
			///////////////////////////////////////////////////////////////
		public:
			//
			// Line Property: lineWidth
			//
			Distance lineWidth() const override;
			void setLineWidth( const Distance& value ) override;


			//
			// Line Property: lineColorNode
			//
			ColorNode lineColorNode() const override;
			void setLineColorNode( const ColorNode& value ) override;
		

			///////////////////////////////////////////////////////////////
			// Capability Implementations
			///////////////////////////////////////////////////////////////
		public:
			bool canLineColor() const override;
			bool canLineWidth() const override;


			///////////////////////////////////////////////////////////////
			// Drawing operations
			///////////////////////////////////////////////////////////////
		protected:
			void drawShadow( QPainter*      painter,
			                 bool           inEditor,
			                 merge::Record* record,
			                 Variables*     variables ) const override;
			
			void drawObject( QPainter*      painter,
			                 bool           inEditor,
			                 merge::Record* record,
			                 Variables*     variables ) const override;

			QPainterPath hoverPath( double scale ) const override;


			///////////////////////////////////////////////////////////////
			// Private Members
			///////////////////////////////////////////////////////////////
		protected:
			Distance    mLineWidth;
			ColorNode   mLineColorNode;

		};

	}
}


#endif // model_ModelLineObject_h
