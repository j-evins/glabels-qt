//  Merge/VCardSimpleParser.cpp
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


#include "VCardSimpleParser.hpp"

#include <QDebug>

#include <utility>


namespace
{

        //
        // Line tokens
        //
        struct LineTokens
        {
                bool           valid{ false };
                QByteArray     group;
                QByteArray     name;
                QByteArray     paramName;
                QByteArrayList paramValues;
                QByteArrayList values;
        };


        ///
        /// Split into two byte arrays at first occurrence of delimiter, default into first
        ///
        std::pair<QByteArray,QByteArray> split2First( const QByteArray& in, char delim )
        {
                std::pair<QByteArray,QByteArray> out;
                auto iDelim = in.indexOf( delim );

                if ( iDelim != -1 )
                {
                        out.first = in.sliced( 0, iDelim );
                        out.second = in.sliced( iDelim+1 );
                }
                else
                {
                        out.first = in;
                }

                return out;
        }


        ///
        /// Split into two byte arrays at first occurrence of delimiter, default into second
        ///
        std::pair<QByteArray,QByteArray> split2Second( const QByteArray& in, char delim )
        {
                std::pair<QByteArray,QByteArray> out;
                auto iDelim = in.indexOf( delim );

                if ( iDelim != -1 )
                {
                        out.first = in.sliced( 0, iDelim );
                        out.second = in.sliced( iDelim+1 );
                }
                else
                {
                        out.second = in;
                }

                return out;
        }


        ///
        /// Split into two byte arrays at first occurrence of delimiter, default to empty values
        ///
        std::pair<QByteArray,QByteArray> split2None( const QByteArray& in, char delim )
        {
                std::pair<QByteArray,QByteArray> out;
                auto iDelim = in.indexOf( delim );

                if ( iDelim != -1 )
                {
                        out.first = in.sliced( 0, iDelim );
                        out.second = in.sliced( iDelim+1 );
                }

                return out;
        }


        LineTokens tokenizeLine( const QByteArray& line )
        {
                //
                // line = [ group "." ] name [ ";" param-name "=" param-value { "," param-value } ":" value { ";" value }
                //
                // TODO: possibly replace with a more robust parser.  Currently just looks for delimeters, but does no
                //       other validation of tokens.
                //
                // REFERENCES:
                //   [1] RFC2426
                //
                if ( !line.contains( ':' ) )
                {
                        return LineTokens();
                }

                const auto& [nameWithGroupWithParam, values] = split2First( line, ':' );

                const auto& [nameWithGroup, param] = split2First( nameWithGroupWithParam, ';' );

                const auto& [group, name] = split2Second( nameWithGroup, '.' );

                const auto& [paramName, paramValues] = split2None( param, '=' );

                LineTokens tokens;
                tokens.valid         = true;
                tokens.group         = group;
                tokens.name          = name;
                tokens.paramName     = paramName;
                tokens.paramValues   = paramValues.split( ',' );
                tokens.values        = values.split( ';' );

                return tokens;
        }

}


namespace glabels::merge
{

        ///
        /// Extract raw vCard
        ///
        QByteArrayList VCardSimpleParser::extractRawVCard( QIODevice& file )
        {
                QByteArrayList buffer;

                bool foundBegin = false;
                bool foundEnd = false;

                while ( !foundEnd )
                {
                        auto line = file.readLine();
                        if ( line.isEmpty() )
                        {
                                break;
                        }

                        if ( foundBegin )
                        {
                                if ( line.toUpper().contains( "END:VCARD" ) )
                                {
                                        foundEnd = true;
                                }
                                else if ( line.toUpper().contains( "BEGIN:VCARD" ) )
                                {
                                        // Unexpected "begin" before "end", vcard is malformed
                                        break;
                                }
                        }
                        else
                        {
                                if ( line.toUpper().contains( "BEGIN:VCARD" ) )
                                {
                                        foundBegin = true;
                                }
                                else
                                {
                                        continue; // skip lines not in vcard
                                }
                        }

                        if ( ( (line[0] == ' ') || (line[0] == '\t') ) && !buffer.isEmpty() )
                        {
                                // Folded line
                                auto& lastLine = buffer.last();
                                lastLine.append( line.trimmed() );
                        }
                        else
                        {

                                buffer.append( line.trimmed() );
                        }
                }

                if ( !foundBegin || !foundEnd )
                {
                        return QByteArrayList(); // Empty.  No record found or it's malformed (possibly truncated)
                }

                return buffer;
        }


        ///
        /// Parse raw vCard
        ///
        Record VCardSimpleParser::parseRawVCard( const QByteArrayList& buffer )
        {
                Record record;

                qDebug() << "";
                qDebug() << "#################################################";

                for ( auto& line : buffer )
                {
                        auto tokens = tokenizeLine( line );

                        qDebug() << "----------------------------------------";
                        qDebug() << "valid = " << tokens.valid;
                        qDebug() << "group= " << tokens.group;
                        qDebug() << "name = " << tokens.name;
                        qDebug() << "param = " << tokens.paramName;
                        qDebug() << "paramValues = " << tokens.paramValues;
                        qDebug() << "values = " << tokens.values;
                        qDebug() << "----------------------------------------";
                }

                return record;
        }

}
