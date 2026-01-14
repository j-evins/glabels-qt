//  FileUtil.hpp
//
//  Copyright (C) 2015-2026  Jaye Evins <evins@snaught.com>
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

#ifndef model_FileUtil_hpp
#define model_FileUtil_hpp


#include <QString>
#include <QDir>


namespace glabels::model
{

        namespace FileUtil
        {

                QString addExtension( const QString& rawFilename, const QString& extension );

                QDir systemTemplatesDir();
                QDir manualUserTemplatesDir();
                QDir userTemplatesDir();

                QDir translationsDir();

                QString makeRelativeIfInDir( const QDir&    dir,
                                             const QString& filename );
        }

}


#endif // model_FileUtil_hpp
