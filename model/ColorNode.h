/*  ColorNode.h
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

#ifndef model_ColorNode_h
#define model_ColorNode_h


#include "Variables.h"
#include "merge/Record.h"

#include <QString>
#include <QColor>

#include <cstdint>


namespace glabels
{
	namespace model
	{

		///
		/// Color Node Type
		///
		class ColorNode
		{

			/////////////////////////////////
			// Life Cycle
			/////////////////////////////////
		public:
			ColorNode();

			ColorNode( bool isField, const QColor& color, const QString& key );

			ColorNode( bool isField, uint32_t rgba, const QString& key );

			ColorNode( const QColor& color );

			ColorNode( const QString& key );


			/////////////////////////////////
			// Operators
			/////////////////////////////////
		public:
			bool operator==( const ColorNode& cn );

			bool operator!=( const ColorNode& cn );


			/////////////////////////////////
			// Properties
			/////////////////////////////////
		public:
			//
			// Field Flag Property
			//
			bool isField() const;
			void setField( bool isField );
		

			//
			// Color Property
			//
			const QColor& color() const;
			void setColor( const QColor& color );


			//
			// Key Property
			//
			const QString& key() const;
			void setKey( const QString& key );


			/////////////////////////////////
			// Misc. Methods
			/////////////////////////////////
		public:
			uint32_t rgba() const;
			QColor color( const merge::Record& record,
			              const Variables&     variables ) const;


			/////////////////////////////////
			// Private Data
			/////////////////////////////////
		private:
			bool    mIsField;
			QColor  mColor;
			QString mKey;

		};

	}
}


#endif // model_ColorNode_h
