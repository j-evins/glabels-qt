//  AppearanceModel.cpp
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


#include "AppearanceModel.hpp"

#include <QApplication>
#include <QDebug>
#include <QPalette>
#include <QSettings>
#include <QStyleFactory>


namespace glabels
{

        //
        // Static data
        //
        std::unique_ptr<AppearanceModel> AppearanceModel::mInstance;
        model::Settings::AppearanceMode AppearanceModel::mMode = model::Settings::LIGHT_MODE;


        //
        // Constructor
        //
        AppearanceModel::AppearanceModel()
        {
                setMode( model::Settings::appearanceMode() );

                connect( model::Settings::instance(), SIGNAL(changed()),
                         this, SLOT(onSettingsChanged()) );
        }


        //
        // Initialize
        //
        void AppearanceModel::init()
        {
                if ( !mInstance )
                {
                        mInstance.reset( new AppearanceModel() );
                }
        }


        //
        // Get singleton instance
        //
        AppearanceModel* AppearanceModel::instance()
        {
                init();
                return mInstance.get();
        }


        //
        // Query current style
        //
        AppearanceModel::Style AppearanceModel::style()
        {
                switch ( mMode )
                {
                case model::Settings::LIGHT_MODE: return STYLE_LIGHT;
                case model::Settings::DARK_MODE:  return STYLE_DARK;

                // TODO: FOLLOW_SYSTEM_MODE, return current style based on current system style

                default:
                        qWarning() << "Unknown appearance mode: " << mMode;
                        return STYLE_LIGHT;
                }
        }


        //
        // Set appearance mode
        //
        void AppearanceModel::setMode( model::Settings::AppearanceMode mode )
        {
                switch ( mode )
                {
                case model::Settings::LIGHT_MODE: setLightMode(); break;
                case model::Settings::DARK_MODE:  setDarkMode(); break;

                default:
                        qWarning() << "Unknown appearance mode: " << mode;
                        break;
                }
        }


        //
        // Set Light Mode
        //
        void AppearanceModel::setLightMode()
        {
                mMode = model::Settings::LIGHT_MODE;

                QIcon::setThemeName( "glabels-flat" );

                QPalette p;
                
                p.setColor( QPalette::Window,          QColor( 250, 249, 248 ) );
                p.setColor( QPalette::WindowText,      QColor(   0,   0,   0 ) );
                p.setColor( QPalette::Base,            QColor( 250, 249, 248 ) );
                p.setColor( QPalette::AlternateBase,   QColor( 234, 233, 232 ) );
                p.setColor( QPalette::ToolTipBase,     QColor( 255, 255, 220 ) );
                p.setColor( QPalette::ToolTipText,     QColor(   0,   0,   0 ) );
                p.setColor( QPalette::PlaceholderText, QColor( 100, 100, 100 ) );
                p.setColor( QPalette::Text,            QColor(   0,   0,   0 ) );
                p.setColor( QPalette::Button,          QColor( 250, 249, 248 ) );
                p.setColor( QPalette::ButtonText,      QColor(   0,   0,   0 ) );
                p.setColor( QPalette::BrightText,      QColor( 255, 255, 255 ) );
                p.setColor( QPalette::Light,           QColor( 255, 255, 255 ) );
                p.setColor( QPalette::Midlight,        QColor( 255, 255, 255 ) );
                p.setColor( QPalette::Dark,            QColor( 219, 218, 218 ) );
                p.setColor( QPalette::Mid,             QColor( 255, 255, 255 ) );
                p.setColor( QPalette::Shadow,          QColor(  12,  12,  12 ) );
                p.setColor( QPalette::Highlight,       QColor(  53, 132, 228 ) );
                p.setColor( QPalette::HighlightedText, QColor( 255, 255, 255 ) );
                p.setColor( QPalette::Link,            QColor(   0,   0, 255 ) );
                p.setColor( QPalette::LinkVisited,     QColor( 255,   0, 255 ) );
 
                p.setColor( QPalette::Disabled, QPalette::WindowText,      QColor( 125, 124, 124 ) );
                p.setColor( QPalette::Disabled, QPalette::Text,            QColor( 125, 124, 124 ) );
                p.setColor( QPalette::Disabled, QPalette::ButtonText,      QColor( 125, 124, 124 ) );
                p.setColor( QPalette::Disabled, QPalette::HighlightedText, QColor(   0,   0,   0 ) );
 
                p.setColor( QPalette::Inactive, QPalette::WindowText,      QColor( 146, 149, 149 ) );

                QApplication::setPalette( p );

                // Re-polish?
                auto style = QApplication::style();
                if ( style )
                {
                        style->unpolish( qApp );
                        style->polish( qApp );
                }

                if ( mInstance ) emit mInstance->changed();
        }


        //
        // Set Dark Mode
        //
        void AppearanceModel::setDarkMode( )
        {
                mMode = model::Settings::DARK_MODE;

                QIcon::setThemeName( "glabels-flat-dark"  );

                QPalette p;

                p.setColor( QPalette::Window, QColor( 50, 50, 50 ) );
                p.setColor( QPalette::WindowText,      QColor( 240, 240, 240 ) );
                p.setColor( QPalette::Base,            QColor(  36,  36,  36 ) );
                p.setColor( QPalette::AlternateBase,   QColor(  43,  43,  43 ) );
                p.setColor( QPalette::ToolTipBase,     QColor( 255, 255, 220 ) );
                p.setColor( QPalette::ToolTipText,     QColor(   0,   0,   0 ) );
                p.setColor( QPalette::PlaceholderText, QColor( 240, 240, 240 ) );
                p.setColor( QPalette::Text,            QColor( 240, 240, 240 ) );
                p.setColor( QPalette::Button,          QColor(  50,  50,  50 ) );
                p.setColor( QPalette::ButtonText,      QColor( 240, 240, 240 ) );
                p.setColor( QPalette::BrightText,      QColor(  75,  75,  75 ) );
                p.setColor( QPalette::Light,           QColor(  75,  75,  75 ) );
                p.setColor( QPalette::Midlight,        QColor(  42,  42,  42 ) );
                p.setColor( QPalette::Dark,            QColor(  33,  33,  33 ) );
                p.setColor( QPalette::Mid,             QColor(  38,  38,  38 ) );
                p.setColor( QPalette::Shadow,          QColor(  25,  25,  25 ) );
                p.setColor( QPalette::Highlight,       QColor(  48, 140, 198 ) );
                p.setColor( QPalette::HighlightedText, QColor( 240, 240, 240 ) );
                p.setColor( QPalette::Link,            QColor(  48, 140, 198 ) );
                p.setColor( QPalette::LinkVisited,     QColor( 255,   0, 255 ) );
 
                p.setColor( QPalette::Disabled, QPalette::WindowText, QColor( 130, 130, 130 ) );
                p.setColor( QPalette::Disabled, QPalette::Base,       QColor(  50,  50,  50 ) );
                p.setColor( QPalette::Disabled, QPalette::Text,       QColor( 130, 130, 130 ) );
                p.setColor( QPalette::Disabled, QPalette::ButtonText, QColor( 130, 130, 130 ) );
                p.setColor( QPalette::Disabled, QPalette::Dark,       QColor( 190, 190, 190 ) );
                p.setColor( QPalette::Disabled, QPalette::Shadow,     QColor(  37,  37,  37 ) );
                p.setColor( QPalette::Disabled, QPalette::Highlight,  QColor( 145, 145, 145 ) );

                QApplication::setPalette( p );

                // Re-polish?
                auto style = QApplication::style();
                if ( style )
                {
                        style->unpolish( qApp );
                        style->polish( qApp );
                }

                if ( mInstance ) emit mInstance->changed();
        }


        //
        // Slot to handle Settings "changed"
        //
        void AppearanceModel::onSettingsChanged()
        {

                auto newMode = model::Settings::appearanceMode();
                if ( newMode != mMode )
                {
                        setMode( newMode );
                }
        }

}
