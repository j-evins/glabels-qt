//  FileUtil.cpp
//
//  Copyright (C) 2015  Jaye Evins <evins@snaught.com>
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


#include "FileUtil.hpp"

#include "Config.hpp"

#include <QApplication>
#include <QStandardPaths>


namespace glabels::model
{

        QString FileUtil::addExtension( const QString& rawFilename, const QString& extension )
        {
                if ( rawFilename.endsWith( extension ) )
                {
                        return rawFilename;
                }

                return rawFilename + extension;
        }


        QDir FileUtil::systemTemplatesDir()
        {
                QDir dir;

                // First, try finding templates directory relative to application path
                dir.cd( QApplication::applicationDirPath() );
                if ( (dir.dirName() == "bin") &&
                     dir.cdUp() && dir.cd( "share" ) && dir.cd( "glabels-qt" ) && dir.cd( "templates" ) )
                {
                        return dir;
                }

                // Next, try running out of the source directory.
                if ( dir.cd( Config::PROJECT_SOURCE_DIR ) && dir.cd( "templates" ) )
                {
                        return dir;
                }

                qFatal( "Cannot locate system template directory!" );
                return QDir("/");
        }


        QDir FileUtil::manualUserTemplatesDir()
        {
                // Location for manually created user-defined templates
                QDir dir( QStandardPaths::writableLocation(QStandardPaths::HomeLocation) );
                dir.mkpath( ".glabels" );
                dir.cd( ".glabels" );

                return dir;
        }


        QDir FileUtil::userTemplatesDir()
        {
                // Location for user-defined templates created using TemplateDesigner
                QDir dir( QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) );
                dir.mkpath( "." );

                return dir;
        }


        QDir FileUtil::translationsDir()
        {
                QDir dir;

                // First, try finding translations directory relative to application path
                dir.cd( QApplication::applicationDirPath() );
                if ( (dir.dirName() == "bin") &&
                     dir.cdUp() && dir.cd( "share" ) && dir.cd( "glabels-qt" ) && dir.cd( "translations" ) )
                {
                        return dir;
                }

                // Next, try running out of the source directory.
                if ( dir.cd( Config::PROJECT_BUILD_DIR ) && dir.cd( "translations" ) )
                {
                        return dir;
                }

                qFatal( "Cannot locate system template directory!" );
                return QDir("/");
        }


        QString FileUtil::makeRelativeIfInDir( const QDir&    dir,
                                               const QString& filename )
        {
                QString relativeFilePath = dir.relativeFilePath( filename ); // Note: directory separators canonicalized to slash by Qt path methods
                if ( !relativeFilePath.startsWith( "../" ) )
                {
                        return relativeFilePath;
                }
                return filename;
        }

}
