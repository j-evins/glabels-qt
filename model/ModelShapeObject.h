/*  ModelShapeObject.h
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

#ifndef model_ModelShapeObject_h
#define model_ModelShapeObject_h


#include "ModelObject.h"


namespace glabels
{
	namespace model
	{

		///
		/// Label Model Shape Object (Box or Ellipse)
		///
		class ModelShapeObject : public ModelObject
		{
			Q_OBJECT

			///////////////////////////////////////////////////////////////
			// Lifecycle Methods
			///////////////////////////////////////////////////////////////
		protected:
			ModelShapeObject();

			ModelShapeObject( const Distance&  x0,
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
			                  const ColorNode& shadowColorNode );

			ModelShapeObject( const ModelShapeObject* object );
		public:
			~ModelShapeObject() override;


			///////////////////////////////////////////////////////////////
			// Property Implementations
			///////////////////////////////////////////////////////////////
		public:
			//
			// Shape Property: lineWidth
			//
			Distance lineWidth() const override;
			void setLineWidth( const Distance& value ) override;


			//
			// Shape Property: lineColorNode
			//
			ColorNode lineColorNode() const override;
			void setLineColorNode( const ColorNode& value ) override;
		

			//
			// Shape Property: fillColorNode
			//
			ColorNode fillColorNode() const override;
			void setFillColorNode( const ColorNode& value ) override;
		

			///////////////////////////////////////////////////////////////
			// Capability Implementations
			///////////////////////////////////////////////////////////////
		public:
			bool canFill() const override;
			bool canLineColor() const override;
			bool canLineWidth() const override;


			///////////////////////////////////////////////////////////////
			// Private Members
			///////////////////////////////////////////////////////////////
		protected:
			Distance             mLineWidth;
			ColorNode            mLineColorNode;
			ColorNode            mFillColorNode;

		};

	}
}


#endif // model_ModelShapeObject_h
