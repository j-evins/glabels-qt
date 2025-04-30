/*  ParserState.cpp
 *
 *  Copyright (C) 2025  Jaye Evins <evins@snaught.com>
 *
 *  This file is part of gLabels-qt.
 *
 *  gLabels-qt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  gLabels-qt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "ParserState.h"

#include <algorithm>


namespace glabels
{
	namespace model
	{

		ParserState::ParserState( const QString& string,
		                          unsigned int   pos )
			: mString(&string),
			  mPos( pos )
		{
		}


		QChar
		ParserState::operator[]( unsigned int i ) const
		{
			auto ii = mPos + i;
			if ( ii < mString->size() )
			{
				return (*mString)[ii];
			}
			else
			{
				return '\0';
			}
		}


		bool
		ParserState::isNextSubString( const QString& s ) const
		{
			for ( unsigned int i = 0; i < s.size(); i++ )
			{
				if ( operator[](i) != s[i] ) return false;
			}
			return true;
		}


		qsizetype
		ParserState::pos() const
		{
			return mPos;
		}


		qsizetype
		ParserState::charsLeft() const
		{
			return mString->size() - mPos;
		}


		void
		ParserState::advanceChars( unsigned int i )
		{
			mPos = std::min( mPos + i, mString->size() );
		}

	}
}
