//  TestFileUtil.cpp
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


#include "TestFileUtil.hpp"

#include "model/FileUtil.hpp"

#include <QDebug>


QTEST_MAIN(TestFileUtil)

using namespace glabels::model;


void TestFileUtil::addExtension()
{
        QCOMPARE( FileUtil::addExtension( "/tmp/file", ".ext" ), QString( "/tmp/file.ext" ) );
        QCOMPARE( FileUtil::addExtension( "/tmp/file.ext", ".ext" ), QString( "/tmp/file.ext" ) );
        QCOMPARE( FileUtil::addExtension( "/tmp/file.ext", ".txt" ), QString( "/tmp/file.ext.txt" ) );
        QCOMPARE( FileUtil::addExtension( "/tmp/file", "txt" ), QString( "/tmp/filetxt" ) );
        QCOMPARE( FileUtil::addExtension( "/tmp/filetxt", "txt" ), QString( "/tmp/filetxt" ) );
}


void TestFileUtil::systemTemplatesDir()
{
        QDir dir = FileUtil::systemTemplatesDir();
        QVERIFY( dir.exists() );
        QVERIFY( dir.isReadable() );
        QVERIFY( dir.path().endsWith( "templates" ) );
}


void TestFileUtil::manualUserTemplatesDir()
{
        QDir dir = FileUtil::manualUserTemplatesDir();
        QVERIFY( dir.exists() );
        QVERIFY( dir.isReadable() );
        QVERIFY( dir.path().endsWith( ".glabels" ) );
}


void TestFileUtil::userTemplatesDir()
{
        QDir dir = FileUtil::userTemplatesDir();
        QVERIFY( dir.exists() );
        QVERIFY( dir.isReadable() );
        QFileInfo fileInfo( dir.path() );
        QVERIFY( fileInfo.isWritable() );
}


void TestFileUtil::translationsDir()
{
        QDir dir = FileUtil::translationsDir();
        QVERIFY( dir.exists() );
        QVERIFY( dir.isReadable() );
        QVERIFY( dir.path().endsWith( "translations" ) );
}


void TestFileUtil::makeRelativeIfInDir_data()
{
        QTest::addColumn<QString>( "dir" );
        QTest::addColumn<QString>( "filename" );
        QTest::addColumn<QString>( "expected" );

        QTest::newRow( "1" ) << "/dir/subdir" << "/dir/subdir/filename" << "filename";
        QTest::newRow( "2" ) << "/dir/subdir" << "filename" << "filename";
        QTest::newRow( "3" ) << "/dir" << "subdir/filename" << "subdir/filename";
        QTest::newRow( "4" ) << "/dir" << "/dir/subdir/subdir/filename" << "subdir/subdir/filename";
        QTest::newRow( "5" ) << "/dir/subdir" << "/dir/subdir/subdir/filename" << "subdir/filename";
        QTest::newRow( "6" ) << "/dir/subdir" << "/dir/subdir2/filename" << "/dir/subdir2/filename";
        QTest::newRow( "7" ) << "/dir2/subdir" << "/dir/subdir/filename" << "/dir/subdir/filename";
        QTest::newRow( "8" ) << "/dir/subdir" << "/dir/filename" << "/dir/filename";
}


void TestFileUtil::makeRelativeIfInDir()
{
        QFETCH( QString, dir );
        QFETCH( QString, filename );
        QFETCH( QString, expected );

        QCOMPARE( FileUtil::makeRelativeIfInDir( QDir( dir ), filename ), expected );
}
