//  PreviewOverlayItem.hpp
//
//  Copyright (C) 2013  Jaye Evins <evins@snaught.com>
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

#ifndef PreviewOverlayItem_hpp
#define PreviewOverlayItem_hpp


#include "model/PageRenderer.hpp"

#include <QGraphicsItem>


namespace glabels
{

        ///
        ///  PreviewOverlayItem Widget
        ///
        class PreviewOverlayItem : public QGraphicsItem
        {

                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        public:
                PreviewOverlayItem( const model::PageRenderer* renderer, QGraphicsItem* parent = nullptr );


                /////////////////////////////////////
                // Virtual method implementations
                /////////////////////////////////////
        public:
                QRectF boundingRect() const override;
                void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget ) override;


                /////////////////////////////////
                // Private Data
                /////////////////////////////////
        private:
                const model::PageRenderer* mRenderer;

        };

}


#endif // PreviewOverlayItem_hpp
