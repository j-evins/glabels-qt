//  ColorHistory.hpp
//
//  Copyright (C) 2014-2016  Jaye Evins <evins@snaught.com>
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

#ifndef ColorHistory_hpp
#define ColorHistory_hpp


#include <QColor>
#include <QObject>


namespace glabels
{

        ///
        /// Color History
        ///
        class ColorHistory : public QObject
        {
                Q_OBJECT

        public:
                static const int MAX_COLORS = 9;

                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        private:
                ColorHistory() = default;

        public:
                static ColorHistory* instance();


                /////////////////////////////////
                // Signals
                /////////////////////////////////
        signals:
                void changed();


                /////////////////////////////////
                // Public Methods
                /////////////////////////////////
        public:
                void addColor( const QColor& color, const QString& name );
                QList<QColor> getColors();
                QStringList getNames();


                /////////////////////////////////
                // Private Methods
                /////////////////////////////////
        private:
                QStringList readNameColorList();
                void writeNameColorList( const QStringList& nameColorList );


                /////////////////////////////////
                // Private Members
                /////////////////////////////////
        private:

        };

}


#endif // ColorHistory_hpp
