//  TestColorNode.cpp
//
//  Copyright (C) 2019-2026  Jaye Evins <evins@snaught.com>
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


#include "TestColorNode.hpp"

#include "model/ColorNode.hpp"

#include <QDebug>


QTEST_MAIN(TestColorNode)

using namespace glabels::model;
using namespace glabels::merge;


void TestColorNode::colorNode()
{
        uint32_t rgbaBlackTransparent = 0;
        uint32_t rgbaWhite = 0xFFFFFFFF;
        uint32_t rgbaRed = 0xFF0000FF; // ColorNode uses RGBA. QColor set alpha to opaque 0xFF by default
        uint32_t qRgbaRed = 0xFFFF0000; // QColor uses ARGB, ie alpha at top
        uint32_t qRgbaGreen80 = 0x8000FF00;

        QColor blackTransparent = QColor::fromRgba( rgbaBlackTransparent );
        QColor white = QColor::fromRgba( rgbaWhite );
        QColor red = QColor::fromRgba( qRgbaRed );
        QColor green80 = QColor::fromRgba( qRgbaGreen80 );
        QColor silver80 = QColor( 192, 192, 192, 128 );

        Record record;
        Variables vars;

        ColorNode colorNode;
        QVERIFY( !colorNode.isField() );
        QCOMPARE( colorNode.color(), blackTransparent );
        QCOMPARE( colorNode.key(), QString( "" ) );
        QCOMPARE( colorNode.rgba(), rgbaBlackTransparent );
        QCOMPARE( colorNode.color( NullRecord(), vars ), blackTransparent );
        QCOMPARE( colorNode.color( record, vars ), blackTransparent );

        colorNode.setField( true );
        QVERIFY( colorNode.isField() );
        colorNode.setField( false );
        QVERIFY( !colorNode.isField() );

        colorNode.setColor( white );
        QCOMPARE( colorNode.color(), white );
        QCOMPARE( colorNode.rgba(), rgbaWhite );
        QCOMPARE( colorNode.color( NullRecord(), vars ), white );
        QCOMPARE( colorNode.color( record, vars ), white );

        colorNode.setKey( "key1" );
        QCOMPARE( colorNode.key(), QString( "key1" ) );

        ///
        /// Constructors
        ///
        ColorNode colorNode2( true, white, QString( "key2" ) );
        QVERIFY( colorNode2.isField() );
        QCOMPARE( colorNode2.key(), QString( "key2" ) );
        QCOMPARE( colorNode2.color(), white );

        QVERIFY( colorNode2 != colorNode );
        colorNode.setField( true );
        QVERIFY( colorNode2 != colorNode );
        colorNode.setKey( "key2" );
        QVERIFY( colorNode2 == colorNode );

        ColorNode colorNode3( red );
        QVERIFY( !colorNode3.isField() );
        QCOMPARE( colorNode3.key(), QString( "" ) );
        QCOMPARE( colorNode3.color(), red );
        QCOMPARE( colorNode3.rgba(), rgbaRed );

        QVERIFY( colorNode3 != colorNode );
        colorNode.setField( false );
        QVERIFY( colorNode3 != colorNode );
        colorNode.setKey( "" );
        QVERIFY( colorNode3 != colorNode );
        colorNode.setColor( red );
        QVERIFY( colorNode3 == colorNode );

        ///
        /// Record
        ///
        colorNode = ColorNode( QString( "key1" ) );
        QVERIFY( colorNode.isField() ); // Defaults to true if given key only
        QCOMPARE( colorNode.key(), QString( "key1" ) );
        QCOMPARE( colorNode.color(), blackTransparent );
        QCOMPARE( colorNode.color( record, vars ), silver80 ); // Defaults to silver if given non-matching record/variables

        record["key1"] = "white";
        QCOMPARE( colorNode.color( record, vars ), white );

        record["key1"] = "red";
        QCOMPARE( colorNode.color( record, vars ), red );

        record["key1"] = "#FF0000";
        QCOMPARE( colorNode.color( record, vars ), red );

        record["key1"] = "#FFFF0000"; // ARGB
        QCOMPARE( colorNode.color( record, vars ), red );

        record["key1"] = "#8000FF00";
        QCOMPARE( colorNode.color( record, vars ), green80 );

        colorNode.setKey( "key2" );
        QCOMPARE( colorNode.color( record, vars ), silver80 );
        record["key2"] = "#8000FF00";
        QCOMPARE( colorNode.color( record, vars ), green80 );

        ///
        /// Variable
        ///
        colorNode = ColorNode( QString( "c1" ) );
        QVERIFY( colorNode.isField() ); // Defaults to true if given key only
        QCOMPARE( colorNode.key(), QString( "c1" ) );
        QCOMPARE( colorNode.color(), blackTransparent );
        QCOMPARE( colorNode.color( record, vars ), silver80 ); // Defaults to silver if given non-matching record/variables

        {
                Variable c1( Variable::Type::COLOR, "c1", "white", Variable::Increment::PER_ITEM );
                vars.addVariable( c1 );
        }
        QCOMPARE( colorNode.color( NullRecord(), vars ), white );
        vars.incrementVariablesOnItem();
        QCOMPARE( colorNode.color( NullRecord(), vars ), white );

        {
                Variable c1( Variable::Type::COLOR, "c1", "red", Variable::Increment::PER_ITEM );
                vars.addVariable( c1 );
        }
        QCOMPARE( colorNode.color( NullRecord(), vars ), red );

        {
                Variable c1( Variable::Type::COLOR, "c1", "#8000FF00", Variable::Increment::PER_ITEM );
                vars.addVariable( c1 );
        }
        QCOMPARE( colorNode.color( NullRecord(), vars ), green80 );

        colorNode.setKey( "c2" );
        QCOMPARE( colorNode.color( record, vars ), silver80 );

        {
                Variable c2( Variable::Type::COLOR, "c2", "#8000FF00", Variable::Increment::PER_ITEM );
                vars.addVariable( c2 );
        }
        QCOMPARE( colorNode.color( NullRecord(), vars ), green80 );

        ///
        /// Record beats variable
        ///
        record["c2"] = "red";
        QCOMPARE( colorNode.color( record, vars ), red );
}
