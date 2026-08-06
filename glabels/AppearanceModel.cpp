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
                qApp->setStyle( QStyleFactory::create( "Fusion" ) );

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

                QPalette lightPalette;
                lightPalette.setColor(                     QPalette::Window,          QColor("#ffefefef") );
                lightPalette.setColor(                     QPalette::WindowText,      Qt::black );
                lightPalette.setColor( QPalette::Disabled, QPalette::WindowText,      QColor(127,127,127) ); //?
                lightPalette.setColor(                     QPalette::Base,            Qt::white );
                lightPalette.setColor(                     QPalette::AlternateBase,   QColor("#fff7f7f7") );
                lightPalette.setColor(                     QPalette::ToolTipBase,     QColor("#ffffffdc") );
                lightPalette.setColor(                     QPalette::ToolTipText,     Qt::black );
                lightPalette.setColor(                     QPalette::Text,            Qt::black );
                lightPalette.setColor( QPalette::Disabled, QPalette::Text,            QColor(127,127,127) ); //?
                lightPalette.setColor(                     QPalette::Dark,            QColor("ff9f9f9f") );
                lightPalette.setColor(                     QPalette::Shadow,          QColor("#ff767676") );
                lightPalette.setColor(                     QPalette::Button,          QColor("#ffefefef") );
                lightPalette.setColor(                     QPalette::ButtonText,      Qt::black );
                lightPalette.setColor( QPalette::Disabled, QPalette::ButtonText,      QColor(127,127,127) ); //?
                lightPalette.setColor(                     QPalette::BrightText,      Qt::white );
                lightPalette.setColor(                     QPalette::Link,            QColor("#ff0000ff") );
                lightPalette.setColor(                     QPalette::LinkVisited,     QColor("#ffff00ff") );
                lightPalette.setColor(                     QPalette::Highlight,       QColor("#ff308cc6") );
                lightPalette.setColor( QPalette::Disabled, QPalette::Highlight,       QColor(80,80,80) ); //?
                lightPalette.setColor(                     QPalette::HighlightedText, Qt::white );
                lightPalette.setColor( QPalette::Disabled, QPalette::HighlightedText, QColor(127,127,127) ); //?
                qApp->setPalette( lightPalette );

                if ( mInstance ) emit mInstance->changed();
        }


        //
        // Set Dark Mode
        //
        void AppearanceModel::setDarkMode( )
        {
                mMode = model::Settings::DARK_MODE;

                QIcon::setThemeName( "glabels-flat-dark"  );

                QPalette darkPalette;
                darkPalette.setColor(                     QPalette::Window,          QColor(53,53,53) );
                darkPalette.setColor(                     QPalette::WindowText,      Qt::white );
                darkPalette.setColor( QPalette::Disabled, QPalette::WindowText,      QColor(127,127,127) );
                darkPalette.setColor(                     QPalette::Base,            QColor(42,42,42) );
                darkPalette.setColor(                     QPalette::AlternateBase,   QColor(66,66,66) );
                darkPalette.setColor(                     QPalette::ToolTipBase,     Qt::black );
                darkPalette.setColor(                     QPalette::ToolTipText,     Qt::white );
                darkPalette.setColor(                     QPalette::Text,            Qt::white );
                darkPalette.setColor( QPalette::Disabled, QPalette::Text,            QColor(127,127,127) );
                darkPalette.setColor(                     QPalette::Dark,            QColor(35,35,35) );
                darkPalette.setColor(                     QPalette::Shadow,          QColor(20,20,20) );
                darkPalette.setColor(                     QPalette::Button,          QColor(53,53,53) );
                darkPalette.setColor(                     QPalette::ButtonText,      Qt::white );
                darkPalette.setColor( QPalette::Disabled, QPalette::ButtonText,      QColor(127,127,127) );
                darkPalette.setColor(                     QPalette::BrightText,      Qt::red );
                darkPalette.setColor(                     QPalette::Link,            QColor(42,130,218) );
                darkPalette.setColor(                     QPalette::LinkVisited,     QColor("#ffff00ff") ); //?
                darkPalette.setColor(                     QPalette::Highlight,       QColor(42,130,218) );
                darkPalette.setColor( QPalette::Disabled, QPalette::Highlight,       QColor(80,80,80) );
                darkPalette.setColor(                     QPalette::HighlightedText, Qt::white );
                darkPalette.setColor( QPalette::Disabled, QPalette::HighlightedText, QColor(127,127,127) );
                qApp->setPalette( darkPalette );

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
