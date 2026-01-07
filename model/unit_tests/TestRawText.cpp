//  TestRawText.cpp
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


#include "TestRawText.hpp"

#include "model/RawText.hpp"

#include "merge/Record.hpp"

#include <QDebug>


QTEST_MAIN(TestRawText)

using namespace glabels::model;
using namespace glabels::merge;


void TestRawText::rawText()
{
        RawText rawText;
        Record record;

        Variables variables;

        QVERIFY( rawText.isEmpty() );
        QVERIFY( !rawText.hasPlaceHolders() );
        QCOMPARE( rawText.toString(), QString( "" ) );
        QCOMPARE( rawText.toStdString(), std::string( "" ) );
        QCOMPARE( rawText.expand( record, variables ), QString( "" ) );

        rawText = "text";
        QVERIFY( !rawText.isEmpty() );
        QVERIFY( !rawText.hasPlaceHolders() );
        QCOMPARE( rawText.toString(), QString( "text" ) );
        QCOMPARE( rawText.toStdString(), std::string( "text" ) );
        QCOMPARE( rawText.expand( record, variables ), QString( "text" ) );

        RawText rawText2( "text" );
        QVERIFY( !rawText2.isEmpty() );
        QVERIFY( !rawText2.hasPlaceHolders() );
        QCOMPARE( rawText2.toString(), QString( "text" ) );

        rawText = "${key1}";
        QVERIFY( !rawText.isEmpty() );
        QVERIFY( rawText.hasPlaceHolders() );
        QCOMPARE( rawText.toString(), QString( "${key1}" ) );
        QCOMPARE( rawText.toStdString(), std::string( "${key1}" ) );
        QCOMPARE( rawText.expand( record, variables ), QString( "" ) );

        ///
        /// Record
        ///
        record["key1"] = "val1";
        QCOMPARE( rawText.expand( record, variables ), QString( "val1" ) );

        rawText = "${key1}${key2}";
        QVERIFY( rawText.hasPlaceHolders() );
        QCOMPARE( rawText.expand( record, variables ), QString( "val1" ) );

        record["key2"] = "val2";
        QCOMPARE( rawText.expand( record, variables ), QString( "val1val2" ) );

        rawText = "${key1}text${key2}";
        QVERIFY( rawText.hasPlaceHolders() );
        QCOMPARE( rawText.expand( record, variables ), QString( "val1textval2" ) );

        rawText = "text1${key1}text2${key2}text3";
        QVERIFY( rawText.hasPlaceHolders() );
        QCOMPARE( rawText.expand( record, variables ), QString( "text1val1text2val2text3" ) );

        rawText = "${key1}text${key2}${key3}";
        QVERIFY( rawText.hasPlaceHolders() );
        QCOMPARE( rawText.expand( record, variables ), QString( "val1textval2" ) );

        rawText = "${key2}${key3}${key1}";
        QVERIFY( rawText.hasPlaceHolders() );
        QCOMPARE( rawText.expand( record, variables ), QString( "val2val1" ) );
}
