/*  ColorNode.cpp
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


#include "ColorNode.h"


namespace glabels
{
	namespace model
	{

		///
		/// Default Constructor
		///
		ColorNode::ColorNode()
			: mIsField(false), mColor(QColor::fromRgba(0x00000000)), mKey("")
		{
			// empty
		}


		///
		/// Constructor From Data
		///
		ColorNode::ColorNode( bool isField, const QColor& color, const QString& key )
			: mIsField(isField), mColor(color), mKey(key)
		{
			// empty
		}


		///
		/// Constructor From Data
		///
		ColorNode::ColorNode( bool isField, uint32_t rgba, const QString& key )
			: mIsField(isField), mKey(key)
		{
			mColor = QColor( (rgba >> 24) & 0xFF,
			                 (rgba >> 16) & 0xFF,
			                 (rgba >>  8) & 0xFF,
			                 (rgba      ) & 0xFF );
		}


		///
		/// Constructor From Color
		///
		ColorNode::ColorNode( const QColor& color )
			: mIsField(false), mColor(color), mKey("")
		{
			// empty
		}


		///
		/// Constructor From Key
		///
		ColorNode::ColorNode( const QString& key )
			: mIsField(true), mColor(QColor::fromRgba(0x00000000)), mKey(key)
		{
			// empty
		}


		///
		/// == Operator
		///
		bool ColorNode::operator==( const ColorNode& cn )
		{
			return (mIsField == cn.mIsField) &&
				(mColor   == cn.mColor)   &&
				(mKey     == cn.mKey);
		}


		///
		/// != Operator
		///
		bool ColorNode::operator!=( const ColorNode& cn )
		{
			return (mIsField != cn.mIsField) ||
				(mColor   != cn.mColor)   ||
				(mKey     != cn.mKey);
		}


		///
		/// Field Flag Property Getter
		///
		bool ColorNode::isField() const
		{
			return mIsField;
		}
		

		///
		/// Field Flag Property Setter
		///
		void ColorNode::setField( bool isField )
		{
			mIsField = isField;
		}


		///
		/// Color Property Getter
		///
		const QColor& ColorNode::color() const
		{
			return mColor;
		}


		///
		/// Color Property Setter
		///
		void ColorNode::setColor( const QColor& color )
		{
			mColor = color;
		}
		

		///
		/// Key Property Getter
		///
		const QString& ColorNode::key() const
		{
			return mKey;
		}


		///
		/// Key Property Setter
		///
		void ColorNode::setKey( const QString& key )
		{
			mKey = key;
		}
		

		///
		/// Get color encoded as an RGBA 32-bit number
		///
		uint32_t ColorNode::rgba() const
		{
			uint32_t c =
				mColor.red()   << 24 |
				mColor.green() << 16 |
				mColor.blue()  <<  8 |
				mColor.alpha();

			return c;
		}


		///
		/// Get color, expand if necessary
		///
		QColor ColorNode::color( const merge::Record& record,
		                         const Variables&     variables ) const
		{
			QColor value = QColor( 192, 192, 192, 128 );
			
			bool haveRecordField = mIsField &&
				record.contains(mKey) &&
				!record.value(mKey).isEmpty();
			bool haveVariable = mIsField &&
				variables.contains(mKey) &&
				!variables[mKey].value().isEmpty();

			if ( haveRecordField )
			{
				value = QColor( record.value(mKey) );
			}
			else if ( haveVariable )
			{
				value = QColor( variables[mKey].value() );
			}
			else if ( !mIsField )
			{
				value = mColor;
			}

			return value;
		}

	}
}
