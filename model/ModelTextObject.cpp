/*  ModelTextObject.cpp
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

#include "ModelTextObject.h"

#include "Size.h"

#include <QBrush>
#include <QPen>
#include <QTextDocument>
#include <QTextBlock>
#include <QRegularExpression>
#include <QtDebug>


namespace glabels
{
	namespace model
	{

		//
		// Private
		//
		namespace
		{
			const double marginPts = 3;
		}


		///
		/// Constructor
		///
		ModelTextObject::ModelTextObject()
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

			mText              = "";
			mFontFamily        = "Sans";
			mFontSize          = 10;
			mFontWeight        = QFont::Normal;
			mFontItalicFlag    = false;
			mFontUnderlineFlag = false;
			mTextColorNode     = ColorNode( QColor( 0, 0, 0 ) );
			mTextHAlign        = Qt::AlignLeft;
			mTextVAlign        = Qt::AlignTop;
			mTextWrapMode      = QTextOption::WordWrap;
			mTextLineSpacing   = 1;
			mTextAutoShrink    = false;
		}


		///
		/// Constructor
		///
		ModelTextObject::ModelTextObject( const Distance&       x0,
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
		                                  const QTransform&     matrix,
		                                  bool                  shadowState,
		                                  const Distance&       shadowX,
		                                  const Distance&       shadowY,
		                                  double                shadowOpacity,
		                                  const ColorNode&      shadowColorNode )
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

			mText              = text;
			mFontFamily        = fontFamily;
			mFontSize          = fontSize;
			mFontWeight        = fontWeight;
			mFontItalicFlag    = fontItalicFlag;
			mFontUnderlineFlag = fontUnderlineFlag;
			mTextColorNode     = textColorNode;
			mTextHAlign        = textHAlign;
			mTextVAlign        = textVAlign;
			mTextWrapMode      = textWrapMode;
			mTextLineSpacing   = textLineSpacing;
			mTextAutoShrink    = textAutoShrink;

			update(); // Initialize cached editor layouts
		}

	
		///
		/// Copy constructor
		///
		ModelTextObject::ModelTextObject( const ModelTextObject* object )
			: ModelObject(object)
		{
			mText              = object->mText;
			mFontFamily        = object->mFontFamily;
			mFontSize          = object->mFontSize;
			mFontWeight        = object->mFontWeight;
			mFontItalicFlag    = object->mFontItalicFlag;
			mFontUnderlineFlag = object->mFontUnderlineFlag;
			mTextColorNode     = object->mTextColorNode;
			mTextHAlign        = object->mTextHAlign;
			mTextVAlign        = object->mTextVAlign;
			mTextWrapMode      = object->mTextWrapMode;
			mTextLineSpacing   = object->mTextLineSpacing;
			mTextAutoShrink    = object->mTextAutoShrink;

			update(); // Initialize cached editor layouts
		}


		///
		/// Destructor
		///
		ModelTextObject::~ModelTextObject()
		{
			delete mOutline;

			foreach( Handle* handle, mHandles )
			{
				delete handle;
			}
			mHandles.clear();
		}


		///
		/// Clone
		///
		ModelTextObject* ModelTextObject::clone() const
		{
			return new ModelTextObject( this );
		}


		///
		/// Text Property Getter
		///
		QString ModelTextObject::text() const
		{
			return mText.toString();
		}


		///
		/// Text Property Setter
		///
		void ModelTextObject::setText( const QString& value )
		{
			if ( mText.toString() != value )
			{
				mText = value;
				update();
				emit changed();
			}
		}


		///
		/// FontFamily Property Getter
		///
		QString ModelTextObject::fontFamily() const
		{
			return mFontFamily;
		}


		///
		/// FontFamily Property Setter
		///
		void ModelTextObject::setFontFamily( const QString& value )
		{
			if ( mFontFamily != value )
			{
				mFontFamily = value;
				update();
				emit changed();
			}
		}


		///
		/// FontSize Property Getter
		///
		double ModelTextObject::fontSize() const
		{
			return mFontSize;
		}


		///
		/// FontSize Property Setter
		///
		void ModelTextObject::setFontSize( double value )
		{
			if ( mFontSize != value )
			{
				mFontSize = value;
				update();
				emit changed();
			}
		}


		///
		/// FontWeight Property Getter
		///
		QFont::Weight ModelTextObject::fontWeight() const
		{
			return mFontWeight;
		}


		///
		/// FontWeight Property Setter
		///
		void ModelTextObject::setFontWeight( QFont::Weight value )
		{
			if ( mFontWeight != value )
			{
				mFontWeight = value;
				update();
				emit changed();
			}
		}


		///
		/// FontItalicFlag Property Getter
		///
		bool ModelTextObject::fontItalicFlag() const
		{
			return mFontItalicFlag;
		}


		///
		/// FontItalicFlag Property Setter
		///
		void ModelTextObject::setFontItalicFlag( bool value )
		{
			if ( mFontItalicFlag != value )
			{
				mFontItalicFlag = value;
				update();
				emit changed();
			}
		}


		///
		/// FontUnderlineFlag Property Getter
		///
		bool ModelTextObject::fontUnderlineFlag() const
		{
			return mFontUnderlineFlag;
		}


		///
		/// FontUnderlineFlag Property Setter
		///
		void ModelTextObject::setFontUnderlineFlag( bool value )
		{
			if ( mFontUnderlineFlag != value )
			{
				mFontUnderlineFlag = value;
				update();
				emit changed();
			}
		}


		///
		/// Text Color Node Property Getter
		///
		ColorNode ModelTextObject::textColorNode() const
		{
			return mTextColorNode;
		}


		///
		/// Text Color Node Property Setter
		///
		void ModelTextObject::setTextColorNode( const ColorNode& value )
		{
			if ( mTextColorNode != value )
			{
				mTextColorNode = value;
				update();
				emit changed();
			}
		}
		

		///
		/// TextHAlign Property Getter
		///
		Qt::Alignment ModelTextObject::textHAlign() const
		{
			return mTextHAlign;
		}


		///
		/// TextHAlign Property Setter
		///
		void ModelTextObject::setTextHAlign( Qt::Alignment value )
		{
			if ( mTextHAlign != value )
			{
				mTextHAlign = value;
				update();
				emit changed();
			}
		}


		///
		/// TextVAlign Property Getter
		///
		Qt::Alignment ModelTextObject::textVAlign() const
		{
			return mTextVAlign;
		}


		///
		/// TextVAlign Property Setter
		///
		void ModelTextObject::setTextVAlign( Qt::Alignment value )
		{
			if ( mTextVAlign != value )
			{
				mTextVAlign = value;
				update();
				emit changed();
			}
		}


		///
		/// Text Wrap Mode Property Getter
		///
		QTextOption::WrapMode ModelTextObject::textWrapMode() const
		{
			return mTextWrapMode;
		}


		///
		/// Text Wrap Mode Property Setter
		///
		void ModelTextObject::setTextWrapMode( QTextOption::WrapMode value )
		{
			if ( mTextWrapMode != value )
			{
				mTextWrapMode = value;
				update();
				emit changed();
			}
		}


		///
		/// TextLineSpacing Property Getter
		///
		double ModelTextObject::textLineSpacing() const
		{
			return mTextLineSpacing;
		}


		///
		/// TextLineSpacing Property Setter
		///
		void ModelTextObject::setTextLineSpacing( double value )
		{
			if ( mTextLineSpacing != value )
			{
				mTextLineSpacing = value;
				update();
				emit changed();
			}
		}


		///
		/// TextAutoShrink Property Getter
		///
		bool ModelTextObject::textAutoShrink() const
		{
			return mTextAutoShrink;
		}


		///
		/// TextAutoShrink Property Setter
		///
		void ModelTextObject::setTextAutoShrink( bool value )
		{
			if ( mTextAutoShrink != value )
			{
				mTextAutoShrink = value;
				update();
				emit changed();
			}
		}


		///
		/// NaturalSize Property Getter
		///
		Size ModelTextObject::naturalSize() const
		{
			QFont font;
			font.setFamily( mFontFamily );
			font.setPointSizeF( mFontSize );
			font.setWeight( mFontWeight );
			font.setItalic( mFontItalicFlag );
			font.setUnderline( mFontUnderlineFlag );

			QTextOption textOption;
			textOption.setAlignment( mTextHAlign );
			textOption.setWrapMode( mTextWrapMode );

			QFontMetricsF fontMetrics( font );
			double dy = fontMetrics.lineSpacing() * mTextLineSpacing;

			QString displayText = mText.isEmpty() ? tr("Text") : mText.toString();
			QTextDocument document( displayText );

			// Do layouts
			double x = 0;
			double y = 0;
			QRectF boundingRect;
			for ( int i = 0; i < document.blockCount(); i++ )
			{
				QTextLayout* layout = new QTextLayout( document.findBlockByNumber(i).text() );
		
				layout->setFont( font );
				layout->setTextOption( textOption );
				layout->setCacheEnabled(true);

				layout->beginLayout();
				for ( QTextLine l = layout->createLine(); l.isValid(); l = layout->createLine() )
				{
					l.setPosition( QPointF( x, y ) );
					y += dy;
				}
				layout->endLayout();

				boundingRect = layout->boundingRect().united( boundingRect );
			}

			return Size( boundingRect.width() + 2*marginPts, boundingRect.height() + 2*marginPts );
		}


		///
		/// Can Text Capability Implementation
		///
		bool ModelTextObject::canText() const
		{
			return true;
		}


		///
		/// Draw shadow of object
		///
		void ModelTextObject::drawShadow( QPainter*      painter,
		                                  bool           inEditor,
		                                  merge::Record* record,
		                                  Variables*     variables ) const
		{
			QColor textColor = mTextColorNode.color( record, variables );

			if ( textColor.alpha() )
			{
				QColor shadowColor = mShadowColorNode.color( record, variables );
				shadowColor.setAlphaF( mShadowOpacity );

				if ( inEditor )
				{
					drawTextInEditor( painter, shadowColor );
				}
				else
				{
					drawText( painter, shadowColor, record, variables );
				}
			}
		}

	
		///
		/// Draw object itself
		///
		void ModelTextObject::drawObject( QPainter*      painter,
		                                  bool           inEditor,
		                                  merge::Record* record,
		                                  Variables*     variables ) const
		{
			QColor textColor = mTextColorNode.color( record, variables );

			if ( inEditor )
			{
				drawTextInEditor( painter, textColor );
			}
			else
			{
				drawText( painter, textColor, record, variables );
			}
		}


		///
		/// Path to test for hover condition
		///
		QPainterPath ModelTextObject::hoverPath( double scale ) const
		{
			return mHoverPath;
		}


		///
		/// Size updated
		///
		void ModelTextObject::sizeUpdated()
		{
			update();
		}


		///
		/// Update cached information for editor view
		///
		void ModelTextObject::update()
		{
			QFont font;
			font.setFamily( mFontFamily );
			font.setPointSizeF( mFontSize );
			font.setWeight( mFontWeight );
			font.setItalic( mFontItalicFlag );
			font.setUnderline( mFontUnderlineFlag );

			QTextOption textOption;
			textOption.setAlignment( mTextHAlign );
			textOption.setWrapMode( mTextWrapMode );

			QFontMetricsF fontMetrics( font );
			double dy = fontMetrics.lineSpacing() * mTextLineSpacing;

			QString displayText = mText.isEmpty() ? tr("Text") : mText.toString();
			QTextDocument document( displayText );

			qDeleteAll( mEditorLayouts );
			mEditorLayouts.clear();

			// Pass #1 -- do initial layouts
			double x = 0;
			double y = 0;
			QRectF boundingRect;
			for ( int i = 0; i < document.blockCount(); i++ )
			{
				QTextLayout* layout = new QTextLayout( document.findBlockByNumber(i).text() );
		
				layout->setFont( font );
				layout->setTextOption( textOption );
				layout->setCacheEnabled(true);

				layout->beginLayout();
				for ( QTextLine l = layout->createLine(); l.isValid(); l = layout->createLine() )
				{
					l.setLineWidth( mW.pt() - 2*marginPts );
					l.setPosition( QPointF( x, y ) );
					y += dy;
				}
				layout->endLayout();

				mEditorLayouts.append( layout );

				boundingRect = layout->boundingRect().united( boundingRect );
			}
			double h = boundingRect.height();


			// Pass #2 -- adjust layout positions for vertical alignment and create hover path
			x = marginPts;
			switch ( mTextVAlign )
			{
			case Qt::AlignVCenter:
				y = mH.pt()/2 - h/2;
				break;
			case Qt::AlignBottom:
				y = mH.pt() - h - marginPts;
				break;
			default:
				y = marginPts;
				break;
			}
			QPainterPath hoverPath; // new empty hover path
			foreach ( QTextLayout* layout, mEditorLayouts )
			{
				for ( int j = 0; j < layout->lineCount(); j++ )
				{
					QTextLine l = layout->lineAt(j);
					l.setPosition( QPointF( x, y ) );
					y += dy;

					hoverPath.addRect( l.naturalTextRect() ); // add to new hover path
				}
			}
	
			mHoverPath = hoverPath; // save new hover path
		}


		///
		/// Draw text in editor from cached information
		///
		void ModelTextObject::drawTextInEditor( QPainter* painter, const QColor& color ) const
		{
			painter->save();

			painter->setClipRect( QRectF( 0, 0, mW.pt(), mH.pt() ) );
			
			if ( mText.isEmpty() )
			{
				QColor mutedColor = color;
				mutedColor.setAlphaF( 0.5 * color.alphaF() );
				painter->setPen( QPen( mutedColor ) );
			}
			else
			{
				painter->setPen( QPen( color ) );
			}

			foreach ( QTextLayout* layout, mEditorLayouts )
			{
				layout->draw( painter, QPointF( 0, 0 ) );
			}

			painter->restore();
		}


		///
		/// Draw text in final printout or preview
		///
		void
		ModelTextObject::drawText( QPainter*      painter,
		                           const QColor&  color,
		                           merge::Record* record,
		                           Variables*     variables ) const
		{
			painter->save();

			painter->setClipRect( QRectF( 0, 0, mW.pt(), mH.pt() ) );
			
			QFont font;
			font.setFamily( mFontFamily );
			font.setPointSizeF( mTextAutoShrink ? autoShrinkFontSize( record, variables ) : mFontSize );
			font.setWeight( mFontWeight );
			font.setItalic( mFontItalicFlag );
			font.setUnderline( mFontUnderlineFlag );

			QTextOption textOption;
			textOption.setAlignment( mTextHAlign );
			textOption.setWrapMode( mTextWrapMode );

			QFontMetricsF fontMetrics( font );
			double dy = fontMetrics.lineSpacing() * mTextLineSpacing;

			QTextDocument document( mText.expand( record, variables ) );

			QList<QTextLayout*> layouts;

			// Pass #1 -- do initial layouts
			double x = 0;
			double y = 0;
			QRectF boundingRect;
			for ( int i = 0; i < document.blockCount(); i++ )
			{
				QTextLayout* layout = new QTextLayout( document.findBlockByNumber(i).text() );
		
				layout->setFont( font );
				layout->setTextOption( textOption );
				layout->setCacheEnabled(true);

				layout->beginLayout();
				for ( QTextLine l = layout->createLine(); l.isValid(); l = layout->createLine() )
				{
					l.setLineWidth( mW.pt() - 2*marginPts );
					l.setPosition( QPointF( x, y ) );
					y += dy;
				}
				layout->endLayout();

				layouts.append( layout );

				boundingRect = layout->boundingRect().united( boundingRect );
			}
			double h = boundingRect.height();


			// Pass #2 -- adjust layout positions for vertical alignment
			x = marginPts;
			switch ( mTextVAlign )
			{
			case Qt::AlignVCenter:
				y = mH.pt()/2 - h/2;
				break;
			case Qt::AlignBottom:
				y = mH.pt() - h - marginPts;
				break;
			default:
				y = marginPts;
				break;
			}
			foreach ( QTextLayout* layout, layouts )
			{
				for ( int j = 0; j < layout->lineCount(); j++ )
				{
					QTextLine l = layout->lineAt(j);
					l.setPosition( QPointF( x, y ) );
					y += dy;
				}
			}

			// Draw layouts
			painter->setPen( QPen( color ) );
			foreach ( QTextLayout* layout, layouts )
			{
				layout->draw( painter, QPointF( 0, 0 ) );
			}

			// Cleanup
			qDeleteAll( layouts );

			painter->restore();
		}


		///
		/// Determine auto shrink font size
		///
		double
		ModelTextObject::autoShrinkFontSize( merge::Record* record, Variables* variables ) const
		{
			QFont font;
			font.setFamily( mFontFamily );
			font.setWeight( mFontWeight );
			font.setItalic( mFontItalicFlag );
			font.setUnderline( mFontUnderlineFlag );

			QTextOption textOption;
			textOption.setAlignment( mTextHAlign );
			textOption.setWrapMode( mTextWrapMode );

			QTextDocument document( mText.expand( record, variables ) );

			double candidateSize = mFontSize;
			while ( candidateSize > 1.0 )
			{
				font.setPointSizeF( candidateSize );

				// Line spacing is affected by font size
				QFontMetricsF fontMetrics( font );
				double dy = fontMetrics.lineSpacing() * mTextLineSpacing;

				// Do candidate layouts, letting text flow according to wrap mode
				double x = 0;
				double y = 0;
				QRectF layoutsRect;
				for ( int i = 0; i < document.blockCount(); i++ )
				{
					QTextLayout layout( document.findBlockByNumber(i).text() );
		
					layout.setFont( font );
					layout.setTextOption( textOption );
					layout.setCacheEnabled(true);

					layout.beginLayout();
					for ( QTextLine l = layout.createLine(); l.isValid(); l = layout.createLine() )
					{
						l.setLineWidth( mW.pt() - 2*marginPts );
						l.setPosition( QPointF( x, y ) );
						y += dy;
					}
					layout.endLayout();

					layoutsRect = layout.boundingRect().united( layoutsRect );
				}

				// Did this candidate fit in our object's bounding box?
				if ( ( (layoutsRect.width() + 2*marginPts) <= mW.pt() ) &&
				     ( (layoutsRect.height() + 2*marginPts) <= mH.pt() ) )
				{
					break;
				}

				// If not, let's try a slightly smaller font size
				candidateSize -= 0.5;
			}

			return candidateSize;
		}


	}
}
