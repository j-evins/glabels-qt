/*  ModelTextObject.h
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

#ifndef model_ModelTextObject_h
#define model_ModelTextObject_h


#include "ModelObject.h"
#include "RawText.h"

#include <QTextLayout>


namespace glabels
{
	namespace model
	{

		///
		/// Label Model Line Object
		///
		class ModelTextObject : public ModelObject
		{
			Q_OBJECT

			///////////////////////////////////////////////////////////////
			// Lifecycle Methods
			///////////////////////////////////////////////////////////////
		public:
			ModelTextObject();
		
			ModelTextObject( const Distance&       x0,
			                 const Distance&       y0,
			                 const Distance&       w,
			                 const Distance&       h,
			                 bool                  lockAspectRatio,
			                 const QString&        text,
			                 const QString&        fontFamily,
			                 double                fontSize,
			                 QFont::Weight         fontWeight,
			                 bool                  fontItalicFlag,
			                 bool                  fontUnderlineFlag,
			                 ColorNode             textColorNode,
			                 Qt::Alignment         textHAlign,
			                 Qt::Alignment         textVAlign,
			                 QTextOption::WrapMode textWrapMode,
			                 double                textLineSpacing,
			                 bool                  textAutoShrink,
			                 const QTransform&     matrix = QTransform(),
			                 bool                  shadowState = false,
			                 const Distance&       shadowX = 0,
			                 const Distance&       shadowY = 0,
			                 double                shadowOpacity = 1.0,
			                 const ColorNode&      shadowColorNode = ColorNode() );

			ModelTextObject( const ModelTextObject* object );
		
			~ModelTextObject() override;


			///////////////////////////////////////////////////////////////
			// Object duplication
			///////////////////////////////////////////////////////////////
			ModelTextObject* clone() const override;


			///////////////////////////////////////////////////////////////
			// Property Implementations
			///////////////////////////////////////////////////////////////
		public:
			//
			// Text Property: text
			//
			QString text() const override;
			void setText( const QString &value ) override;


			//
			// Text Property: fontFamily
			//
			QString fontFamily() const override;
			void setFontFamily( const QString &value ) override;


			//
			// Text Property: fontSize
			//
			double fontSize() const override;
			void setFontSize( double value ) override;


			//
			// Text Property: fontWeight
			//
			QFont::Weight fontWeight() const override;
			void setFontWeight( QFont::Weight value ) override;


			//
			// Text Property: fontItalicFlag
			//
			bool fontItalicFlag() const override;
			void setFontItalicFlag( bool value ) override;


			//
			// Text Property: fontUnderlineFlag
			//
			bool fontUnderlineFlag() const override;
			void setFontUnderlineFlag( bool value ) override;


			//
			// Text Property: textColorNode
			//
			ColorNode textColorNode() const override;
			void setTextColorNode( const ColorNode &value ) override;
		

			//
			// Text Property: textHAlign
			//
			Qt::Alignment textHAlign() const override;
			void setTextHAlign( Qt::Alignment value ) override;


			//
			// Text Property: textVAlign
			//
			Qt::Alignment textVAlign() const override;
			void setTextVAlign( Qt::Alignment value ) override;


			//
			// Text Property: textWrapMode
			//
			QTextOption::WrapMode textWrapMode() const override;
			void setTextWrapMode( QTextOption::WrapMode value ) override;


			//
			// Text Property: textLineSpacing
			//
			double textLineSpacing() const override;
			void setTextLineSpacing( double value ) override;


			//
			// Text Property: textAutoShrink
			//
			bool textAutoShrink() const override;
			void setTextAutoShrink( bool value ) override;


			//
			// Property: naturalSize
			//
			Size naturalSize() const override;


			///////////////////////////////////////////////////////////////
			// Capability Implementations
			///////////////////////////////////////////////////////////////
		public:
			bool canText() const override;


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
			
			void drawTextInEditor( QPainter*     painter,
			                       const QColor& color ) const;
			
			void drawText( QPainter*      painter,
			               const QColor&  color,
			               merge::Record* record,
			               Variables*     variables ) const;
			
			double autoShrinkFontSize( merge::Record* record,
			                           Variables*     variables ) const;
	

			///////////////////////////////////////////////////////////////
			// Private Members
			///////////////////////////////////////////////////////////////
		private:
			RawText               mText;
			QString               mFontFamily;
			double                mFontSize;
			QFont::Weight         mFontWeight;
			bool                  mFontItalicFlag;
			bool                  mFontUnderlineFlag;
			ColorNode             mTextColorNode;
			Qt::Alignment         mTextHAlign;
			Qt::Alignment         mTextVAlign;
			QTextOption::WrapMode mTextWrapMode;
			double                mTextLineSpacing;
			bool                  mTextAutoShrink;

			QList<QTextLayout*>   mEditorLayouts;
			QPainterPath          mHoverPath;

		};

	}
}


#endif // model_ModelTextObject_h
