//  TestTextNode.cpp
//
//  Copyright (C) 2019  Jaye Evins <evins@snaught.com>
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


#include "TestTextNode.hpp"

#include "model/TextNode.hpp"

#include <QDebug>


QTEST_MAIN(TestTextNode)

using namespace glabels::model;
using namespace glabels::merge;


void TestTextNode::textNode()
{
        Record record;
        Variables vars;

        TextNode textNode;
        QVERIFY( !textNode.isField() );
        QCOMPARE( textNode.data(), QString( "" ) );
        QVERIFY( textNode == TextNode() );
        QVERIFY( !(textNode != TextNode()) );
        QCOMPARE( textNode.text( NullRecord(), vars ), QString( "" ) );
        QCOMPARE( textNode.text( record, vars ), QString( "" ) );

        textNode.setField( true );
        QVERIFY( textNode.isField() );
        QCOMPARE( textNode.text( record, vars ), QString( "" ) );

        textNode.setField( false );
        QVERIFY( !textNode.isField() );

        textNode.setData( QString( "data1" ) );
        QCOMPARE( textNode.data(), QString( "data1" ) );
        QCOMPARE( textNode.text( NullRecord(), vars ), QString( "data1" ) );
        QCOMPARE( textNode.text( record, vars ), QString( "data1" ) );

        textNode.setField( true );
        QCOMPARE( textNode.text( NullRecord(), vars ), QString( "" ) );
        QCOMPARE( textNode.text( record, vars ), QString( "" ) );

        ///
        /// Constructors
        ///
        TextNode textNode2( true, "data2" );
        QVERIFY( textNode2.isField() );
        QCOMPARE( textNode2.data(), QString( "data2" ) );
        textNode.setField( false );
        QVERIFY( !(textNode2 == textNode) );
        QVERIFY( textNode2 != textNode );
        textNode.setField( true );
        QVERIFY( !(textNode2 == textNode) );
        QVERIFY( textNode2 != textNode );
        textNode.setData( QString( "data2" ) );
        QVERIFY( textNode2 == textNode );
        QVERIFY( !(textNode2 != textNode) );

        ///
        /// Record
        ///
        record["key1"] = "";
        QCOMPARE( textNode.text( record, vars ), QString( "" ) );

        textNode.setData( QString( "key1" ) );
        QCOMPARE( textNode.text( record, vars ), QString( "" ) );

        record["key1"] = "val1";
        QCOMPARE( textNode.text( record, vars ), QString( "val1" ) );

        ///
        /// Variable
        ///
        {
                Variable key1( Variable::Type::STRING, "key1", "", Variable::Increment::PER_ITEM );
                vars.addVariable( key1 );
        }
        QCOMPARE( textNode.text( NullRecord(), vars ), QString( "" ) );

        {
                Variable key1( Variable::Type::STRING, "key1", "val1", Variable::Increment::PER_ITEM );
                vars.addVariable( key1 );
        }
        QCOMPARE( textNode.text( NullRecord(), vars ), QString( "val1" ) );

        {
                Variable key1( Variable::Type::INTEGER, "key1", "1", Variable::Increment::PER_ITEM, "1" );
                vars.addVariable( key1 );
        }
        QCOMPARE( textNode.text( NullRecord(), vars ), QString( "1" ) );
        vars.incrementVariablesOnItem();
        QCOMPARE( textNode.text( NullRecord(), vars ), QString( "2" ) );

        ///
        /// Record beats variable
        ///
        QCOMPARE( textNode.text( record, vars ), QString( "val1" ) );
}
