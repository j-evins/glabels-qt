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


namespace glabels::merge
{

        ///
        /// Extract raw record
        ///
        QByteArrayList VCardSimpleParser::extractRawRecord( QIODevice& file )
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
                                if ( line.toLower().startsWith( "end:vcard" ) )
                                {
                                        foundEnd = true;
                                }
                                else if ( line.toLower().startsWith( "begin:vcard" ) )
                                {
                                        // Unexpected "begin" before "end", vcard is malformed
                                        break;
                                }
                        }
                        else
                        {
                                if ( line.toLower().startsWith( "begin:vcard" ) )
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
        /// Parse record
        ///
        Record VCardSimpleParser::parseRawRecord( const QByteArrayList& buffer )
        {
                Record record;

                return record;
        }

}
