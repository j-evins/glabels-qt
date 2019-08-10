/*  TestRawText.cpp
 *
 *  Copyright (C) 2019  Jim Evins <evins@snaught.com>
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

#include "TestRawText.h"

#include "model/RawText.h"

#include "merge/Record.h"

#include <QtDebug>


QTEST_MAIN(TestRawText)

using namespace glabels::model;
using namespace glabels::merge;


void TestRawText::rawText()
{
	RawText rawText;
	Record record;

	QVERIFY( rawText.isEmpty() );
	QVERIFY( !rawText.hasPlaceHolders() );
	QCOMPARE( rawText.toString(), QString( "" ) );
	QCOMPARE( rawText.toStdString(), std::string( "" ) );
	QCOMPARE( rawText.expand( &record ), QString( "" ) );

	rawText = "text";
	QVERIFY( !rawText.isEmpty() );
	QVERIFY( !rawText.hasPlaceHolders() );
	QCOMPARE( rawText.toString(), QString( "text" ) );
	QCOMPARE( rawText.toStdString(), std::string( "text" ) );
	QCOMPARE( rawText.expand( &record ), QString( "text" ) );

	RawText rawText2( "text" );
	QVERIFY( !rawText2.isEmpty() );
	QVERIFY( !rawText2.hasPlaceHolders() );
	QCOMPARE( rawText2.toString(), QString( "text" ) );

	rawText = "${key1}";
	QVERIFY( !rawText.isEmpty() );
	QVERIFY( rawText.hasPlaceHolders() );
	QCOMPARE( rawText.toString(), QString( "${key1}" ) );
	QCOMPARE( rawText.toStdString(), std::string( "${key1}" ) );
	QCOMPARE( rawText.expand( &record ), QString( "" ) );

	///
	/// Record
	///
	record["key1"] = "val1";
	QCOMPARE( rawText.expand( &record ), QString( "val1" ) );

	rawText = "${key1}${key2}";
	QVERIFY( rawText.hasPlaceHolders() );
	QCOMPARE( rawText.expand( &record ), QString( "val1" ) );

	record["key2"] = "val2";
	QCOMPARE( rawText.expand( &record ), QString( "val1val2" ) );

	rawText = "${key1}text${key2}";
	QVERIFY( rawText.hasPlaceHolders() );
	QCOMPARE( rawText.expand( &record ), QString( "val1textval2" ) );

	rawText = "text1${key1}text2${key2}text3";
	QVERIFY( rawText.hasPlaceHolders() );
	QCOMPARE( rawText.expand( &record ), QString( "text1val1text2val2text3" ) );

	rawText = "${key1}text${key2}${key3}";
	QVERIFY( rawText.hasPlaceHolders() );
	QCOMPARE( rawText.expand( &record ), QString( "val1textval2" ) );

	rawText = "${key2}${key3}${key1}";
	QVERIFY( rawText.hasPlaceHolders() );
	QCOMPARE( rawText.expand( &record ), QString( "val2val1" ) );

	///
	/// Sample Record
	///
	record["key3"] = "val3";

	rawText = "${key0}";
	QCOMPARE( rawText.expandSample( "1234567890", &record ), QString( "1234567890" ) );

	rawText = "${key0}${key1}${key3}asdf${key4}${key2}${key3}";
	QCOMPARE( rawText.expandSample( "1234567890", &record ), QString( "1234567890val1val3asdf1234567890val2val3" ) );

	rawText = "asdf";
	QCOMPARE( rawText.expandSample( "1234567890", &record ), QString( "asdf" ) );
}
