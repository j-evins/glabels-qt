//  Variable.hpp
//
//  Copyright (C) 2019  Jaye Evins <evins@snaught.com>
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

#ifndef model_Variable_hpp
#define model_Variable_hpp


#include <QCoreApplication>
#include <QString>


namespace glabels::model
{

        class Variable
        {
                Q_DECLARE_TR_FUNCTIONS(Variable)

        public:
                enum class Type
                {
                        STRING,
                        INTEGER,
                        FLOATING_POINT,
                        COLOR
                };

                enum class Increment
                {
                        NEVER,
                        PER_ITEM,
                        PER_COPY,
                        PER_PAGE
                };


        public:
                Variable();

                Variable( Type            type,
                          const QString&  name,
                          const QString&  initialValue,
                          Increment       increment = Increment::NEVER,
                          const QString&  stepSize = "0" );

                virtual ~Variable() = default;


                Type      type() const;
                QString   name() const;
                QString   initialValue() const;
                Increment increment() const;
                QString   stepSize() const;

                void setInitialValue( const QString& value );

                void    resetValue();
                void    incrementValueOnItem();
                void    incrementValueOnCopy();
                void    incrementValueOnPage();
                QString value() const;

                static QString   typeToI18nString( Type type );
                static QString   typeToIdString( Type type );
                static Type      idStringToType( const QString& string );

                static QString   incrementToI18nString( Increment increment );
                static QString   incrementToIdString( Increment increment );
                static Increment idStringToIncrement( const QString& string );


        private:
                Type      mType;
                QString   mName;
                QString   mInitialValue;
                Increment mIncrement;
                QString   mStepSize;

                long long mIntegerValue;
                long long mIntegerStep;
                double    mFloatingPointValue;
                double    mFloatingPointStep;

        };

}


#endif // model_Variable_hpp
