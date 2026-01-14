//  Merge/TextSemicolonKeys.cpp
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


#include "TextSemicolonKeys.hpp"


namespace
{
        static const QString ID = "Text/Semicolon/Keys";
}


namespace glabels::merge
{

        ///
        /// Constructor
        ///
        TextSemicolonKeys::TextSemicolonKeys() : Text(';',true)
        {
                mId = ID;
        }


        ///
        /// Constructor
        ///
        TextSemicolonKeys::TextSemicolonKeys( const TextSemicolonKeys* merge ) : Text( merge )
        {
        }


        ///
        /// Clone
        ///
        TextSemicolonKeys* TextSemicolonKeys::clone() const
        {
                return new TextSemicolonKeys( this );
        }


        ///
        /// Get ID
        ///
        QString TextSemicolonKeys::id()
        {
                return ID;
        }


        ///
        /// Create
        ///
        Merge* TextSemicolonKeys::create()
        {
                return new TextSemicolonKeys();
        }

}
