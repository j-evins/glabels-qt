//  PaletteUtil.cpp
//
//  Copyright (C) 2026  Jaye Evins <evins@snaught.com>
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


#include "PaletteUtil.hpp"

#include <QApplication>
#include <QDebug>
#include <QMetaEnum>
#include <QPalette>
#include <QStyle>
#include <QStyleHints>


namespace glabels
{

        void PaletteUtil::printPalette()
        {
                QMetaEnum metaEnum = QMetaEnum::fromType<QPalette::ColorRole>();
                QList<QPalette::ColorRole> roleList =
                        {
                                QPalette::Window,
                                QPalette::WindowText,
                                QPalette::Base,
                                QPalette::AlternateBase,
                                QPalette::ToolTipBase,
                                QPalette::ToolTipText,
                                QPalette::PlaceholderText,
                                QPalette::Text,
                                QPalette::Button,
                                QPalette::ButtonText,
                                QPalette::BrightText,
                                QPalette::Light,
                                QPalette::Midlight,
                                QPalette::Dark,
                                QPalette::Mid,
                                QPalette::Shadow,
                                QPalette::Highlight,
                                QPalette::HighlightedText,
                                QPalette::Link,
                                QPalette::LinkVisited
                        };

                QPalette palette = QApplication::palette();

                qDebug().nospace() << " ";

                qDebug().nospace() << "style = " << QApplication::style()->name();
#if QT_VERSION >= QT_VERSION_CHECK(6,8,0)
                qDebug().nospace() << "scheme = " << QGuiApplication::styleHints()->colorScheme();
#endif
                qDebug().nospace() << " ";

                for ( auto role : roleList )
                {
                        QColor color0 = palette.color( role );
                        qDebug().nospace() << "p.setColor( QPalette::" << metaEnum.key(role) << ", "
                                           << "QColor( " << color0.red() << ", " << color0.green() << ", " << color0.blue() << " )"
                                           << " );";

                }

                qDebug().nospace() << " ";

                for ( auto role : roleList )
                {
                        QColor color0 = palette.color( role );
                        QColor color1 = palette.color( QPalette::Disabled, role );
                        if ( color1 != color0 )
                        {
                                qDebug().nospace() << "p.setColor( QPalette::Disabled, QPalette::" << metaEnum.key(role) << ", "
                                                   << "QColor( " << color1.red() << ", " << color1.green() << ", " << color1.blue() << " )"
                                                   << " );";
                        }
                }

                qDebug().nospace() << " ";

                for ( auto role : roleList )
                {
                        QColor color0 = palette.color( role );
                        QColor color2 = palette.color( QPalette::Inactive, role );
                        if ( color2 != color0 )
                        {
                                qDebug().nospace() << "p.setColor( QPalette::Inactive, QPalette::" << metaEnum.key(role) << ", "
                                                   << "QColor( " << color2.red() << ", " << color2.green() << ", " << color2.blue() << " )"
                                                   << " );";
                        }
                }
        }

}
