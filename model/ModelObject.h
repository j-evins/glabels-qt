/*  ModelObject.h
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

#ifndef model_ModelObject_h
#define model_ModelObject_h


#include "ColorNode.h"
#include "Distance.h"
#include "Handles.h"
#include "Outline.h"
#include "TextNode.h"
#include "Variables.h"

#include "barcode/Style.h"
#include "merge/Record.h"

#include <QObject>
#include <QFont>
#include <QTransform>
#include <QPainter>


namespace glabels
{
	namespace model
	{

		// Forward References
		class Region;
		class Size;


		///
		/// Label Model Object Base Class
		///
		class ModelObject : public QObject
		{
			Q_OBJECT

			///////////////////////////////////////////////////////////////
			// Lifecycle Methods
			///////////////////////////////////////////////////////////////
		protected:
			ModelObject();
		
			ModelObject( const Distance&  x0,
			             const Distance&  y0,
			             const Distance&  w,
			             const Distance&  h,
			             bool             lockAspectRatio = false,
			             const QTransform& matrix = QTransform(),
			             bool             shadowState = false,
			             const Distance&  shadowX = 0,
			             const Distance&  shadowY = 0,
			             double           shadowOpacity = 1.0,
			             const ColorNode& shadowColorNode = ColorNode() );
		
			ModelObject( const ModelObject* object );
		
		public:
			~ModelObject() override;


			///////////////////////////////////////////////////////////////
			// Object duplication
			///////////////////////////////////////////////////////////////
			virtual ModelObject* clone() const = 0;

	
			///////////////////////////////////////////////////////////////
			// Signals
			///////////////////////////////////////////////////////////////
		signals:
			void moved();
			void changed();


			///////////////////////////////////////////////////////////////
			// Common Properties
			///////////////////////////////////////////////////////////////
		public:
			//
			// ID Property.
			//
			int id() const;

			//
			// Selected Property.
			//
			bool isSelected() const;
			void select( bool value = true );
			void unselect();


			//
			// x0 Property ( x coordinate of origin )
			//
			Distance x0() const;
			void setX0( const Distance& value );
		

			//
			// y0 Property ( y coordinate of origin )
			//
			Distance y0() const;
			void setY0( const Distance& value );
		

			//
			// w Property ( width of bounding box )
			//
			Distance w() const;
			void setW( const Distance& value );
		

			//
			// h Property ( height of bounding box )
			//
			Distance h() const;
			void setH( const Distance& value );


			//
			// Lock Aspect Ratio Property
			//
			bool lockAspectRatio() const;
			void setLockAspectRatio( bool value );


			//
			// Transformation Matrix Property
			//
			QTransform matrix() const;
			void setMatrix( const QTransform& value );


			//
			// Shadow State Property
			//
			bool shadow() const;
			void setShadow( bool value );


			//
			// Shadow x Offset Property
			//
			Distance shadowX() const;
			void setShadowX( const Distance& value );
		

			//
			// Shadow y Offset Property
			//
			Distance shadowY() const;
			void setShadowY( const Distance& value );
		

			//
			// Shadow opacity Property
			//
			double shadowOpacity() const;
			void setShadowOpacity( double value );
		

			//
			// Shadow Color Property
			//
			ColorNode shadowColorNode() const;
			void setShadowColorNode( const ColorNode& value );
		

			//
			// Natural Size Property (read-only)
			//
			virtual Size naturalSize() const;


			///////////////////////////////////////////////////////////////
			// Text Properties Virtual Interface
			///////////////////////////////////////////////////////////////
		public:
			//
			// Virtual Text Property: text
			//
			virtual QString text() const;
			virtual void setText( const QString &value );


			//
			// Virtual Text Property: fontFamily
			//
			virtual QString fontFamily() const;
			virtual void setFontFamily( const QString &value );


			//
			// Virtual Text Property: fontSize
			//
			virtual double fontSize() const;
			virtual void setFontSize( double value );


			//
			// Virtual Text Property: fontWeight
			//
			virtual QFont::Weight fontWeight() const;
			virtual void setFontWeight( QFont::Weight value );


			//
			// Virtual Text Property: fontItalicFlag
			//
			virtual bool fontItalicFlag() const;
			virtual void setFontItalicFlag( bool value );


			//
			// Virtual Text Property: fontUnderlineFlag
			//
			virtual bool fontUnderlineFlag() const;
			virtual void setFontUnderlineFlag( bool value );


			//
			// Virtual Text Property: textColorNode
			//
			virtual ColorNode textColorNode() const;
			virtual void setTextColorNode( const ColorNode &value );
		

			//
			// Virtual Text Property: textHAlign
			//
			virtual Qt::Alignment textHAlign() const;
			virtual void setTextHAlign( Qt::Alignment value );


			//
			// Virtual Text Property: textVAlign
			//
			virtual Qt::Alignment textVAlign() const;
			virtual void setTextVAlign( Qt::Alignment value );


			//
			// Virtual Text Property: textWrapMode
			//
			virtual QTextOption::WrapMode textWrapMode() const;
			virtual void setTextWrapMode( QTextOption::WrapMode value );


			//
			// Virtual Text Property: textLineSpacing
			//
			virtual double textLineSpacing() const;
			virtual void setTextLineSpacing( double value );


			//
			// Virtual Text Property: textAutoShrink
			//
			virtual bool textAutoShrink() const;
			virtual void setTextAutoShrink( bool value );


			///////////////////////////////////////////////////////////////
			// Image Properties Virtual Interface
			///////////////////////////////////////////////////////////////
		public:
			//
			// Virtual Image Property: filenameNode
			//
			virtual TextNode filenameNode() const;
			virtual void setFilenameNode( const TextNode &value );


			//
			// Virtual Image Property: image
			//
			virtual const QImage* image() const;
			virtual void setImage( const QImage& value );
			virtual void setImage( const QString& name, const QImage& value );


			//
			// Virtual Image Property: svg
			//
			virtual QByteArray svg() const;
			virtual void setSvg( const QString& name, const QByteArray& value );


			///////////////////////////////////////////////////////////////
			// Shape Properties Virtual Interface
			///////////////////////////////////////////////////////////////
		public:
			//
			// Virtual Shape Property: lineWidth
			//
			virtual Distance lineWidth() const;
			virtual void setLineWidth( const Distance& value );


			//
			// Virtual Shape Property: lineColorNode
			//
			virtual ColorNode lineColorNode() const;
			virtual void setLineColorNode( const ColorNode &value );
		

			//
			// Virtual Shape Property: fillColorNode
			//
			virtual ColorNode fillColorNode() const;
			virtual void setFillColorNode( const ColorNode &value );
		

			///////////////////////////////////////////////////////////////
			// Barcode Properties Virtual Interface
			///////////////////////////////////////////////////////////////
		public:
			//
			// Virtual Barcode Property: bcData
			//
			virtual QString bcData() const;
			virtual void setBcData( const QString& value );
		

			//
			// Virtual Barcode Property: bcTextFlag
			//
			virtual bool bcTextFlag() const;
			virtual void setBcTextFlag( bool value );


			//
			// Virtual Barcode Property: bcChecksumFlag
			//
			virtual bool bcChecksumFlag() const;
			virtual void setBcChecksumFlag( bool value );


			//
			// Virtual Barcode Property: bcColorNode
			//
			virtual ColorNode bcColorNode() const;
			virtual void setBcColorNode( const ColorNode &value );
		

			//
			// Virtual Barcode Property: bcStyle
			//
			virtual barcode::Style bcStyle() const;
			virtual void setBcStyle( const barcode::Style &value );
		

			//
			// Virtual Barcode Property: bcFormatDigits
			//
			virtual int bcFormatDigits() const;
			virtual void setBcFormatDigits( int value );


			///////////////////////////////////////////////////////////////
			// Capabilities (Overridden by concrete classes.)
			///////////////////////////////////////////////////////////////
		public:
			virtual bool canText() const;
			virtual bool canFill() const;
			virtual bool canLineColor() const;
			virtual bool canLineWidth() const;


			///////////////////////////////////////////////////////////////
			// Position and Size methods
			///////////////////////////////////////////////////////////////
		public:
			void setPosition( const Distance& x0, const Distance& y0 );
			void setPositionRelative( const Distance& dx, const Distance& dy );
			Size size() const;
			void setSize( const Distance& w, const Distance& h );
			void setSize( const Size& size );
			void setSizeHonorAspect( const Distance& w, const Distance& h );
			void setWHonorAspect( const Distance& w );
			void setHHonorAspect( const Distance& h );
			Region getExtent();
			void rotate( double thetaDegs );
			void flipHoriz();
			void flipVert();
			bool isLocatedAt( double scale, const Distance& x, const Distance& y ) const;
			Handle* handleAt( double scale, const Distance& x, const Distance& y ) const;


			///////////////////////////////////////////////////////////////
			// Drawing operations
			///////////////////////////////////////////////////////////////
		public:
			void draw( QPainter*      painter,
			           bool           inEditor,
			           merge::Record* record,
			           Variables*     variables ) const;
			
			void drawSelectionHighlight( QPainter* painter, double scale ) const;

		protected:
			virtual void drawShadow( QPainter*      painter,
			                         bool           inEditor,
			                         merge::Record* record,
			                         Variables*     variables ) const = 0;
			
			virtual void drawObject( QPainter*      painter,
			                         bool           inEditor,
			                         merge::Record* record,
			                         Variables*     variables ) const = 0;
			
			virtual QPainterPath hoverPath( double scale ) const = 0;

			virtual void sizeUpdated();

		
			///////////////////////////////////////////////////////////////
			// Protected Members
			///////////////////////////////////////////////////////////////
		protected:
			bool              mSelectedFlag;

			Distance          mX0;
			Distance          mY0;
			Distance          mW;
			Distance          mH;
			bool              mLockAspectRatio;

			bool              mShadowState;
			Distance          mShadowX;
			Distance          mShadowY;
			double            mShadowOpacity;
			ColorNode         mShadowColorNode;

			QList<Handle*>    mHandles;
			Outline*          mOutline;


			///////////////////////////////////////////////////////////////
			// Private Members
			///////////////////////////////////////////////////////////////
		private:
			static int msNextId;
			int        mId;

			QTransform mMatrix;

		};

	}
}


#endif // model_ModelObject_h
