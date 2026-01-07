//  Merge/TextCsv.cpp
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


#include "TextCsv.hpp"


namespace
{
        static const QString ID = "Text/Comma";
}


namespace glabels::merge
{

        ///
        /// Constructor
        ///
        TextCsv::TextCsv() : Text(',',false)
        {
                mId = ID;
        }


        ///
        /// Constructor
        ///
        TextCsv::TextCsv( const TextCsv* merge ) : Text( merge )
        {
        }


        ///
        /// Clone
        ///
        TextCsv* TextCsv::clone() const
        {
                return new TextCsv( this );
        }


        ///
        /// Get ID
        ///
        QString TextCsv::id()
        {
                return ID;
        }


        ///
        /// Create
        ///
        Merge* TextCsv::create()
        {
                return new TextCsv();
        }

}
