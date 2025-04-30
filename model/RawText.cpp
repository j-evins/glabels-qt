/*  RawText.cpp
 *
 *  Copyright (C) 2017  Jaye Evins <evins@snaught.com>
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


#include "RawText.h"

#include "ParserState.h"

#include <QRegularExpression>


namespace glabels
{
	namespace model
	{

		///
		/// Constructor from QString
		///
		RawText::RawText( const QString& string ) : mString(string)
		{
			tokenize();
		}


		///
		/// Constructor from C string operator
		///
		RawText::RawText( const char* cString ) : mString(QString(cString))
		{
			tokenize();
		}

	
		///
		/// Access as QString
		///
		QString RawText::toString() const
		{
			return mString;
		}
	

		///
		/// Access as std::string
		///
		std::string RawText::toStdString() const
		{
			return mString.toStdString();
		}
	

		///
		/// Expand all place holders
		///
		QString RawText::expand( merge::Record* record, Variables* variables ) const
		{
			QString text;
		
			foreach ( const Token& token, mTokens )
			{
				if ( token.isField )
				{
					text += token.field.evaluate( record, variables );
				}
				else
				{
					text += token.text;
				}
			}

			return text;
		}


		///
		/// Does raw text contain place holders?
		///
		bool RawText::hasPlaceHolders() const
		{
			QRegularExpression re("\\${\\w+}");
			return mString.contains( re );
		}


		///
		/// Is raw text empty?
		///
		bool RawText::isEmpty() const
		{
			return mString.isEmpty();
		}


		///
		/// Tokenize string
		///
		void RawText::tokenize()
		{
			Token token;

			ParserState s( mString );
			while ( s.charsLeft() )
			{
				SubstitutionField field;
				if ( SubstitutionField::parse( s, field ) )
				{
					// Finalize current text token, if apropos
					if ( !token.text.isEmpty() )
					{
						token.isField = false;
						mTokens.append( token );
					}
				
					// Create and finalize field token
					token.isField = true;
					token.text = "";
					token.field = field;
					mTokens.append( token );
				}
				else
				{
					token.text += s[0];
					s.advanceChars(1);
				}
			}

			// Finalize last text token, if apropos
			if ( !token.text.isEmpty() )
			{
				token.isField = false;
				mTokens.append( token );
			}
				
		}

	
	}
}
