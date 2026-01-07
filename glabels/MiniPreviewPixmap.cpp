//  MiniPreviewPixmap.cpp
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


#include "MiniPreviewPixmap.hpp"


#include "RollTemplatePath.hpp"
#include "model/Template.hpp"


//
// Private
//
namespace
{
        const QColor paperColor( 217, 217, 217 );
        const QColor paperOutlineColor( 0, 0, 0 );
        const double paperOutlineWidthPixels = 1.0;

        const QColor labelColor( 242, 242, 242 );
        const QColor labelOutlineColor( 64, 64, 64 );
        const double labelOutlineWidthPixels = 1.0;
}


namespace glabels
{

        MiniPreviewPixmap::MiniPreviewPixmap( const model::Template& tmplate, int width, int height )
                : QPixmap( width, height )
        {
                draw( tmplate, width, height );
        }


        void MiniPreviewPixmap::draw( const model::Template& tmplate, int width, int height )
        {
                fill( Qt::transparent );

                QPainter painter( this );

                painter.setBackgroundMode( Qt::TransparentMode );
                painter.setRenderHint( QPainter::Antialiasing, true );

                // For "Roll" templates, allow extra room for tape width and continuation break lines
                model::Distance drawWidth  = tmplate.pageWidth();
                model::Distance drawHeight = tmplate.pageHeight();
                if ( tmplate.isRoll() )
                {
                        drawWidth   = tmplate.rollWidth();
                        drawHeight *= 1.2;
                }

                double w = width - 1;
                double h = height - 1;
                double scale;
                if ( (w/drawWidth.pt()) > (h/drawHeight.pt()) )
                {
                        scale = h / drawHeight.pt();
                }
                else
                {
                        scale = w / drawWidth.pt();
                }
                painter.scale( scale, scale );

                model::Distance xOffset = ( model::Distance::pt(width/scale) - tmplate.pageWidth() ) / 2;
                model::Distance yOffset = ( model::Distance::pt(height/scale) - tmplate.pageHeight() ) / 2;
                painter.translate( xOffset.pt(), yOffset.pt() );

                drawPaper( painter, tmplate, scale );
                drawLabelOutlines( painter, tmplate, scale );
        }


        void MiniPreviewPixmap::drawPaper( QPainter& painter, const model::Template& tmplate, double scale )
        {
                QBrush brush( paperColor );
                QPen pen( paperOutlineColor );
                pen.setWidth( paperOutlineWidthPixels/scale );

                painter.save();

                painter.setBrush( brush );
                painter.setPen( pen );

                if ( !tmplate.isRoll() )
                {
                        painter.drawRect( 0, 0, tmplate.pageWidth().pt(), tmplate.pageHeight().pt() );
                }
                else
                {
                        painter.drawPath( RollTemplatePath( tmplate ) );
                }

                painter.restore();
        }


        void MiniPreviewPixmap::drawLabelOutlines( QPainter& painter, const model::Template& tmplate, double scale )
        {
                QBrush brush( labelColor );
                QPen pen( labelOutlineColor );
                pen.setWidth( labelOutlineWidthPixels/scale );

                painter.save();

                painter.setBrush( brush );
                painter.setPen( pen );

                auto frame = tmplate.frame();
                for ( model::Point p0 : frame->getOrigins() )
                {
                        drawLabelOutline( painter, frame, p0 );
                }

                painter.restore();
        }


        void MiniPreviewPixmap::drawLabelOutline( QPainter& painter, const model::Frame* frame, const model::Point& p0 )
        {
                painter.save();

                painter.translate( p0.x().pt(), p0.y().pt() );
                painter.drawPath( frame->path() );

                painter.restore();
        }

} // namespace glabels
