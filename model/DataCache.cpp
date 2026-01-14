//  DataCache.cpp
//
//  Copyright (C) 2017-2026  Jaye Evins <evins@snaught.com>
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


#include "DataCache.hpp"

#include "ModelImageObject.hpp"


namespace glabels::model
{

        DataCache::DataCache()
        {
                // empty
        }


        DataCache::DataCache( const QList<ModelObject*>& objects )
        {
                for ( ModelObject* object : objects )
                {
                        if ( auto* imageObject = dynamic_cast<ModelImageObject*>(object) )
                        {
                                TextNode filenameNode = imageObject->filenameNode();
                                if ( !filenameNode.isField()  )
                                {
                                        auto& image = imageObject->image();
                                        if ( !image.isNull() )
                                        {
                                                addImage( filenameNode.data(), image );
                                        }
                                        else
                                        {
                                                auto& svg = imageObject->svg();
                                                if ( !svg.isEmpty() )
                                                {
                                                        addSvg( filenameNode.data(), svg );
                                                }
                                        }
                                }
                        }
                }
        }


        bool DataCache::hasImage( const QString& name ) const
        {
                return mImageMap.contains( name );
        }


        QImage DataCache::getImage( const QString& name ) const
        {
                return mImageMap[ name ];
        }


        void DataCache::addImage( const QString& name, const QImage& image )
        {
                mImageMap[ name ] = image;
        }


        QList<QString> DataCache::imageNames() const
        {
                return mImageMap.keys();
        }


        bool DataCache::hasSvg( const QString& name ) const
        {
                return mSvgMap.contains( name );
        }


        QByteArray DataCache::getSvg( const QString& name ) const
        {
                return mSvgMap[ name ];
        }


        void DataCache::addSvg( const QString& name, const QByteArray& svg )
        {
                mSvgMap[ name ] = svg;
        }


        QList<QString> DataCache::svgNames() const
        {
                return mSvgMap.keys();
        }


}
