//  Merge/None.cpp
//
//  Copyright (C) 2015-2016  Jaye Evins <evins@snaught.com>
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


#include "None.hpp"


namespace glabels::merge
{

        ///
        /// Constructor
        ///
        None::None() : Merge()
        {
                mId = "None";
        }


        ///
        /// Constructor
        ///
        None::None( const None* merge ) : Merge( merge )
        {
        }


        ///
        /// Clone
        ///
        None* None::clone() const
        {
                return new None( this );
        }


        ///
        /// Get ID
        ///
        QString None::id()
        {
                return "None";
        }


        ///
        /// Create
        ///
        Merge* None::create()
        {
                return new None();
        }


        ///
        /// Get key list
        ///
        QStringList None::keys() const
        {
                QStringList emptyList;
                return emptyList;
        }


        ///
        /// Get primary key
        ///
        QString None::primaryKey() const
        {
                return "";
        }


        ///
        /// Open source
        ///
        void None::open()
        {
        }


        ///
        /// Close source
        ///
        void None::close()
        {
        }


        ///
        /// Read next record
        ///
        Record None::readNextRecord()
        {
                return NullRecord();
        }

}
