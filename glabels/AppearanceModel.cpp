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
#include <QStyleHints>


namespace
{
	//
	// Fusion light palette
	//
	QPalette fusionLightPalette()
	{
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

		return p;
	}


	//
	// Fusion dark palette
	//
	QPalette fusionDarkPalette()
	{
		QPalette p;

		p.setColor( QPalette::Window,          QColor(  50,  50,  50 ) );
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

		return p;
	}
}


namespace glabels
{

        //
        // Static data
        //
        std::unique_ptr<AppearanceModel> AppearanceModel::mInstance;
        model::Settings::ColorScheme AppearanceModel::mColorScheme = model::Settings::LIGHT_COLOR_SCHEME;


        //
        // Constructor
        //
        AppearanceModel::AppearanceModel()
        {
#if QT_VERSION >= QT_VERSION_CHECK(6,8,0)
                auto styleName = QApplication::style()->name();
                if ( ( styleName.compare( "windows", Qt::CaseInsensitive ) != 0 ) &&
                     ( styleName.compare( "macos",   Qt::CaseInsensitive ) != 0 ) &&
                     ( styleName.compare( "fusion",  Qt::CaseInsensitive ) != 0 ) )
                {
	                // Default to and hardcode to "fusion", if not one of the above
	                QApplication::setStyle( QStyleFactory::create( "Fusion" ) );
                }
#else
                // Hardcode style to "fusion"
                QApplication::setStyle( QStyleFactory::create( "Fusion" ) );
#endif

                setColorScheme( model::Settings::colorScheme() );

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
                switch ( mColorScheme )
                {
                case model::Settings::LIGHT_COLOR_SCHEME: return STYLE_LIGHT;
                case model::Settings::DARK_COLOR_SCHEME:  return STYLE_DARK;
                case model::Settings::SYSTEM_COLOR_SCHEME:
	                qWarning() << "System color scheme not yet supported.";
                        return STYLE_LIGHT;
                default:
                        qWarning() << "Unknown color scheme: " << mColorScheme;
                        return STYLE_LIGHT;
                }
        }


        //
        // Do we support SYSTEM_COLOR_SCHEME?
        //
        bool AppearanceModel::supportsSystemColorScheme()
        {
	        // FIXME: Currently do not support a system color scheme.
	        return false;
        }


        //
        // Set Color Scheme
        //
        void AppearanceModel::setColorScheme( model::Settings::ColorScheme scheme )
        {
                switch ( scheme )
                {
                case model::Settings::LIGHT_COLOR_SCHEME:  setLightColorScheme();  break;
                case model::Settings::DARK_COLOR_SCHEME:   setDarkColorScheme();   break;
                case model::Settings::SYSTEM_COLOR_SCHEME: setSystemColorScheme(); break;
                default:
                        qWarning() << "Unknown color scheme: " << scheme;
                        break;
                }
        }


        //
        // Set Light Color Scheme
        //
        void AppearanceModel::setLightColorScheme()
        {
                mColorScheme = model::Settings::LIGHT_COLOR_SCHEME;

                QIcon::setThemeName( "glabels-flat" );

                auto styleName = QApplication::style()->name();
                if ( styleName.compare( "fusion", Qt::CaseInsensitive ) == 0 )
                {
	                QApplication::setPalette( fusionLightPalette() );
                }
#if QT_VERSION >= QT_VERSION_CHECK(6,8,0)
                else
                {
	                QGuiApplication::styleHints()->setColorScheme( Qt::ColorScheme::Light );
                }
#endif

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
        // Set Dark Color Scheme
        //
        void AppearanceModel::setDarkColorScheme( )
        {
                mColorScheme = model::Settings::DARK_COLOR_SCHEME;

                QIcon::setThemeName( "glabels-flat-dark"  );

                auto styleName = QApplication::style()->name();
                if ( styleName.compare( "fusion", Qt::CaseInsensitive ) == 0 )
                {
	                QApplication::setPalette( fusionDarkPalette() );
                }
#if QT_VERSION >= QT_VERSION_CHECK(6,8,0)
                else
                {
	                QGuiApplication::styleHints()->setColorScheme( Qt::ColorScheme::Dark );
                }
#endif

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
        // Set System Color Scheme
        //
        void AppearanceModel::setSystemColorScheme( )
        {
	        qWarning() << "System color scheme not yet supported.";
        }


        //
        // Handle Settings "changed" signal
        //
        void AppearanceModel::onSettingsChanged()
        {

                auto newScheme = model::Settings::colorScheme();
                if ( newScheme != mColorScheme )
                {
                        setColorScheme( newScheme );
                }
        }

}
