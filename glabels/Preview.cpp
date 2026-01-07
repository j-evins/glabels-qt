//  Preview.cpp
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


#include "Preview.hpp"

#include "PreviewOverlayItem.hpp"
#include "RollTemplatePath.hpp"

#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsRectItem>


//
// Private
//
namespace
{
        const QColor  paperColor( 242, 242, 242 );
        const QColor  paperOutlineColor( 0, 0, 0 );
        const double  paperOutlineWidthPixels = 1;

        const QColor  shadowColor( 64, 64, 64 );
        const double  shadowOffsetPixels = 3;
        const double  shadowRadiusPixels = 12;

        const QColor  labelColor( 255, 255, 255 );
        const QColor  labelOutlineColor( 192, 192, 192 );
        const double  labelOutlineWidthPixels = 1;

        const QColor  labelNumberColor( 192, 192, 255, 128 );
        const QString labelNumberFontFamily( "Sans" );
        const double  labelNumberScale = 0.5;
}


namespace glabels
{

        ///
        /// Constructor
        ///
        Preview::Preview( QWidget *parent )
                : QGraphicsView(parent), mModel(nullptr), mRenderer(nullptr)
        {
                mScene = new QGraphicsScene();
                setScene( mScene );

                setAttribute(Qt::WA_TranslucentBackground);
                viewport()->setAutoFillBackground(false);

                setFrameStyle( QFrame::NoFrame );
                setRenderHints( QPainter::Antialiasing );
        }


        ///
        /// Set renderer
        ///
        void Preview::setRenderer( const model::PageRenderer* renderer )
        {
                mRenderer = renderer;

                connect( mRenderer, SIGNAL(changed()), this, SLOT(onRendererChanged()) );
                onRendererChanged();
        }


        ///
        /// Renderer changed handler
        ///
        void Preview::onRendererChanged()
        {
                mModel = mRenderer->model();

                mScene->clear();

                if ( mModel != nullptr )
                {
                        auto tmplate = mModel->tmplate();

                        // For "Roll" templates, allow extra room to draw continuation break lines.
                        model::Distance drawHeight = mModel->tmplate().pageHeight();
                        model::Distance drawOffset = 0;
                        if ( tmplate.isRoll() )
                        {
                                drawHeight = 1.2 * tmplate.pageHeight();
                                drawOffset = 0.1 * tmplate.pageHeight();
                        }

                        // Set scene up with a 5% margin around paper
                        model::Distance x = -0.05 * tmplate.pageWidth();
                        model::Distance y = -0.05 * drawHeight - drawOffset;
                        model::Distance w = 1.10 * tmplate.pageWidth();
                        model::Distance h = 1.10 * drawHeight;

                        mScene->setSceneRect( x.pt(), y.pt(), w.pt(), h.pt() );
                        fitInView( mScene->sceneRect(), Qt::KeepAspectRatio );

                        drawPaper();
                        drawLabels();
                        drawPreviewOverlay();
                        drawLabelNumberOverlay();
                }
        }


        void Preview::drawLabelNumberOverlaySingle( model::Distance     x,
                                                    model::Distance     y,
                                                    const QPainterPath& path,
                                                    uint32_t            labelInstance)
        {
                QBrush brush( labelNumberColor );

                auto frame = mModel->tmplate().frame();

                model::Distance w = frame->w();
                model::Distance h = frame->h();

                model::Distance minWH = min( w, h );

                auto labelText = QString::number(labelInstance);
                QGraphicsSimpleTextItem *labelNumberItem = new QGraphicsSimpleTextItem( labelText );
                labelNumberItem->setBrush( brush );
                labelNumberItem->setFont( QFont( labelNumberFontFamily, minWH.pt()*labelNumberScale, QFont::Bold ) );
                labelNumberItem->setPos( (x+w/2).pt(), (y+h/2).pt() );
                QRectF rect = labelNumberItem->boundingRect();
                labelNumberItem->setPos(labelNumberItem->x() - (rect.width() / 2), labelNumberItem->y() - (rect.height() / 2));

                mScene->addItem( labelNumberItem );
        }


        void Preview::drawLabelNumberOverlay()
        {
                auto frame = mModel->tmplate().frame();
                auto i = 0;

                for ( model::Point origin : frame->getOrigins() )
                {
                        i++;
                        drawLabelNumberOverlaySingle( origin.x(), origin.y(), frame->path(), i);
                }
        }


        ///
        /// Resize Event Handler
        ///
        void Preview::resizeEvent( QResizeEvent* event )
        {
                fitInView( mScene->sceneRect(), Qt::KeepAspectRatio );
        }


        ///
        /// Draw Paper
        ///
        void Preview::drawPaper()
        {
                auto *shadowEffect = new QGraphicsDropShadowEffect();
                shadowEffect->setColor( shadowColor );
                shadowEffect->setOffset( shadowOffsetPixels );
                shadowEffect->setBlurRadius( shadowRadiusPixels );

                QBrush brush( paperColor );
                QPen pen( paperOutlineColor );
                pen.setCosmetic( true );
                pen.setWidthF( paperOutlineWidthPixels );

                QAbstractGraphicsShapeItem* pageItem;
                auto tmplate = mModel->tmplate();
                if ( !tmplate.isRoll() )
                {
                        pageItem = new QGraphicsRectItem( 0, 0, tmplate.pageWidth().pt(), tmplate.pageHeight().pt() );
                }
                else
                {
                        pageItem = new QGraphicsPathItem( RollTemplatePath( tmplate ) );
                }
                pageItem->setBrush( brush );
                pageItem->setPen( pen );
                pageItem->setGraphicsEffect( shadowEffect );

                mScene->addItem( pageItem );
        }


        ///
        /// Draw Labels on Paper
        ///
        void Preview::drawLabels()
        {
                auto frame = mModel->tmplate().frame();

                for ( model::Point origin : frame->getOrigins() )
                {
                        drawLabel( origin.x(), origin.y(), frame->path() );
                }
        }


        ///
        /// Draw a Single Label at x,y
        ///
        void Preview::drawLabel( model::Distance     x,
                                 model::Distance     y,
                                 const QPainterPath& path )
        {
                QBrush brush( labelColor );
                QPen pen( labelOutlineColor );
                pen.setCosmetic( true );
                pen.setWidthF( labelOutlineWidthPixels );

                auto *labelOutlineItem  = new QGraphicsPathItem( path );
                labelOutlineItem->setBrush( brush );
                labelOutlineItem->setPen( pen );
                labelOutlineItem->setPos( x.pt(), y.pt() );

                mScene->addItem( labelOutlineItem );
        }


        ///
        /// Draw Preview Overlay
        ///
        void Preview::drawPreviewOverlay()
        {
                if ( mRenderer )
                {
                        auto* overlayItem = new PreviewOverlayItem( mRenderer );
                        mScene->addItem( overlayItem );
                }
        }

} // namespace glabels
