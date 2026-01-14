//  main.cpp
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


#include "MainWindow.hpp"

#include "model/Db.hpp"
#include "model/FileUtil.hpp"
#include "model/Model.hpp"
#include "model/Settings.hpp"
#include "model/Version.hpp"
#include "model/XmlLabelParser.hpp"

#include "barcode/Backends.hpp"
#include "merge/Factory.hpp"

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QIcon>
#include <QLibraryInfo>
#include <QLocale>
#include <QTextStream>
#include <QTranslator>


int main( int argc, char **argv )
{
        QApplication app( argc, argv );

        QCoreApplication::setOrganizationName( glabels::model::Version::ORGANIZATION_NAME );
        QCoreApplication::setOrganizationDomain( glabels::model::Version::ORGANIZATION_DOMAIN );
        QCoreApplication::setApplicationName( glabels::model::Version::APPLICATION_NAME );
        QCoreApplication::setApplicationVersion( glabels::model::Version::LONG_STRING );
        QApplication::setDesktopFileName( glabels::model::Version::DESKTOP_FILE_NAME );

        QIcon::setThemeName( "glabels-flat" );

        //
        // Setup translators
        //
        QLocale locale = QLocale::system();
        QString qtTranslationsDir = QLibraryInfo::path( QLibraryInfo::TranslationsPath );
        QString myTranslationsDir = glabels::model::FileUtil::translationsDir().canonicalPath();

        QTranslator qtTranslator;
        if ( qtTranslator.load( locale, "qt", "_", qtTranslationsDir ) )
        {
                app.installTranslator(&qtTranslator);
        }

        QTranslator glabelsTranslator;
        if ( glabelsTranslator.load( locale, "glabels", "_", myTranslationsDir ) )
        {
                app.installTranslator(&glabelsTranslator);
        }

        QTranslator templatesTranslator;
        if ( templatesTranslator.load( locale, "templates", "_", myTranslationsDir ) )
        {
                app.installTranslator(&templatesTranslator);
        }


        //
        // Parse command line
        //
        QCommandLineParser parser;
        parser.setApplicationDescription( QCoreApplication::translate( "main", "gLabels Label Designer" ) );
        parser.addHelpOption();
        parser.addVersionOption();
        parser.addOption( { { "V", "Version" }, QCoreApplication::translate( "main", "More detailed version information." ) } );
        parser.addPositionalArgument( "files",
                                      QCoreApplication::translate( "main", "gLabels project files to open, optionally." ),
                                      "[files...]" );
        parser.process( app );

        // Handle verbose version option
        if ( parser.isSet( "Version" ) )
        {
                QTextStream(stdout) << glabels::model::Version::details() << Qt::endl;
                return 0;
        }


        //
        // Initialize subsystems
        //
        glabels::model::Settings::init();
        glabels::model::Db::init();
        glabels::merge::Factory::init();
        glabels::barcode::Backends::init();


        //
        // Open each file in its own main window
        //
        bool openedFiles = false;
        for ( QString filename : parser.positionalArguments() )
        {
                glabels::model::Model *model = glabels::model::XmlLabelParser::readFile( filename );
                if ( model )
                {
                        auto *newWindow = new glabels::MainWindow();
                        newWindow->setModel( model );
                        newWindow->show();
                        openedFiles = true;

                        glabels::model::Settings::addToRecentFileList( model->fileName() );
                }
        }


        //
        // Launch main window
        //
        if ( !openedFiles )
        {
                auto *mainWindow = new glabels::MainWindow();
                mainWindow->show();
        }


        return app.exec();
}
