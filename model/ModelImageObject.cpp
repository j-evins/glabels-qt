//  ModelImageObject.cpp
//
//  Copyright (C) 2013-2016  Jaye Evins <evins@snaught.com>
//
//  This file is part of gLabels-qt.
//
//  gLabels-qt is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  gLabels-qt is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
//


#include "ModelImageObject.hpp"

#include "Model.hpp"
#include "Size.hpp"

#include <QBrush>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QPen>


//
// Private
//
namespace
{
        using namespace glabels::model;

        const QColor fillColor  = QColor( 224, 224, 224, 255 );
        const QColor labelColor = QColor( 102, 102, 102, 255 );
        const Distance pad = Distance::pt(2);
}


namespace glabels::model
{

        ///
        /// Static data
        ///
        QImage ModelImageObject::smDefaultImage( ":images/checkerboard.png" );


        ///
        /// Constructor
        ///
        ModelImageObject::ModelImageObject()
        {
                mOutline.setOwner( this );

                mHandles.push_back( Handle( this, Handle::NW ) );
                mHandles.push_back( Handle( this, Handle::N ) );
                mHandles.push_back( Handle( this, Handle::NE ) );
                mHandles.push_back( Handle( this, Handle::E ) );
                mHandles.push_back( Handle( this, Handle::SE ) );
                mHandles.push_back( Handle( this, Handle::S ) );
                mHandles.push_back( Handle( this, Handle::SW ) );
                mHandles.push_back( Handle( this, Handle::W ) );

                mLockAspectRatio = true;
        }


        ///
        /// Constructor
        ///
        ModelImageObject::ModelImageObject( Distance          x0,
                                            Distance          y0,
                                            Distance          w,
                                            Distance          h,
                                            bool              lockAspectRatio,
                                            const TextNode&   filenameNode,
                                            const QTransform& matrix,
                                            bool              shadowState,
                                            Distance          shadowX,
                                            Distance          shadowY,
                                            double            shadowOpacity,
                                            const ColorNode&  shadowColorNode )
        : ModelObject( x0,
                       y0,
                       w,
                       h,
                       lockAspectRatio,
                       matrix,
                       shadowState,
                       shadowX,
                       shadowY,
                       shadowOpacity,
                       shadowColorNode )
        {
                mOutline.setOwner( this );

                mHandles.push_back( Handle( this, Handle::NW ) );
                mHandles.push_back( Handle( this, Handle::N ) );
                mHandles.push_back( Handle( this, Handle::NE ) );
                mHandles.push_back( Handle( this, Handle::E ) );
                mHandles.push_back( Handle( this, Handle::SE ) );
                mHandles.push_back( Handle( this, Handle::S ) );
                mHandles.push_back( Handle( this, Handle::SW ) );
                mHandles.push_back( Handle( this, Handle::W ) );

                mFilenameNode = filenameNode;

                loadImage();
        }


        ///
        /// Constructor
        ///
        ModelImageObject::ModelImageObject( Distance          x0,
                                            Distance          y0,
                                            Distance          w,
                                            Distance          h,
                                            bool              lockAspectRatio,
                                            const QString&    filename,
                                            const QImage&     image,
                                            const QTransform& matrix,
                                            bool              shadowState,
                                            Distance          shadowX,
                                            Distance          shadowY,
                                            double            shadowOpacity,
                                            const ColorNode&  shadowColorNode )
        : ModelObject( x0,
                       y0,
                       w,
                       h,
                       lockAspectRatio,
                       matrix,
                       shadowState,
                       shadowX,
                       shadowY,
                       shadowOpacity,
                       shadowColorNode )
        {
                mOutline.setOwner( this );

                mHandles.push_back( Handle( this, Handle::NW ) );
                mHandles.push_back( Handle( this, Handle::N ) );
                mHandles.push_back( Handle( this, Handle::NE ) );
                mHandles.push_back( Handle( this, Handle::E ) );
                mHandles.push_back( Handle( this, Handle::SE ) );
                mHandles.push_back( Handle( this, Handle::S ) );
                mHandles.push_back( Handle( this, Handle::SW ) );
                mHandles.push_back( Handle( this, Handle::W ) );

                mImage = image;
                mFilenameNode = TextNode( false, filename );
        }


        ///
        /// Constructor
        ///
        ModelImageObject::ModelImageObject( Distance          x0,
                                            Distance          y0,
                                            Distance          w,
                                            Distance          h,
                                            bool              lockAspectRatio,
                                            const QString&    filename,
                                            const QByteArray& svg,
                                            const QTransform& matrix,
                                            bool              shadowState,
                                            Distance          shadowX,
                                            Distance          shadowY,
                                            double            shadowOpacity,
                                            const ColorNode&  shadowColorNode )
        : ModelObject( x0,
                       y0,
                       w,
                       h,
                       lockAspectRatio,
                       matrix,
                       shadowState,
                       shadowX,
                       shadowY,
                       shadowOpacity,
                       shadowColorNode )
        {
                mOutline.setOwner( this );

                mHandles.push_back( Handle( this, Handle::NW ) );
                mHandles.push_back( Handle( this, Handle::N ) );
                mHandles.push_back( Handle( this, Handle::NE ) );
                mHandles.push_back( Handle( this, Handle::E ) );
                mHandles.push_back( Handle( this, Handle::SE ) );
                mHandles.push_back( Handle( this, Handle::S ) );
                mHandles.push_back( Handle( this, Handle::SW ) );
                mHandles.push_back( Handle( this, Handle::W ) );

                if ( QSvgRenderer( svg ).isValid() )
                {
                        mSvg = svg;
                }

                mFilenameNode = TextNode( false, filename );
        }


        ///
        /// Copy constructor
        ///
        ModelImageObject::ModelImageObject( const ModelImageObject* object ) : ModelObject(object)
        {
                mFilenameNode = object->mFilenameNode;
                mImage = object->mImage;
                mSvg = object->mSvg;
        }


        ///
        /// Clone
        ///
        ModelImageObject* ModelImageObject::clone() const
        {
                return new ModelImageObject( this );
        }


        ///
        /// Image filenameNode Property Getter
        ///
        TextNode ModelImageObject::filenameNode() const
        {
                return mFilenameNode;
        }


        ///
        /// Image filenameNode Property Setter
        ///
        void ModelImageObject::setFilenameNode( const TextNode& value )
        {
                if ( mFilenameNode != value )
                {
                        mFilenameNode = value;
                        loadImage();

                        emit changed();
                }
        }


        ///
        /// Image image Property Getter
        ///
        const QImage& ModelImageObject::image() const
        {
                return mImage;
        }


        ///
        /// Image Property Setter
        ///
        void ModelImageObject::setImage( const QImage& value )
        {
                if ( !value.isNull() )
                {
                        mSvg.clear();

                        mImage = value;
                        quint16 cs = qChecksum( QByteArray( (const char*)mImage.constBits(), mImage.sizeInBytes() ) );
                        mFilenameNode = TextNode( false, QString("%image_%1%").arg( cs ) );

                        emit changed();
                }
        }


        ///
        /// Image Property Setter
        ///
        void ModelImageObject::setImage( const QString& name, const QImage& value )
        {
                if ( !value.isNull() )
                {
                        mSvg.clear();

                        mImage = value;
                        mFilenameNode = TextNode( false, name );

                        emit changed();
                }
        }


        ///
        /// Image svg Property Getter
        ///
        const QByteArray& ModelImageObject::svg() const
        {
                return mSvg;
        }


        ///
        /// Image svgSource Property Setter
        ///
        void ModelImageObject::setSvg( const QString& name, const QByteArray& value )
        {
                if ( !value.isEmpty() )
                {
                        mImage = QImage();  // clear
                        mSvg.clear();

                        if ( QSvgRenderer( value ).isValid() )
                        {
                                mSvg = value;
                        }
                        mFilenameNode = TextNode( false, name );

                        emit changed();
                }
        }


        ///
        /// naturalSize Property Getter (assumes 72 DPI, i.e. 1pixel == 1pt)
        ///
        Size ModelImageObject::naturalSize() const
        {
                Size size( Distance::pt(72), Distance::pt(72) );

                if ( !mImage.isNull() )
                {
                        QSize qsize = mImage.size();
                        size.setW( Distance::pt( qsize.width() ) );
                        size.setH( Distance::pt( qsize.height() ) );
                }
                else if ( !mSvg.isEmpty() )
                {
                        QSize qsize = QSvgRenderer( mSvg ).defaultSize();
                        size.setW( Distance::pt( qsize.width() ) );
                        size.setH( Distance::pt( qsize.height() ) );
                }

                return size;
        }


        ///
        /// Draw shadow of object
        ///
        void ModelImageObject::drawShadow( QPainter*            painter,
                                           bool                 inEditor,
                                           const merge::Record& record,
                                           const Variables&     variables ) const
        {
                QRectF destRect( 0, 0, mW.pt(), mH.pt() );

                QColor shadowColor = mShadowColorNode.color( record, variables );
                shadowColor.setAlphaF( mShadowOpacity );

                if ( !mImage.isNull() && mImage.hasAlphaChannel() && (mImage.depth() == 32) )
                {
                        auto shadowImage = createShadowImage( mImage, shadowColor );
                        painter->drawImage( destRect, shadowImage );
                }
                else if ( !mImage.isNull() || !mSvg.isEmpty() || inEditor )
                {
                        painter->setBrush( shadowColor );
                        painter->setPen( QPen( Qt::NoPen ) );

                        painter->drawRect( destRect );
                }
                else
                {
                        QString filename = mFilenameNode.text( record, variables ).trimmed();
                        QImage image;
                        QByteArray svg;
                        if ( readImageFile( filename, image, svg ) )
                        {
                                if ( !image.isNull() && image.hasAlphaChannel() && (image.depth() == 32) )
                                {
                                        QImage shadowImage = createShadowImage( image, shadowColor );
                                        painter->drawImage( destRect, shadowImage );
                                }
                                else
                                {
                                        painter->setBrush( shadowColor );
                                        painter->setPen( QPen( Qt::NoPen ) );

                                        painter->drawRect( destRect );
                                }
                        }
                }
        }


        ///
        /// Draw object itself
        ///
        void ModelImageObject::drawObject( QPainter*            painter,
                                           bool                 inEditor,
                                           const merge::Record& record,
                                           const Variables&     variables ) const
        {
                QRectF destRect( 0, 0, mW.pt(), mH.pt() );

                if ( inEditor && (mFilenameNode.isField() || (mImage.isNull() && mSvg.isEmpty()) ) )
                {
                        //
                        // Render default place holder image
                        //
                        painter->save();
                        painter->setRenderHint( QPainter::SmoothPixmapTransform, false );
                        painter->drawImage( destRect, smDefaultImage );
                        painter->restore();

                        //
                        // Print label on top of place holder image, if we have room
                        //
                        if ( (mW > 6*pad) && (mH > 4*pad) )
                        {
                                QString labelText = tr("No image");
                                if ( mFilenameNode.isField() )
                                {
                                        labelText = QString( "${%1}" ).arg( mFilenameNode.data() );
                                }

                                // Determine font size for labelText
                                QFont font( "Sans" );
                                font.setPointSizeF( 6 );

                                QFontMetricsF fm( font );
                                QRectF textRect = fm.boundingRect( labelText );

                                double wPts = (mW - 2*pad).pt();
                                double hPts = (mH - 2*pad).pt();
                                if ( (wPts < textRect.width()) || (hPts < textRect.height()) )
                                {
                                        double scaleX = wPts / textRect.width();
                                        double scaleY = hPts / textRect.height();
                                        font.setPointSizeF( 6 * std::min( scaleX, scaleY ) );
                                }

                                // Render hole for text (font size may have changed above)
                                fm = QFontMetricsF( font );
                                textRect = fm.boundingRect( labelText );

                                QRectF holeRect( (mW.pt() - textRect.width())/2 - pad.pt(),
                                                 (mH.pt() - textRect.height())/2 - pad.pt(),
                                                 textRect.width() + 2*pad.pt(),
                                                 textRect.height() + 2*pad.pt() );

                                painter->setPen( Qt::NoPen );
                                painter->setBrush( QBrush( fillColor ) );
                                painter->drawRect( holeRect );

                                // Render text
                                painter->setFont( font );
                                painter->setPen( QPen( labelColor ) );
                                painter->drawText( QRectF( 0, 0, mW.pt(), mH.pt() ),
                                                   Qt::AlignCenter,
                                                   labelText );
                        }
                }
                else if ( !mImage.isNull() )
                {
                        painter->drawImage( destRect, mImage );
                }
                else if ( !mSvg.isEmpty() )
                {
                        QSvgRenderer( mSvg ).render( painter, destRect );
                }
                else if ( mFilenameNode.isField() )
                {
                        QString filename = mFilenameNode.text( record, variables ).trimmed();
                        QImage image;
                        QByteArray svg;
                        if ( readImageFile( filename, image, svg ) )
                        {
                                if ( !image.isNull() )
                                {
                                        painter->drawImage( destRect, image );
                                }
                                else if ( !svg.isEmpty() )
                                {
                                        QSvgRenderer( svg ).render( painter, destRect );
                                }
                        }
                }
        }


        ///
        /// Path to test for hover condition
        ///
        QPainterPath ModelImageObject::hoverPath( double scale ) const
        {
                QPainterPath path;
                path.addRect( 0, 0, mW.pt(), mH.pt() );

                return path;
        }


        ///
        /// Load image
        ///
        void ModelImageObject::loadImage()
        {
                mImage = QImage(); // clear
                mSvg.clear();

                if ( !mFilenameNode.isField() )
                {
                        QString filename = mFilenameNode.data();
                        if ( readImageFile( filename, mImage, mSvg ) )
                        {
                                double aspectRatio = 0;
                                if ( !mSvg.isEmpty() )
                                {
                                        // Adjust size based on aspect ratio of SVG image
                                        QRectF rect = QSvgRenderer( mSvg ).viewBoxF();
                                        aspectRatio = rect.width() ? rect.height() / rect.width() : 0;
                                }
                                else
                                {
                                        // Adjust size based on aspect ratio of image
                                        double imageW = mImage.width();
                                        double imageH = mImage.height();
                                        aspectRatio = imageW ? imageH / imageW : 0;
                                }

                                if ( aspectRatio )
                                {
                                        if ( mH > mW*aspectRatio )
                                        {
                                                mH = mW*aspectRatio;
                                        }
                                        else
                                        {
                                                mW = mH/aspectRatio;
                                        }
                                }
                        }
                }
        }


        ///
        /// Read an image or svg file
        ///
        bool ModelImageObject::readImageFile( const QString& fileName,
                                              QImage&        image,
                                              QByteArray&    svg ) const
        {
                image = QImage(); // clear
                svg.clear();

                if ( !fileName.isEmpty() )
                {
                        QFileInfo fileInfo( fileName );
                        if ( fileInfo.isRelative() )
                        {
                                // Look for image file relative to project file 1st then CWD 2nd
                                auto* model = dynamic_cast<Model*>( parent() );
                                QDir::setSearchPaths( "images", {model ? model->dirPath() : "", QDir::currentPath()} );
                                fileInfo.setFile( QString("images:") + fileName );
                        }

                        if ( fileInfo.isReadable() )
                        {
                                if ( fileInfo.suffix().toLower() == "svg" )
                                {
                                        QFile file( fileInfo.filePath() );
                                        if ( file.open( QFile::ReadOnly ) )
                                        {
                                                svg = file.readAll();
                                                file.close();
                                                QSvgRenderer renderer( svg );
                                                if ( !renderer.isValid() )
                                                {
                                                        svg.clear();
                                                }
                                        }
                                }
                                else
                                {
                                        image = QImage( fileInfo.filePath() );
                                }
                        }
                }

                return !image.isNull() || !svg.isEmpty();
        }


        ///
        /// Create shadow image
        ///
        QImage ModelImageObject::createShadowImage( const QImage& image,
                                                    const QColor& color ) const
        {
                int r = color.red();
                int g = color.green();
                int b = color.blue();
                int a = color.alpha();

                QImage shadow = image;
                for ( int iy = 0; iy < shadow.height(); iy++ )
                {
                        auto* scanLine = (QRgb*)shadow.scanLine( iy );

                        for ( int ix = 0; ix < shadow.width(); ix++ )
                        {
                                scanLine[ix] = qRgba( r, g, b, (a*qAlpha(scanLine[ix]))/255 );
                        }
                }

                return shadow;
        }

}
