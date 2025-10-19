/*  Merge/TextTsvKeys.h
 *
 *  Copyright (C) 2016  Jaye Evins <evins@snaught.com>
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

#ifndef merge_TextTsvKeys_h
#define merge_TextTsvKeys_h


#include "Text.h"


namespace glabels
{
	namespace merge
	{

		///
		/// TextTsvKeys Merge Backend
		///
		struct TextTsvKeys : public Text
		{

			/////////////////////////////////
			// Life Cycle
			/////////////////////////////////
		private:
			TextTsvKeys();
			TextTsvKeys( const TextTsvKeys* merge );
			virtual ~TextTsvKeys() = default;


			/////////////////////////////////
			// Object duplication
			/////////////////////////////////
		public:
			TextTsvKeys* clone() const override;


			/////////////////////////////////
			// Static methods
			/////////////////////////////////
		public:
			static QString id();
			static Merge* create();

		};

	}
}


#endif // merge_TextTsvKeys_h
