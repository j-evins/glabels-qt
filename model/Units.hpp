//  Units.hpp
//
//  Copyright (C) 2016-2026  Jaye Evins <evins@snaught.com>
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

#ifndef model_Units_hpp
#define model_Units_hpp


#include <QCoreApplication>
#include <QString>


namespace glabels::model
{


        class Units
        {
                Q_DECLARE_TR_FUNCTIONS(Units)

        public:
                enum Enum { PT, IN, MM, CM, PC };

                Units() = default;
                Units( Enum enumValue );
                Units( const QString& idString );

                static Units pt();
                static Units in();
                static Units mm();
                static Units cm();
                static Units pc();

                Enum toEnum() const;

                QString toIdString() const;
                QString toTrName() const;

                double resolution() const;
                int resolutionDigits() const;

                static bool isIdValid( const QString& unitsId );


        private:
                Enum mEnumValue{ PT };

        };

}


#endif // model_Units_hpp
