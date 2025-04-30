/*  ParserState.h
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

#ifndef model_ParserState_h
#define model_ParserState_h


#include <QString>


namespace glabels
{
	namespace model
	{

		class ParserState
		{
		public:
			ParserState() = default;
			ParserState( const QString& string,
			             unsigned int   pos = 0 );
			~ParserState() = default;

			QChar operator[]( unsigned int i ) const;
			bool isNextSubString( const QString& s ) const;
			qsizetype pos() const;
			qsizetype charsLeft() const;

			void advanceChars( unsigned int i );


		private:
			const QString* mString;
			qsizetype      mPos{ 0 };
		};

	}
}


#endif // model_ParserState_h
