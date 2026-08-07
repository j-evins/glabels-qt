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

                QColor white( 255, 255, 255 );
                QColor gray239( 239, 239, 239 );
                QColor gray247( 247, 247, 247 );
                QColor gray202( 202, 202, 202 );
                QColor gray190( 190, 190, 190 );
                QColor gray184( 184, 184, 184 );
                QColor gray177( 177, 177, 177 );
                QColor gray159( 159, 159, 159 );
                QColor gray145( 145, 145, 145 );
                QColor gray118( 118, 118, 118 );
                QColor black( 0, 0, 0 );

                QColor toolTipColor( 255, 255, 220 );
                QColor highlightColor( 48, 140, 198 );
                QColor linkColor( 0, 0, 255 );
                QColor linkVisitedColor( 255, 0, 255 );

                QPalette lightPalette;

                lightPalette.setColor( QPalette::Window,          gray239 );
                lightPalette.setColor( QPalette::WindowText,      black );
                lightPalette.setColor( QPalette::Base,            white );
                lightPalette.setColor( QPalette::AlternateBase,   gray247 );
                lightPalette.setColor( QPalette::ToolTipBase,     toolTipColor );
                lightPalette.setColor( QPalette::ToolTipText,     black );
                lightPalette.setColor( QPalette::PlaceholderText, black );
                lightPalette.setColor( QPalette::Text,            black );
                lightPalette.setColor( QPalette::Button,          gray239 );
                lightPalette.setColor( QPalette::ButtonText,      black );
                lightPalette.setColor( QPalette::BrightText,      white );
                lightPalette.setColor( QPalette::Light,           white );
                lightPalette.setColor( QPalette::Midlight,        gray202 );
                lightPalette.setColor( QPalette::Dark,            gray159 );
                lightPalette.setColor( QPalette::Mid,             gray184 );
                lightPalette.setColor( QPalette::Shadow,          gray118 );
                lightPalette.setColor( QPalette::Highlight,       highlightColor );
                lightPalette.setColor( QPalette::HighlightedText, white );
                lightPalette.setColor( QPalette::Link,            linkColor );
                lightPalette.setColor( QPalette::LinkVisited,     linkVisitedColor );

                lightPalette.setColor( QPalette::Disabled, QPalette::WindowText, gray190 );
                lightPalette.setColor( QPalette::Disabled, QPalette::Base,       gray239 );
                lightPalette.setColor( QPalette::Disabled, QPalette::Dark,       gray190 );
                lightPalette.setColor( QPalette::Disabled, QPalette::Text,       gray190 );
                lightPalette.setColor( QPalette::Disabled, QPalette::ButtonText, gray190 );
                lightPalette.setColor( QPalette::Disabled, QPalette::Shadow,     gray177 );
                lightPalette.setColor( QPalette::Disabled, QPalette::Highlight,  gray145 );

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

                QColor white( 255, 255, 255 );
                QColor gray127( 127, 127, 127 );
                QColor gray80( 80, 80, 80 );
                QColor gray66( 66, 66, 66 );
                QColor gray53( 53, 53, 53 );
                QColor gray42( 42, 42, 42 );
                QColor gray35( 35, 35, 35 );
                QColor gray20( 20, 20, 20 );
                QColor black( 0, 0, 0 );

                QColor toolTipColor( 0, 0, 0 );
                QColor highlightColor( 42, 130, 218 );
                QColor linkColor( 42, 130, 218 );
                QColor linkVisitedColor( 255, 0, 255 ); //??
                QColor brightTextColor( 255, 0, 0 );

                QPalette darkPalette;

                darkPalette.setColor( QPalette::Window,          gray53 );
                darkPalette.setColor( QPalette::WindowText,      white );
                darkPalette.setColor( QPalette::Base,            gray42 );
                darkPalette.setColor( QPalette::AlternateBase,   gray66 );
                darkPalette.setColor( QPalette::ToolTipBase,     toolTipColor );
                darkPalette.setColor( QPalette::ToolTipText,     white );
                darkPalette.setColor( QPalette::PlaceholderText, white ); //??
                darkPalette.setColor( QPalette::Text,            white );
                darkPalette.setColor( QPalette::Button,          gray53 );
                darkPalette.setColor( QPalette::ButtonText,      white );
                darkPalette.setColor( QPalette::BrightText,      brightTextColor );
                darkPalette.setColor( QPalette::Light,           gray80 ); //??
                darkPalette.setColor( QPalette::Midlight,        gray66 ); //??
                darkPalette.setColor( QPalette::Dark,            gray35 );
                darkPalette.setColor( QPalette::Mid,             gray42); //??
                darkPalette.setColor( QPalette::Shadow,          gray20 );
                darkPalette.setColor( QPalette::Highlight,       highlightColor );
                darkPalette.setColor( QPalette::HighlightedText, white );
                darkPalette.setColor( QPalette::Link,            linkColor );
                darkPalette.setColor( QPalette::LinkVisited,     linkVisitedColor );

                darkPalette.setColor( QPalette::Disabled, QPalette::WindowText, gray127 );
                darkPalette.setColor( QPalette::Disabled, QPalette::Base,       gray20 ); //??
                darkPalette.setColor( QPalette::Disabled, QPalette::Dark,       gray127 ); //??
                darkPalette.setColor( QPalette::Disabled, QPalette::Text,       gray127 );
                darkPalette.setColor( QPalette::Disabled, QPalette::ButtonText, gray127 );
                darkPalette.setColor( QPalette::Disabled, QPalette::Shadow,     gray35 ); //??
                darkPalette.setColor( QPalette::Disabled, QPalette::Highlight,  gray80 );

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
