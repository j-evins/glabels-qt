//  Variables.hpp
//
//  Copyright (C) 2013-2016  Jaye Evins <evins@snaught.com>
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

#ifndef model_Variables_hpp
#define model_Variables_hpp


#include "Variable.hpp"

#include <QMap>
#include <QObject>
#include <QString>


namespace glabels::model
{

        ///
        /// Variables Collection
        ///
        class Variables : public QObject, public QMap<QString,Variable>
        {
                Q_OBJECT

                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        public:
                Variables() = default;
                Variables( const Variables& other );


                /////////////////////////////////
                // Copy from other object
                /////////////////////////////////
                void copy( const Variables& other );


                /////////////////////////////////
                // Methods
                /////////////////////////////////
                bool hasVariable( const QString& name ) const;
                void addVariable( const Variable& variable );
                void deleteVariable( const QString& name );
                void replaceVariable( const QString& name, const Variable& variable );

                void setVariables( const QMap<QString,QString>& definitions );

                void resetVariables();
                void resetOnCopyVariables();
                void incrementVariablesOnItem();
                void incrementVariablesOnCopy();
                void incrementVariablesOnPage();


                /////////////////////////////////
                // Signals
                /////////////////////////////////
        signals:
                void changed();


                /////////////////////////////////
                // Private data
                /////////////////////////////////
        private:

        };

}


#endif // model_Variables_hpp
