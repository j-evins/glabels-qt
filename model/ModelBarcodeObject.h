/*  ModelBarcodeObject.h
 *
 *  Copyright (C) 2017  Jaye Evins <evins@snaught.com>
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

#ifndef model_ModelBarcodeObject_h
#define model_ModelBarcodeObject_h


#include "ModelObject.h"

#include "RawText.h"

#include "glbarcode/Barcode.h"


namespace glabels
{
	namespace model
	{

		///
		/// Label Model Line Object
		///
		class ModelBarcodeObject : public ModelObject
		{
			Q_OBJECT

			///////////////////////////////////////////////////////////////
			// Lifecycle Methods
			///////////////////////////////////////////////////////////////
		public:
			ModelBarcodeObject();

			ModelBarcodeObject( const Distance&       x0,
			                    const Distance&       y0,
			                    const Distance&       w,
			                    const Distance&       h,
			                    bool                  lockAspectRatio,
			                    const barcode::Style& bcStyle,
			                    bool                  bcTextFlag,
			                    bool                  bcChecksumFlag,
			                    QString               bcData,
			                    const ColorNode&      bcColorNode,
			                    const QTransform&     matrix = QTransform() );

			ModelBarcodeObject( const ModelBarcodeObject* object );

			~ModelBarcodeObject() override;


			///////////////////////////////////////////////////////////////
			// Object duplication
			///////////////////////////////////////////////////////////////
			ModelBarcodeObject* clone() const override;


			///////////////////////////////////////////////////////////////
			// Property Implementations
			///////////////////////////////////////////////////////////////
		public:
			//
			// Barcode Property: bcData
			//
			QString bcData() const override;
			void setBcData( const QString &value ) override;
                

			//
			// Barcode Property: bcTextFlag
			//
			bool bcTextFlag() const override;
			void setBcTextFlag( bool value ) override;


			//
			// Barcode Property: bcChecksumFlag
			//
			bool bcChecksumFlag() const override;
			void setBcChecksumFlag( bool value ) override;


			//
			// Barcode Property: bcColorNode
			//
			ColorNode bcColorNode() const override;
			void setBcColorNode( const ColorNode &value ) override;
                

			//
			// Barcode Property: bcStyle
			//
			barcode::Style bcStyle() const override;
			void setBcStyle( const barcode::Style &value ) override;
                

			//
			// Barcode Property: bcFormatDigits
			//
			int bcFormatDigits() const override;
			void setBcFormatDigits( int value ) override;



			///////////////////////////////////////////////////////////////
			// Capability Implementations
			///////////////////////////////////////////////////////////////
		public:


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
			// Private methods
			///////////////////////////////////////////////////////////////
		private:
			void sizeUpdated() override;
			void update();

			void drawBcInEditor( QPainter* painter, const QColor& color ) const;

			void drawBc( QPainter*      painter,
			             const QColor&  color,
			             merge::Record* record,
			             Variables*     variables ) const;

			void drawPlaceHolder( QPainter* painter, const QColor& color, const QString& text ) const;

	

			///////////////////////////////////////////////////////////////
			// Private Members
			///////////////////////////////////////////////////////////////
		private:
			barcode::Style      mBcStyle;
			bool                mBcTextFlag;
			bool                mBcChecksumFlag;
			int                 mBcFormatDigits;
			RawText             mBcData;
			ColorNode           mBcColorNode;

			glbarcode::Barcode* mEditorBarcode;
			glbarcode::Barcode* mEditorDefaultBarcode;
		
			QPainterPath mHoverPath;

		};

	}
}


#endif // model_ModelBarcodeObject_h
