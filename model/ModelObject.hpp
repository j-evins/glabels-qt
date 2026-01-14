//  ModelObject.hpp
//
//  Copyright (C) 2013-2026  Jaye Evins <evins@snaught.com>
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

#ifndef model_ModelObject_hpp
#define model_ModelObject_hpp


#include "ColorNode.hpp"
#include "Distance.hpp"
#include "Handle.hpp"
#include "Outline.hpp"
#include "TextNode.hpp"
#include "Variables.hpp"

#include "barcode/Style.hpp"
#include "merge/Record.hpp"

#include <QObject>
#include <QFont>
#include <QTransform>
#include <QPainter>

#include <list>
#include <memory>


namespace glabels::model
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

                ModelObject( Distance          x0,
                             Distance          y0,
                             Distance          w,
                             Distance          h,
                             bool              lockAspectRatio = false,
                             const QTransform& matrix = QTransform(),
                             bool              shadowState = false,
                             Distance          shadowX = 0,
                             Distance          shadowY = 0,
                             double            shadowOpacity = 1.0,
                             const ColorNode&  shadowColorNode = ColorNode() );

                ModelObject( const ModelObject* object );

        public:
                virtual ~ModelObject() = default;


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
                void setX0( Distance value );


                //
                // y0 Property ( y coordinate of origin )
                //
                Distance y0() const;
                void setY0( Distance value );


                //
                // w Property ( width of bounding box )
                //
                Distance w() const;
                void setW( Distance value );


                //
                // h Property ( height of bounding box )
                //
                Distance h() const;
                void setH( Distance value );


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
                void setShadowX( Distance value );


                //
                // Shadow y Offset Property
                //
                Distance shadowY() const;
                void setShadowY( Distance value );


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
                virtual const QImage& image() const;
                virtual void setImage( const QImage& value );
                virtual void setImage( const QString& name, const QImage& value );


                //
                // Virtual Image Property: svg
                //
                virtual const QByteArray& svg() const;
                virtual void setSvg( const QString& name, const QByteArray& value );


                ///////////////////////////////////////////////////////////////
                // Shape Properties Virtual Interface
                ///////////////////////////////////////////////////////////////
        public:
                //
                // Virtual Shape Property: lineWidth
                //
                virtual Distance lineWidth() const;
                virtual void setLineWidth( Distance value );


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
                void setPosition( Distance x0, Distance y0 );
                void setPositionRelative( Distance dx, Distance dy );
                Size size() const;
                void setSize( Distance w, Distance h );
                void setSize( Size size );
                void setSizeHonorAspect( Distance w, Distance h );
                void setWHonorAspect( Distance w );
                void setHHonorAspect( Distance h );
                Region getExtent();
                void rotate( double thetaDegs );
                void flipHoriz();
                void flipVert();
                bool isLocatedAt( double scale, Distance x, Distance y ) const;
                const Handle& handleAt( double scale, Distance x, Distance y ) const;


                ///////////////////////////////////////////////////////////////
                // Drawing operations
                ///////////////////////////////////////////////////////////////
        public:
                void draw( QPainter*            painter,
                           bool                 inEditor,
                           const merge::Record& record,
                           const Variables&     variables ) const;

                void drawSelectionHighlight( QPainter* painter, double scale ) const;

        protected:
                virtual void drawShadow( QPainter*            painter,
                                         bool                 inEditor,
                                         const merge::Record& record,
                                         const Variables&     variables ) const = 0;

                virtual void drawObject( QPainter*            painter,
                                         bool                 inEditor,
                                         const merge::Record& record,
                                         const Variables&     variables ) const = 0;

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

                Outline           mOutline;

                QList<Handle>     mHandles;


                ///////////////////////////////////////////////////////////////
                // Private Members
                ///////////////////////////////////////////////////////////////
        private:
                static int msNextId;
                int        mId;

                QTransform mMatrix;

        };

}


#endif // model_ModelObject_hpp
