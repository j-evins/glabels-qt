//  ParserState.hpp
//
//  Copyright (C) 2025-2026  Jaye Evins <evins@snaught.com>
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

#ifndef model_ParserState_hpp
#define model_ParserState_hpp


#include <QString>


namespace glabels::model
{

        class ParserState
        {
        public:
                ParserState() = default;
                ParserState( const QString& string,
                             qsizetype      pos = 0 );
                ~ParserState() = default;

                QChar operator[]( qsizetype i ) const;
                bool isNextSubString( const QString& s ) const;
                qsizetype pos() const;
                qsizetype charsLeft() const;

                void advanceChars( qsizetype i );


        private:
                const QString* mString{ nullptr };
                qsizetype      mPos{ 0 };
        };

}


#endif // model_ParserState_hpp
