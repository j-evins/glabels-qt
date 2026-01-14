//  PreviewOverlayItem.cpp
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


#include "PreviewOverlayItem.hpp"

#include "model/PageRenderer.hpp"

#include <QDebug>


namespace glabels
{

        PreviewOverlayItem::PreviewOverlayItem( const model::PageRenderer* renderer, QGraphicsItem* parent )
                : QGraphicsItem(parent), mRenderer(renderer)
        {
                // empty
        }


        QRectF PreviewOverlayItem::boundingRect() const
        {
                return mRenderer->pageRect();
        }


        void PreviewOverlayItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
        {
                mRenderer->printPage( painter );
        }

} // namespace glabels
