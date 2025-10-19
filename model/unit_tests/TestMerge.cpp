/*  TestMerge.cpp
 *
 *  Copyright (C) 2019  Jaye Evins <evins@snaught.com>
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


#include "TestMerge.h"

#include "merge/Factory.h"
#include "merge/None.h"
#include "merge/TextCsv.h"
#include "merge/TextCsvKeys.h"
#include "merge/TextTsv.h"
#include "merge/TextTsvKeys.h"
#include "merge/TextColon.h"
#include "merge/TextColonKeys.h"
#include "merge/TextSemicolon.h"
#include "merge/TextSemicolonKeys.h"

#include "merge/Record.h"

#include <QtDebug>


QTEST_MAIN(TestMerge)

Q_DECLARE_METATYPE(glabels::merge::Factory::SourceType)

using namespace glabels::merge;


void TestMerge::initTestCase()
{
	Factory::init();
}


void TestMerge::factory_data()
{
	QTest::addColumn<QString>( "id" );
	QTest::addColumn<QString>( "name" );
	QTest::addColumn<Factory::SourceType>( "type" );
	QTest::addColumn<int>( "index" );

	int index = 0;
	QTest::newRow( "None" ) << None::id() << "None" << Factory::NONE << index++;
	QTest::newRow( "TextCsv" ) << TextCsv::id() << "Text: Comma Separated Values (CSV)" << Factory::FILE << index++;
	QTest::newRow( "TextCsvKeys" ) << TextCsvKeys::id() << "Text: Comma Separated Values (CSV), keys on line 1" << Factory::FILE << index++;
	QTest::newRow( "TextTsv" ) << TextTsv::id() << "Text: Tab Separated Values (TSV)" << Factory::FILE << index++;
	QTest::newRow( "TextTsvKeys" ) << TextTsvKeys::id() << "Text: Tab Separated Values (TSV), keys on line 1" << Factory::FILE << index++;
	QTest::newRow( "TextColon" ) << TextColon::id() << "Text: Colon Separated Values" << Factory::FILE << index++;
	QTest::newRow( "TextColonKeys" ) << TextColonKeys::id() << "Text: Colon Separated Values, keys on line 1" << Factory::FILE << index++;
	QTest::newRow( "TextSemicolon" ) << TextSemicolon::id() << "Text: Semicolon Separated Values" << Factory::FILE << index++;
	QTest::newRow( "TextSemicolonKeys" ) << TextSemicolonKeys::id() << "Text: Semicolon Separated Values, keys on line 1" << Factory::FILE << index++;
}


void TestMerge::factory()
{
	QFETCH( QString, id );
	QFETCH( QString, name );
	QFETCH( Factory::SourceType, type );
	QFETCH( int, index );

	QVERIFY( Factory::nameList().contains( name ) );

	QString outName = Factory::idToName( id );
	QCOMPARE( outName, name );

	QString outId = Factory::nameToId( name );
	QCOMPARE( outId, id );

	Factory::SourceType outType = Factory::idToType( id );
	QCOMPARE( outType, type );

	outId = Factory::indexToId( index );
	QCOMPARE( outId, id );

	Merge* merge = Factory::createMerge( id );
	QVERIFY( merge );
	QCOMPARE( merge->id(), id );

	Merge* cloneMerge = merge->clone();
	QCOMPARE( cloneMerge->id(), merge->id() );
	delete cloneMerge;
	delete merge;
}


void TestMerge::factoryNotRegistered()
{
	QString id( "unregistered" );
	Merge* merge = Factory::createMerge( id );
	QVERIFY( merge );
	QVERIFY( merge->id() != id );
	QCOMPARE( merge->id(), None::id() );
	delete merge;
}


void TestMerge::text_data()
{
	QTest::addColumn<QString>( "id" );
	QTest::addColumn<bool>( "keyed" );
	QTest::addColumn<char>( "delim" );

	QTest::newRow( "TextCsv" ) << TextCsv::id() << false << ',';
	QTest::newRow( "TextCsvKeys" ) << TextCsvKeys::id() << true << ',';
	QTest::newRow( "TextTsv" ) << TextTsv::id() << false << '\t';
	QTest::newRow( "TextTsvKeys" ) << TextTsvKeys::id() << true << '\t';
	QTest::newRow( "TextColon" ) << TextColon::id() << false << ':';
	QTest::newRow( "TextColonKeys" ) << TextColonKeys::id() << true << ':';
	QTest::newRow( "TextSemicolon" ) << TextSemicolon::id() << false << ';';
	QTest::newRow( "TextSemicolonKeys" ) << TextSemicolonKeys::id() << true << ';';
}


void TestMerge::text()
{
	QFETCH( QString, id );
	QFETCH( bool, keyed );
	QFETCH( char, delim );

	QTemporaryFile file;
	file.open();
	if ( keyed )
	{
		file.write( "header1" );
		file.putChar( delim );
		file.write( "\"header 2\"" );
		file.putChar( delim );
		file.write( "header3\r\n" );
	}
	file.write( "  val11" ); // Leading spaces in SIMPLE entry
	file.putChar( delim );
	file.write( "\"\"\"val 12\"\"\"" ); // 2DQUOTE at beginning and end of DQUOTE entry
	file.putChar( delim );
	file.write( "  \"val 13\"\n" ); // Leading spaces before DQUOTE entry, end line with LF only

	file.write( "\"  val21\"\"\"" ); // Leading spaces within DQUOTE entry, 2DQUOTE at end
	file.putChar( delim );
	file.write( "\"\"\"val 22\"" ); // 2DQUOTE at beginning of DQUOTE entry
	file.putChar( delim );
	file.write( "\r\n" ); // Last field blank

	file.write( "\"\"\"\"\"\"" ); // 2 2DQUOTES alone in DQUOTE entry
	file.putChar( delim );
	file.write( "val \"32" ); // DQUOTE in SIMPLE entry
	file.putChar( delim );
	file.write( "val \"\\\"33\r\n" ); // DQUOTE backslashed-DQUOTE in SIMPLE entry

	file.putChar( delim ); file.putChar( delim ); // All fields blank
	file.write( "\r\n" );

	file.write( "val\\n \\t \\r \\\\ \\x51" ); // Backslashed-n/-t/-r/-backslash/-x in SIMPLE entry
	file.putChar( delim );
	file.write( "\"val\\n \\t \\r \\\\ \\x52\"" ); // Backslashed-n/-t/-r/-backslash/-x in QUOTE entry
	file.write( "\r\n" ); // No last delim

	file.write( "\"val \"\"61\"" ); // 2DQUOTE in middle of DQUOTE entry
	file.putChar( delim );
	file.write( "\"val\"\"" ); file.putChar( delim ); file.write( "\r\n\\\"\u2019\\\\52\"" ); // 2DQUOTE delim CRLF backslashed-DQUOTE U+2019 backslashed-backslash
	file.putChar( delim );
	file.write( "\"val63\"" ); // End without CRLF
	file.close();

	Merge* merge = Factory::createMerge( id );
	QCOMPARE( merge->id(), id );

	merge->setSource( file.fileName() );
	QCOMPARE( merge->source(), file.fileName() );

	const QList<Record>& recordList = merge->recordList();
	QCOMPARE( recordList.size(), 6 );

	//
	// Records
	//
	const char* h1 = keyed ? "header1" : "1";
	const char* h2 = keyed ? "header 2" : "2";
	const char* h3 = keyed ? "header3" : "3";
	Record record;

	record = recordList[0];
	QVERIFY( record.contains( h1 ) );
	QCOMPARE( record.value( h1 ), QString( "  val11" ) );
	QVERIFY( record.contains( h2 ) );
	QCOMPARE( record.value( h2 ), QString( "\"val 12\"" ) );
	QVERIFY( record.contains( h3 ) );
	QCOMPARE( record.value( h3 ), QString( "  \"val 13\"" ) ); // NOTE: Treats as unquoted due to leading spaces

	record = recordList[1];
	QVERIFY( record.contains( h1 ) );
	QCOMPARE( record.value( h1 ), QString( "  val21\"" ) );
	QVERIFY( record.contains( h2 ) );
	QCOMPARE( record.value( h2 ), QString( "\"val 22" ) );
	QVERIFY( record.contains( h3 ) );
	QCOMPARE( record.value( h3 ), QString( "" ) );

	record = recordList[2];
	QVERIFY( record.contains( h1 ) );
	QCOMPARE( record.value( h1 ), QString( "\"\"" ) );
	QVERIFY( record.contains( h2 ) );
	QCOMPARE( record.value( h2 ), QString( "val \"32" ) );
	QVERIFY( record.contains( h3 ) );
	QCOMPARE( record.value( h3 ), QString( "val \"\"33" ) );

	record = recordList[3];
	QVERIFY( record.contains( h1 ) );
	QCOMPARE( record.value( h1 ), QString( "" ) );
	QVERIFY( record.contains( h2 ) );
	QCOMPARE( record.value( h2 ), QString( "" ) );
	QVERIFY( record.contains( h3 ) );
	QCOMPARE( record.value( h3 ), QString( "" ) );

	record = recordList[4];
	QVERIFY( record.contains( h1 ) );
	QCOMPARE( record.value( h1 ), QString( "val\n \t r \\ x51" ) );
	QVERIFY( record.contains( h2 ) );
	QCOMPARE( record.value( h2 ), QString( "val\n \t r \\ x52" ) );
	QVERIFY( !record.contains( h3 ) );

	record = recordList[5];
	QVERIFY( record.contains( h1 ) );
	QCOMPARE( record.value( h1 ), QString( "val \"61" ) );
	QVERIFY( record.contains( h2 ) );
	QCOMPARE( record.value( h2 ), QString( "val\"" ).append( delim ).append( "\n\"\u2019\\52" ) ); // NOTE: CR missing (QIODevice::Text strips all CRs from stream)
	QVERIFY( record.contains( h3 ) );
	QCOMPARE( record.value( h3 ), QString( "val63" ) );

	//
	// Selection
	//
	QCOMPARE( merge->nSelectedRecords(), 6 ); // Initially all selected
	merge->unselectAll();
	QCOMPARE( merge->nSelectedRecords(), 0 );

	merge->setSelected( 1 );
	QCOMPARE( merge->nSelectedRecords(), 1 );
	QCOMPARE( merge->selectedRecords().size(), 1 );

	merge->setSelected( 1, false );
	QCOMPARE( merge->nSelectedRecords(), 0 );
	QCOMPARE( merge->selectedRecords().size(), 0 );

	merge->setSelected( 0 );
	merge->setSelected( 3 );
	QCOMPARE( merge->nSelectedRecords(), 2 );
	QCOMPARE( merge->selectedRecords().size(), 2 );
	QCOMPARE( merge->selectedRecords().first(), recordList[0] );
	QCOMPARE( merge->selectedRecords().last(), recordList[3] );

	merge->setSelected( 0, false );
	QCOMPARE( merge->nSelectedRecords(), 1 );
	QCOMPARE( merge->selectedRecords().size(), 1 );

	//
	// Keys
	//
	QStringList keys = merge->keys();
	QCOMPARE( keys.size(), 3 );
	QCOMPARE( keys[0], QString( h1 ) );
	QCOMPARE( keys[1], QString( h2 ) );
	QCOMPARE( keys[2], QString( h3 ) );
	QCOMPARE( merge->primaryKey(), QString( h1 ) );

	//
	// Clone
	//
	merge->unselectAll();
	merge->setSelected( 0 );
	QCOMPARE( merge->nSelectedRecords(), 1 );

	Merge* cloneMerge = merge->clone();
	QCOMPARE( cloneMerge->id(), merge->id() );
	QCOMPARE( cloneMerge->source(), merge->source() );
	QCOMPARE( cloneMerge->recordList().size(), merge->recordList().size() );
	QCOMPARE( cloneMerge->recordList()[0], merge->recordList()[0] );
	QCOMPARE( cloneMerge->recordList()[1], merge->recordList()[1] );
	QCOMPARE( cloneMerge->recordList()[2], merge->recordList()[2] );
	QCOMPARE( cloneMerge->recordList()[3], merge->recordList()[3] );
	QCOMPARE( cloneMerge->recordList()[4], merge->recordList()[4] );
	QCOMPARE( cloneMerge->recordList()[5], merge->recordList()[5] );
	QCOMPARE( cloneMerge->nSelectedRecords(), merge->nSelectedRecords() );
	QCOMPARE( cloneMerge->selectedRecords().size(), merge->selectedRecords().size() );
	QCOMPARE( cloneMerge->selectedRecords()[0], merge->selectedRecords()[0] );
	QCOMPARE( cloneMerge->keys(), merge->keys() );
	QCOMPARE( cloneMerge->primaryKey(), merge->primaryKey() );
	delete cloneMerge;
	delete merge;
}


void TestMerge::none()
{
	None none;
	QCOMPARE( none.id(), QString( "None" ) );

	None* cloneNone = none.clone();
	QCOMPARE( cloneNone->id(), none.id() );
	QCOMPARE( cloneNone->keys(), none.keys() );
	QCOMPARE( cloneNone->primaryKey(), none.primaryKey() );
	delete cloneNone;
}


void TestMerge::record()
{
	Record record;
	QCOMPARE( record.isSelected(), true );
	record.setSelected( false );
	QCOMPARE( record.isSelected(), false );
	record.setSelected( true );
	QCOMPARE( record.isSelected(), true );

	record["key"] = "val";
	QVERIFY( record.contains( "key" ) );
	QCOMPARE( record["key"], QString( "val" ) );

	Record cloneRecord = record;
	QCOMPARE( cloneRecord.isSelected(), true );
	QVERIFY( cloneRecord.contains( "key" ) );
	QCOMPARE( cloneRecord.value( "key" ), QString( "val" ) );

	record.setSelected( false );
	Record record2( record );
	QCOMPARE( record2.isSelected(), false );
	QVERIFY( record2.contains( "key" ) );
	QCOMPARE( record2["key"], QString( "val" ) );
}
