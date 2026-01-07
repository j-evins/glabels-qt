//  DataCache.hpp
//
//  Copyright (C) 2017  Jaye Evins <evins@snaught.com>
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

#ifndef model_DataCache_hpp
#define model_DataCache_hpp


#include "Model.hpp"


namespace glabels::model
{

        class DataCache
        {
        public:
                DataCache();

                DataCache( const QList<ModelObject*>& objects );

                bool hasImage( const QString& name ) const;
                QImage getImage( const QString& name ) const;
                void addImage( const QString& name, const QImage& image );
                QList<QString> imageNames() const;

                bool hasSvg( const QString& name ) const;
                QByteArray getSvg( const QString& name ) const;
                void addSvg( const QString& name, const QByteArray& svg );
                QList<QString> svgNames() const;


        private:
                QMap<QString,QImage> mImageMap;
                QMap<QString,QByteArray> mSvgMap;

        };

}


#endif // model_DataCache_hpp
