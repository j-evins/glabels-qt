//  TextNode.cpp
//
//  Copyright (C) 2017-2026  Jaye Evins <evins@snaught.com>
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


#include "TextNode.hpp"


namespace glabels::model
{

        ///
        /// Default Constructor
        ///
        TextNode::TextNode()
                : mIsField(false), mData("")
        {
                // empty
        }


        ///
        /// Constructor from Data
        ///
        TextNode::TextNode( bool isField, const QString &data )
                : mIsField(isField), mData(data)
        {
                // empty
        }


        ///
        /// == Operator
        ///
        bool TextNode::operator==( const TextNode& other )
        {
                return (mIsField == other.mIsField) &&
                        (mData    == other.mData);
        }


        ///
        /// != Operator
        ///
        bool TextNode::operator!=( const TextNode& other )
        {
                return (mIsField != other.mIsField) ||
                        (mData    != other.mData);
        }


        ///
        /// isField? Property Getter
        ///
        bool TextNode::isField() const
        {
                return mIsField;
        }


        ///
        /// isField Flag Property Setter
        ///
        void TextNode::setField( bool isField )
        {
                mIsField = isField;
        }


        ///
        /// Data Property Getter
        ///
        const QString& TextNode::data() const
        {
                return mData;
        }


        ///
        /// Data Property Setter
        ///
        void TextNode::setData( const QString& data )
        {
                mData = data;
        }


        ///
        /// Get text, expand if necessary
        ///
        QString TextNode::text( const merge::Record& record,
                                const Variables&     variables ) const
        {
                QString value("");

                bool haveRecordField = mIsField &&
                        record.contains(mData) &&
                        !record.value(mData).isEmpty();
                bool haveVariable = mIsField &&
                        variables.contains(mData) &&
                        !variables[mData].value().isEmpty();

                if ( haveRecordField )
                {
                        value = record.value(mData);
                }
                else if ( haveVariable )
                {
                        value = variables[mData].value();
                }
                else if ( !mIsField )
                {
                        value = mData;
                }

                return value;
        }


}
