//  Merge/Text.cpp
//
//  Copyright (C) 2016  Jaye Evins <evins@snaught.com>
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


#include "Text.hpp"

#include "Record.hpp"

#include <QDebug>


namespace glabels::merge
{

        ///
        /// Constructor
        ///
        Text::Text( QChar delimiter, bool line1HasKeys )
                : Merge(),
                  mDelimeter(delimiter),
                  mLine1HasKeys(line1HasKeys),
                  mNFieldsMax(0)
        {
        }


        ///
        /// Constructor
        ///
        Text::Text( const Text* merge )
                : Merge( merge ),
                  mDelimeter(merge->mDelimeter),
                  mLine1HasKeys(merge->mLine1HasKeys),
                  mKeys(merge->mKeys),
                  mNFieldsMax(merge->mNFieldsMax)
        {
        }


        ///
        /// Get key list
        ///
        QStringList Text::keys() const
        {
                QStringList keys;
                for ( int iField = 0; iField < mNFieldsMax; iField++ )
                {
                        keys << keyFromIndex(iField);
                }
                return keys;
        }


        ///
        /// Get primary key
        ///
        QString Text::primaryKey() const
        {
                return keyFromIndex(0);
        }


        ///
        /// Open source
        ///
        void Text::open()
        {
                mKeys.clear();
                mNFieldsMax = 0;

                mFile.setFileName( source() );
                if (mFile.open( QIODevice::ReadOnly|QIODevice::Text ))
                {
                        if ( mLine1HasKeys )
                        {
                                mKeys = parseLine();
                                if ( (mKeys.size() == 1) && (mKeys[0] == "") )
                                {
                                        mKeys.clear();
                                }
                                else
                                {
                                        mNFieldsMax = mKeys.size();
                                }
                        }
                }
        }


        ///
        /// Close source
        ///
        void Text::close()
        {
                if ( mFile.isOpen() )
                {
                        mFile.close();
                }
        }


        ///
        /// Read next record
        ///
        Record Text::readNextRecord()
        {
                QStringList values = parseLine();
                if ( !values.isEmpty() )
                {
                        Record record;

                        int iField = 0;
                        for ( const auto& value : values )
                        {
                                record[ keyFromIndex(iField) ] = value;
                                iField++;
                        }
                        mNFieldsMax = std::max( mNFieldsMax, iField );

                        return record;
                }

                return NullRecord();
        }


        ///
        /// Key from field index
        ///
        QString Text::keyFromIndex( int iField ) const
        {
                if ( mLine1HasKeys && ( iField < mKeys.size() ) )
                {
                        return mKeys[iField];
                }
                else
                {
                        return QString::number( iField+1 );
                }
        }


        ///
        /// Parse line.
        ///
        /// Attempt to be a robust parser of various CSV (and similar) formats.
        ///
        /// Based on CSV format described in RFC 4180 section 2.
        ///
        /// Additions to RFC 4180 rules:
        ///   - delimeters and other special characters may be "escaped" by a leading
        ///     backslash (\)
        ///   - C escape sequences for newline (\n) and tab (\t) are also translated.
        ///   - if quoted text is not followed by a delimeter, any additional text is
        ///     concatenated with quoted portion.
        ///
        /// Returns a list of fields.  A blank line is considered a line with one
        /// empty field.  Returns an empty list when done.
        ///
        QStringList Text::parseLine()
        {
                QStringList fields;

                enum State
                        {
                                DELIM, QUOTED, QUOTED_QUOTE1, QUOTED_ESCAPED, SIMPLE, SIMPLE_ESCAPED, DONE
                        } state = DELIM;

                QByteArray field;

                while ( state != DONE )
                {
                        char c;
                        if ( mFile.getChar( &c ) )
                        {
                                switch (state)
                                {

                                case DELIM:
                                        switch (c)
                                        {
                                        case '\n':
                                                /* last field is empty. */
                                                fields << "";
                                                state = DONE;
                                                break;
                                        case '\r':
                                                /* ignore */
                                                state = DELIM;
                                                break;
                                        case '"':
                                                /* start a quoted field. */
                                                state = QUOTED;
                                                break;
                                        case '\\':
                                                /* simple field, but 1st character is an escape. */
                                                state = SIMPLE_ESCAPED;
                                                break;
                                        default:
                                                if ( c == mDelimeter )
                                                {
                                                        /* field is empty. */
                                                        fields << "";
                                                        state = DELIM;
                                                }
                                                else
                                                {
                                                        /* beginning of a simple field. */
                                                        field.append( c );
                                                        state = SIMPLE;
                                                }
                                                break;
                                        }
                                        break;

                                case QUOTED:
                                        switch (c)
                                        {
                                        case '"':
                                                /* Possible end of field, but could be 1st of a pair. */
                                                state = QUOTED_QUOTE1;
                                                break;
                                        case '\\':
                                                /* Escape next character, or special escape, e.g. \n. */
                                                state = QUOTED_ESCAPED;
                                                break;
                                        default:
                                                /* Use character literally. */
                                                field.append( c );
                                                break;
                                        }
                                        break;

                                case QUOTED_QUOTE1:
                                        switch (c)
                                        {
                                        case '\n':
                                                /* line ended after quoted item */
                                                fields << QString( field );
                                                state = DONE;
                                                break;
                                        case '"':
                                                /* second quote, insert and stay quoted. */
                                                field.append( c );
                                                state = QUOTED;
                                                break;
                                        case '\r':
                                                /* ignore and go to fallback */
                                                state = SIMPLE;
                                                break;
                                        default:
                                                if ( c == mDelimeter )
                                                {
                                                        /* end of field. */
                                                        fields << QString( field );
                                                        field.clear();
                                                        state = DELIM;
                                                }
                                                else
                                                {
                                                        /* fallback if not a delim or another quote. */
                                                        field.append( c );
                                                        state = SIMPLE;
                                                }
                                                break;
                                        }
                                        break;

                                case QUOTED_ESCAPED:
                                        switch (c)
                                        {
                                        case 'n':
                                                /* Decode "\n" as newline. */
                                                field.append( '\n' );
                                                state = QUOTED;
                                                break;
                                        case 't':
                                                /* Decode "\t" as tab. */
                                                field.append( '\t' );
                                                state = QUOTED;
                                                break;
                                        default:
                                                /* Use character literally. */
                                                field.append( c );
                                                state = QUOTED;
                                                break;
                                        }
                                        break;

                                case SIMPLE:
                                        switch (c)
                                        {
                                        case '\n':
                                                /* line ended */
                                                fields << QString( field );
                                                state = DONE;
                                                break;
                                        case '\r':
                                                /* ignore */
                                                state = SIMPLE;
                                                break;
                                        case '\\':
                                                /* Escape next character, or special escape, e.g. \n. */
                                                state = SIMPLE_ESCAPED;
                                                break;
                                        default:
                                                if ( c == mDelimeter )
                                                {
                                                        /* end of field. */
                                                        fields << QString( field );
                                                        field.clear();
                                                        state = DELIM;
                                                }
                                                else
                                                {
                                                        /* Use character literally. */
                                                        field.append( c );
                                                        state = SIMPLE;
                                                }
                                                break;
                                        }
                                        break;

                                case SIMPLE_ESCAPED:
                                        switch (c)
                                        {
                                        case 'n':
                                                /* Decode "\n" as newline. */
                                                field.append( '\n' );
                                                state = SIMPLE;
                                                break;
                                        case 't':
                                                /* Decode "\t" as tab. */
                                                field.append( '\t' );
                                                state = SIMPLE;
                                                break;
                                        default:
                                                /* Use character literally. */
                                                field.append( (char)c );
                                                state = SIMPLE;
                                                break;
                                        }
                                        break;

                                default:
                                        qWarning( "merge::Text::parseLine()::Should not be reached! #1" );
                                        break;
                                }

                        }
                        else
                        {
                                /* Handle EOF (could also be an error while reading). */
                                switch (state)
                                {

                                case DELIM:
                                        /* EOF, no more lines. */
                                        break;

                                case QUOTED:
                                        /* File ended midway through quoted item. Truncate field. */
                                        fields << QString( field );
                                        break;

                                case QUOTED_QUOTE1:
                                        /* File ended after quoted item. */
                                        fields << QString( field );
                                        break;

                                case QUOTED_ESCAPED:
                                        /* File ended midway through quoted item. Truncate field. */
                                        fields << QString( field );
                                        break;

                                case SIMPLE:
                                        /* File ended after simple item. */
                                        fields << QString( field );
                                        break;

                                case SIMPLE_ESCAPED:
                                        /* File ended midway through escaped item. */
                                        fields << QString( field );
                                        break;

                                default:
                                        qWarning( "merge::Text::parseLine()::Should not be reached! #2" );
                                        break;
                                }

                                state = DONE;
                        }
                }


                return fields;
        }

}
