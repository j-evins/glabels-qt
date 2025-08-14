/*  ModelBarcodeObject.cpp
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

#include "ModelBarcodeObject.h"

#include "Size.h"

#include "barcode/Backends.h"

#include "glbarcode/Factory.h"
#include "glbarcode/QtRenderer.h"

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
			const QColor fillColor = QColor( 224, 224, 224, 255 );
			const Distance pad = Distance::pt(4);
			const Distance minW = Distance::pt(18);
			const Distance minH = Distance::pt(18);
			const double MIN_POINT_SIZE = 0.4; // Less than ~0.37 causes issues for QFontMetricsF
		}


		///
		/// Constructor
		///
		ModelBarcodeObject::ModelBarcodeObject()
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

			mBcStyle        = barcode::Backends::defaultStyle();
			mBcTextFlag     = mBcStyle.canText();
			mBcChecksumFlag = mBcStyle.canChecksum();
			mBcFormatDigits = mBcStyle.preferedN();
			mBcData         = "";
			mBcColorNode    = ColorNode( Qt::black );

			mEditorBarcode = nullptr;
			mEditorDefaultBarcode = nullptr;

			update(); // Initialize cached editor layouts
		}


		///
		/// Constructor
		///
		ModelBarcodeObject::ModelBarcodeObject( const Distance&       x0,
		                                        const Distance&       y0,
		                                        const Distance&       w,
		                                        const Distance&       h,
		                                        bool                  lockAspectRatio,
		                                        const barcode::Style& bcStyle,
		                                        bool                  bcTextFlag,
		                                        bool                  bcChecksumFlag,
		                                        QString               bcData,
		                                        const ColorNode&      bcColorNode,
		                                        const QTransform&     matrix )
		: ModelObject( x0, y0, w, h, lockAspectRatio, matrix )
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

			mBcStyle        = bcStyle;
			mBcTextFlag     = bcTextFlag;
			mBcChecksumFlag = bcChecksumFlag;
			mBcFormatDigits = mBcStyle.preferedN();
			mBcData         = bcData;
			mBcColorNode    = bcColorNode;

			mEditorBarcode = nullptr;
			mEditorDefaultBarcode = nullptr;

			update(); // Initialize cached editor layouts
		}
	

		///
		/// Copy constructor
		///
		ModelBarcodeObject::ModelBarcodeObject( const ModelBarcodeObject* object )
			: ModelObject(object)
		{
			mBcStyle        = object->mBcStyle;
			mBcTextFlag     = object->mBcTextFlag;
			mBcChecksumFlag = object->mBcChecksumFlag;
			mBcFormatDigits = object->mBcFormatDigits;
			mBcData         = object->mBcData;
			mBcColorNode    = object->mBcColorNode;

			mEditorBarcode = nullptr;
			mEditorDefaultBarcode = nullptr;

			update(); // Initialize cached editor layouts
		}


		///
		/// Destructor
		///
		ModelBarcodeObject::~ModelBarcodeObject()
		{
			delete mOutline;

			foreach( Handle* handle, mHandles )
			{
				delete handle;
			}
			mHandles.clear();

			delete mEditorBarcode;
		}


		///
		/// Clone
		///
		ModelBarcodeObject* ModelBarcodeObject::clone() const
		{
			return new ModelBarcodeObject( this );
		}


		///
		/// bcData Property Getter
		///
		QString ModelBarcodeObject::bcData() const
		{
			return mBcData.toString();
		}


		///
		/// bcData Property Setter
		///
		void ModelBarcodeObject::setBcData( const QString& value )
		{
			if ( mBcData.toString() != value )
			{
				mBcData = value;
				update();
				emit changed();
			}
		}


		///
		/// bcTextFlag Property Getter
		///
		bool ModelBarcodeObject::bcTextFlag() const
		{
			return mBcTextFlag;
		}


		///
		/// bcTextFlag Property Setter
		///
		void ModelBarcodeObject::setBcTextFlag( bool value )
		{
			if ( mBcTextFlag != value )
			{
				mBcTextFlag = value;
				update();
				emit changed();
			}
		}


		///
		/// bcChecksumFlag Property Getter
		///
		bool ModelBarcodeObject::bcChecksumFlag() const
		{
			return mBcChecksumFlag;
		}


		///
		/// bcChecksumFlag Property Setter
		///
		void ModelBarcodeObject::setBcChecksumFlag( bool value )
		{
			if ( mBcChecksumFlag != value )
			{
				mBcChecksumFlag = value;
				update();
				emit changed();
			}
		}


		///
		/// Barcode Color Node Property Getter
		///
		ColorNode ModelBarcodeObject::bcColorNode() const
		{
			return mBcColorNode;
		}


		///
		/// Barcode Color Node Property Setter
		///
		void ModelBarcodeObject::setBcColorNode( const ColorNode& value )
		{
			if ( mBcColorNode != value )
			{
				mBcColorNode = value;
				update();
				emit changed();
			}
		}
		

		///
		/// Barcode Style Property Getter
		///
		barcode::Style ModelBarcodeObject::bcStyle() const
		{
			return mBcStyle;
		}


		///
		/// Barcode Style Property Setter
		///
		void ModelBarcodeObject::setBcStyle( const barcode::Style& value )
		{
			if ( mBcStyle != value )
			{
				mBcStyle = value;
				update();
				emit changed();
			}
		}


		///
		/// Barcode Format Digits Property Getter
		///
		int ModelBarcodeObject::bcFormatDigits() const
		{
			return mBcFormatDigits;
		}


		///
		/// Barcode Format Digits Property Setter
		///
		void ModelBarcodeObject::setBcFormatDigits( int value )
		{
			if ( mBcFormatDigits != value )
			{
				mBcFormatDigits = value;
				update();
				emit changed();
			}
		}


		///
		/// Draw shadow of object
		///
		void ModelBarcodeObject::drawShadow( QPainter*      painter,
		                                     bool           inEditor,
		                                     merge::Record* record,
		                                     Variables*     variables ) const
		{
			// Barcodes don't support shadows.
		}

	
		///
		/// Draw object itself
		///
		void ModelBarcodeObject::drawObject( QPainter*      painter,
		                                     bool           inEditor,
		                                     merge::Record* record,
		                                     Variables*     variables ) const
		{
			QColor bcColor = mBcColorNode.color( record, variables );

			if ( inEditor )
			{
				drawBcInEditor( painter, bcColor );
			}
			else
			{
				drawBc( painter, bcColor, record, variables );
			}
		}


		///
		/// Path to test for hover condition
		///
		QPainterPath ModelBarcodeObject::hoverPath( double scale ) const
		{
			return mHoverPath;
		}


		///
		/// Size updated
		///
		void ModelBarcodeObject::sizeUpdated()
		{
			update();
		}


		///
		/// Update cached information for editor view
		///
		void ModelBarcodeObject::update()
		{
			//
			// Build barcode from data
			//
			if ( mEditorBarcode )
			{
				delete mEditorBarcode;
			}
			mEditorBarcode = glbarcode::Factory::createBarcode( mBcStyle.fullId().toStdString() );
			if ( !mEditorBarcode )
			{
				qWarning() << "Invalid barcode style" << mBcStyle.fullId() << "using \"code39\".";
				mBcStyle = barcode::Backends::defaultStyle();
				mEditorBarcode = glbarcode::Factory::createBarcode( mBcStyle.id().toStdString() );
			}
			mEditorBarcode->setChecksum(mBcChecksumFlag);
			mEditorBarcode->setShowText(mBcTextFlag);

			mEditorBarcode->build( mBcData.toStdString(), mW.pt(), mH.pt() );

			//
			// Build a place holder barcode to display in editor, if cannot display actual barcode
			//
			if ( mEditorDefaultBarcode )
			{
				delete mEditorDefaultBarcode;
			}
			mEditorDefaultBarcode = glbarcode::Factory::createBarcode( mBcStyle.fullId().toStdString() );
			if ( !mEditorDefaultBarcode )
			{
				qWarning() << "Invalid barcode style" << mBcStyle.fullId() << "using \"code39\".";
				mBcStyle = barcode::Backends::defaultStyle();
				mEditorDefaultBarcode = glbarcode::Factory::createBarcode( mBcStyle.id().toStdString() );
			}
			mEditorDefaultBarcode->setChecksum(mBcChecksumFlag);
			mEditorDefaultBarcode->setShowText(mBcTextFlag);

			mEditorDefaultBarcode->build( mBcStyle.defaultDigits().toStdString(), mW.pt(), mH.pt() );

			//
			// Adjust size
			//
			if ( mEditorBarcode->isDataValid() )
			{
				mW = Distance::pt( mEditorBarcode->width() );
				mH = Distance::pt( mEditorBarcode->height() );
			}
			else
			{
				mW = Distance::pt( mEditorDefaultBarcode->width() );
				mH = Distance::pt( mEditorDefaultBarcode->height() );
			}

			QPainterPath path;
			path.addRect( 0, 0, mW.pt(), mH.pt() );
			mHoverPath = path;
		}


		///
		/// Draw barcode in editor from cached information
		///
		void ModelBarcodeObject::drawBcInEditor( QPainter* painter, const QColor& color ) const
		{
			if ( mBcData.isEmpty() )
			{
				drawPlaceHolder( painter, color, tr("No barcode data") );
			}
			else if ( mBcData.hasPlaceHolders() )
			{
				drawPlaceHolder( painter, color, mBcData.toString() );
			}
			else if ( mEditorBarcode->isDataValid() )
			{
				painter->setPen( QPen( color ) );
				glbarcode::QtRenderer renderer(painter);
				mEditorBarcode->render( renderer );
			}
			else
			{
				drawPlaceHolder( painter, color, tr("Invalid barcode data") );
			}
		}


		///
		/// Draw barcode in final printout or preview
		///
		void
		ModelBarcodeObject::drawBc( QPainter*      painter,
		                            const QColor&  color,
		                            merge::Record* record,
		                            Variables*     variables ) const
		{
			painter->setPen( QPen( color ) );

			glbarcode::Barcode* bc = glbarcode::Factory::createBarcode( mBcStyle.fullId().toStdString() );
			bc->setChecksum(mBcChecksumFlag);
			bc->setShowText(mBcTextFlag);

			bc->build( mBcData.expand( record, variables ).toStdString(), mW.pt(), mH.pt() );

			glbarcode::QtRenderer renderer(painter);
			bc->render( renderer );
		}


		///
		/// Draw barcode place holder in editor
		///
		void
		ModelBarcodeObject::drawPlaceHolder( QPainter*      painter,
		                                     const QColor&  color,
		                                     const QString& text ) const
		{
			QString shortText = text.left( 32 ); // Don't let the text get out of hand
		
			//
			// Render box
			//
			painter->setPen( Qt::NoPen );
			painter->setBrush( QBrush( fillColor ) );
			painter->drawRect( QRectF( 0, 0, mW.pt(), mH.pt() ) );

			//
			// Render default barcode
			//
			painter->setPen( QPen( color ) );
			glbarcode::QtRenderer renderer(painter);
			mEditorDefaultBarcode->render( renderer );

			//
			// Determine font size for text
			//
			QFont font( "Sans" );
			font.setPointSizeF( 6 );

			QFontMetricsF fm( font );
			QRectF textRect = fm.boundingRect( shortText );

			double wPts = (mW - 2*pad).pt();
			double hPts = (mH - 2*pad).pt();
			if ( (wPts < textRect.width()) || (hPts < textRect.height()) )
			{
				double scaleX = wPts / textRect.width();
				double scaleY = hPts / textRect.height();
				font.setPointSizeF( std::max( 6 * std::min( scaleX, scaleY ), MIN_POINT_SIZE ) );
			}

			//
			// Render hole for text (font size may have changed above)
			//
			fm = QFontMetricsF( font );
			textRect = fm.boundingRect( shortText );
		
			QRectF holeRect( (mW.pt() - textRect.width())/2 - pad.pt(),
			                 (mH.pt() - textRect.height())/2 - pad.pt(),
			                 textRect.width() + 2*pad.pt(),
			                 textRect.height() + 2*pad.pt() );

			painter->setPen( Qt::NoPen );
			painter->setBrush( QBrush( fillColor ) );
			painter->drawRect( holeRect );

			//
			// Render text
			//
			painter->setFont( font );
			painter->setPen( QPen( color ) );
			painter->drawText( QRectF( 0, 0, mW.pt(), mH.pt() ), Qt::AlignCenter, shortText );
		}

	
	}
}
