//  ColorHistory.cpp
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


#include "ColorHistory.hpp"

#include <QDebug>
#include <QSettings>


namespace glabels
{

        ColorHistory* ColorHistory::instance()
        {
                static ColorHistory* singletonInstance = nullptr;

                if ( singletonInstance == nullptr )
                {
                        singletonInstance = new ColorHistory();
                }

                return singletonInstance;
        }


        void ColorHistory::addColor( const QColor &color, const QString& name )
        {
                QString nameColor = name + ":" + color.name();

                QStringList nameColorList = readNameColorList();

                // Remove any occurrences of this color already in list
                nameColorList.removeAll( nameColor );

                // Now add to list
                nameColorList.append( nameColor );

                // Remove oldest colors, if size exceeds current max
                while ( nameColorList.size() > MAX_COLORS )
                {
                        nameColorList.removeFirst();
                }

                writeNameColorList( nameColorList );

                emit changed();
        }


        QList<QColor> ColorHistory::getColors()
        {
                QList<QColor> colorList;

                for ( QString& nameColor : readNameColorList() )
                {
                        QStringList v = nameColor.split( ':' );
                        if ( v.size() == 2 )
                        {
                                colorList << QColor( v[1] );
                        }
                        else if ( v.size() == 1 )
                        {
                                // Old-style, no name
                                colorList << QColor( v[0] );
                        }
                        else
                        {
                                // Should not happen
                                qWarning() << "Invalid color history.";
                        }
                }

                return colorList;
        }


        QStringList ColorHistory::getNames()
        {
                QStringList nameList;

                for ( QString& nameColor : readNameColorList() )
                {
                        QStringList v = nameColor.split( ':' );
                        if ( v.size() == 2 )
                        {
                                nameList << v[0];
                        }
                        else if ( v.size() == 1 )
                        {
                                 // Old-style, no name
                                nameList << QString(tr("color %1")).arg( v[0] );
                        }
                        else
                        {
                                // Should not happen
                                qWarning() << "Invalid color history.";
                        }
                }

                return nameList;
        }


        QStringList ColorHistory::readNameColorList()
        {
                QStringList defaultList;
                QSettings settings;

                settings.beginGroup( "ColorHistory" );
                QStringList nameColorList = settings.value( "colors", defaultList ).toStringList();
                settings.endGroup();

                // Remove oldest colors, if size exceeds current max
                while ( nameColorList.size() > MAX_COLORS )
                {
                        nameColorList.removeFirst();
                }

                return nameColorList;
        }


        void ColorHistory::writeNameColorList( const QStringList& nameColorList )
        {
                // Save
                QSettings settings;
                settings.beginGroup( "ColorHistory" );
                settings.setValue( "colors", nameColorList );
                settings.endGroup();
        }

} // namespace glabels
