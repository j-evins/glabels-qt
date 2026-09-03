//  Merge/RawVCard.hpp
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

#ifndef merge_RawVCard_hpp
#define merge_RawVCard_hpp


#include "Record.hpp"

#include <QByteArrayList>
#include <QIODevice>


namespace glabels::merge::RawVCard
{
        //
        // Parameter tokens
        //
        struct ParamTokens
        {
                QByteArray     name;
                QByteArrayList values;
        };


        //
        // Line tokens
        //
        struct LineTokens
        {
                bool               valid{ false };
                QByteArray         group;
                QByteArray         name;
                QList<ParamTokens> params;
                QByteArray         value;
                QByteArrayList     values;  // Split out version of value, for list and structured types
        };


        QByteArrayList readNextRawVCard( QIODevice& file );

        LineTokens tokenizeLine( const QByteArray& line );
        bool paramsHas( const QList<ParamTokens>& params, const QString& name, const QString& value );
}


#endif // merge_RawVCard_hpp
