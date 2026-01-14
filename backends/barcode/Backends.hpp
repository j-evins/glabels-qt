//  Backends.hpp
//
//  Copyright (C) 2014-2026  Jaye Evins <evins@snaught.com>
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

#ifndef barcode_Backends_hpp
#define barcode_Backends_hpp


#include "Style.hpp"

#include <QList>
#include <QMap>
#include <QObject>
#include <QString>


namespace glabels::barcode
{

        ///
        ///  Backends Database
        ///
        class Backends : public QObject
        {
                Q_OBJECT


                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        private:
                Backends();

        public:
                static void init();

                /////////////////////////////////
                // Public Methods
                /////////////////////////////////
        public:
                static const QStringList& backendList();
                static QString backendName( const QString& backendId );
                static const QList<Style>& styleList();
                static const Style& defaultStyle();
                static const Style& style( const QString& backendId, const QString& StyleId );


                /////////////////////////////////
                // Private Methods
                /////////////////////////////////
        private:
                static void registerBackend( const QString &backendId, const QString &backendName );

                static void registerStyle( const QString& id,
                                           const QString& backendId,
                                           const QString& name,
                                           bool           canText,
                                           bool           textOptional,
                                           bool           canChecksum,
                                           bool           checksumOptional,
                                           const QString& defaultDigits,
                                           bool           canFreeForm,
                                           int            preferedN );


                /////////////////////////////////
                // Private Members
                /////////////////////////////////
                static QStringList mBackendIdList;
                static QMap<QString,QString> mBackendNameMap;

                static QList<Style> mStyleList;

        };

}


#endif // barcode_Backends_hpp
