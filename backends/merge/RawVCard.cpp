//  Merge/RawVCard.cpp
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


#include "RawVCard.hpp"

#include <QDebug>


namespace
{

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

}


namespace glabels::merge
{

        ///
        /// Read raw vCard
        ///
        QByteArrayList
        RawVCard::readNextRawVCard( QIODevice& file )
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

                        if ( !foundBegin && ( ( line.toUpper().trimmed() == "BEGIN:VCARD" ) ||
                                              ( line.toUpper().trimmed().endsWith( ".BEGIN:VCARD" ) ) ) )
                        {
                                foundBegin = true;
                        }
                        else if ( foundBegin && ( ( line.toUpper().trimmed() == "END:VCARD" ) ||
                                                  ( line.toUpper().trimmed().endsWith( ".END:VCARD" ) ) ) )
                        {
                                foundEnd = true;
                        }
                        else if ( foundBegin )
                        {
                                if ( ( line.toUpper().trimmed() == "BEGIN:VCARD" ) ||
                                     ( line.toUpper().trimmed().endsWith( ".BEGIN:VCARD" ) ) )
                                {
                                        // Unexpected "begin" before "end", vcard is malformed
                                        return QByteArrayList(); // empty
                                }

                                // unfold folded lines
                                if ( ( (line[0] == ' ') || (line[0] == '\t') ) && !buffer.isEmpty() )
                                {
                                        auto& lastLine = buffer.last();
                                        lastLine.append( line.trimmed() );
                                }
                                else
                                {

                                        buffer.append( line.trimmed() );
                                }
                        }
                }

                if ( !foundBegin || !foundEnd )
                {
                        return QByteArrayList(); // Empty.  No record found or it's malformed (possibly truncated)
                }

                return buffer;
        }


        //
        // Tokenize line
        //
        RawVCard::LineTokens
        RawVCard::tokenizeLine( const QByteArray& line )
        {
                //
                // line = [ group "." ] name [ ";" params ] ":" value { ";" value }
                // params = param-name "=" param-value { "," param-value } { ";" "param-name "=" param-value }
                //
                // TODO: possibly replace with a more robust parser.  Currently just looks for delimeters hierarchically,
                //       but does no other validation of tokens.
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


        //
        // Do the parameters have a parameter with given name and value?
        //
        bool RawVCard::paramsHas( const QList<ParamTokens>& params, const QString& name, const QString& value )
        {
                for ( auto& p : params )
                {
                        if ( p.name == name )
                        {
                                for ( auto& v : p.values )
                                {
                                        if ( v == value ) return true;
                                }
                        }
                }

                return false;
        }

}
