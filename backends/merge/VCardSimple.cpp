//  Merge/VCardSimple.cpp
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


#include "VCardSimple.hpp"

#include "RawVCard.hpp"
#include "Record.hpp"

#include <QDebug>


namespace glabels::merge
{

        ///
        /// Constructor
        ///
        VCardSimple::VCardSimple()
                : Merge()
        {
        }


        ///
        /// Constructor
        ///
        VCardSimple::VCardSimple( const VCardSimple* merge )
                : Merge( merge ),
                  mKeys(merge->mKeys),
                  mNFieldsMax(merge->mNFieldsMax)
        {
        }


        ///
        /// Get key list
        ///
        QStringList VCardSimple::keys() const
        {
                return mKeys;
        }


        ///
        /// Get primary key
        ///
        QString VCardSimple::primaryKey() const
        {
                return "File Under";
        }


        ///
        /// Open source
        ///
        void VCardSimple::open()
        {
                mKeys.clear();
                mNFieldsMax = 0;

                mFile.setFileName( source() );
                mFile.open( QIODevice::ReadOnly|QIODevice::Text );
        }


        ///
        /// Close source
        ///
        void VCardSimple::close()
        {
                if ( mFile.isOpen() )
                {
                        mFile.close();
                }
        }


        ///
        /// Read next record
        ///
        Record VCardSimple::readNextRecord()
        {
                auto buffer = RawVCard::readNextRawVCard( mFile );

                if ( buffer.isEmpty() )
                {
                        return NullRecord();
                }

#if 0
                auto record = VCardSimpleParser::parseRawVCard( buffer );
                mNFieldsMax = std::max( mNFieldsMax, record.size() );
#endif
                Record record;

                return record;
        }

}
