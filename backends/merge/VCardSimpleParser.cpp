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


        //
        // Key-value pair
        //
        struct KeyValuePair
        {
                QString key;
                QString value;
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
                // line = [ group "." ] name [ ";" params ] ":" value { ";" value }
                // params = param-name "=" param-value { "," param-value } { ";" "param-name "=" param-value }
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

                const auto& [nameWithGroupWithParams, value] = split2First( line, ':' );

                const auto& [nameWithGroup, params] = split2First( nameWithGroupWithParams, ';' );

                const auto& [group, name] = split2Second( nameWithGroup, '.' );

                LineTokens tokens;
                tokens.valid  = true;
                tokens.group  = group;
                tokens.name   = name;

                ParamTokens paramTokens;
                for ( auto& param : params.split( ';' ) )
                {
                        const auto& [paramName, paramValues] = split2None( param, '=' );
                        if ( !paramName.isEmpty() )
                        {
                                paramTokens.name = paramName;
                                paramTokens.values = paramValues.split( ',' );

                                tokens.params.append( paramTokens );
                        }
                }

                tokens.value  = value;
                tokens.values = value.split( ';' );

                return tokens;
        }


        KeyValuePair parseTokens( const LineTokens& tokens )
        {
                //
                // Types not parsed into key-value pairs
                //
                //    - BEGIN
                //    - END
                //    - VERSION
                //    - NAME
                //    - PROFILE
                //    - SOURCE
                //    - PHOTO
                //    - LOGO
                //    - SOUND
                //    - KEY
                //    - AGENT

                //
                // Simple text types, no/ignore parameters, single text value
                //
                if ( ( tokens.name == "FN" )          ||
                     ( tokens.name == "MAILER" )      ||
                     ( tokens.name == "TITLE" )       ||
                     ( tokens.name == "ROLE" )        ||
                     ( tokens.name == "NOTE" )        ||
                     ( tokens.name == "PRODID" )      ||
                     ( tokens.name == "SORT-STRING" ) ||
                     ( tokens.name == "N" )           ||
                     ( tokens.name == "NICKNAME" )    ||
                     ( tokens.name == "UID" )         ||
                     ( tokens.name == "URL" )         ||
                     ( tokens.name == "TZ" )          ||
                     ( tokens.name == "ORG" )         ||
                     ( tokens.name == "BDAY" )        ||
                     ( tokens.name == "GEO" )         ||
                     ( tokens.name == "CATEGORIES" )  ||
                     ( tokens.name == "REV" )         ||
                     ( tokens.name == "CLASS" )       ||
                     ( tokens.name.startsWith( "X-" ) ) )
                {
                        return { tokens.name, tokens.value };
                }

                //
                // Email/Adress/Lable
                //
                if ( ( tokens.name == "EMAIL" ) ||
                     ( tokens.name == "TEL" )   ||
                     ( tokens.name == "ADR" )   ||
                     ( tokens.name == "LABEL" ) )
                {
                        QString qName = tokens.name;
                        for ( auto& paramTokens : tokens.params )
                        {
                                if ( paramTokens.name == "TYPE" )
                                {
                                        for ( auto& value : paramTokens.values )
                                        {
                                                qName += "_" + value.toUpper();
                                        }
                                }
                        }

                        return { qName, tokens.value };
                }

                return { "", "" };
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
                                if ( ( line.toUpper().startsWith( "END:VCARD" ) ) ||
                                     ( line.toUpper().contains( ".END:VCARD" ) ) )
                                {
                                        foundEnd = true;
                                }
                                else if ( ( line.toUpper().startsWith( "BEGIN:VCARD" ) ) ||
                                          ( line.toUpper().contains( ".BEGIN:VCARD" ) ) )
                                {
                                        // Unexpected "begin" before "end", vcard is malformed
                                        break;
                                }
                        }
                        else
                        {
                                if ( ( line.toUpper().startsWith( "BEGIN:VCARD" ) ) ||
                                     ( line.toUpper().contains( ".BEGIN:VCARD" ) ) )
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

                int i = 0;
                for ( auto& line : buffer )
                {
                        auto tokens = tokenizeLine( line );

#if 0
                        qDebug() << "----------------------------------------";

                        qDebug() << "valid = " << tokens.valid;
                        qDebug() << "group= " << tokens.group;
                        qDebug() << "name = " << tokens.name;
                        for ( auto& paramTokens : tokens.params )
                        {
                                qDebug() << "param = " << paramTokens.name;
                                qDebug() << "    paramValues = " << paramTokens.values;
                        }
                        qDebug() << "values = " << tokens.values;

                        qDebug() << "----------------------------------------";
#endif

                        auto pair = parseTokens( tokens );
                        if ( !pair.key.isEmpty() )
                        {
                                qDebug() << i << ". " << pair.key << " = " << pair.value;
                        }
                        else
                        {
                                qDebug() << i << ".";
                        }
                        i++;

                }

                return record;
        }

}
