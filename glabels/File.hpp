//  File.hpp
//
//  Copyright (C) 2017-2026  Jaye Evins <evins@snaught.com>
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

#ifndef File_hpp
#define File_hpp


#include <QObject>


namespace glabels
{

        // Forward References
        class MainWindow;


        ///
        /// File Actions
        ///
        /// Note: class provides a translation context for these static functions.
        ///
        class File : public QObject
        {
                Q_OBJECT

        public:
                static bool newLabel( MainWindow *window = nullptr );
                static void open( MainWindow *window );
                static void open( const QString& fileName, MainWindow *window );
                static bool save( MainWindow *window );
                static bool saveAs( MainWindow *window );
                static void templateDesigner( MainWindow *window );
                static void close( MainWindow *window );
                static void exit();

        private:
                static QString mCwd;

        };

}


#endif // File_hpp
