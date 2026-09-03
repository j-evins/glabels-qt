//  TestRawVCard.cpp
//
//  Copyright (C) 2018-2026  Jaye Evins <evins@snaught.com>
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


#include "TestRawVCard.hpp"

#include "VCardTestData.hpp"

#include "backends/merge/RawVCard.hpp"

#include <QBuffer>


QTEST_MAIN(TestRawVCard)


void TestRawVCard::readNextRawVCard()
{
        using namespace glabels::merge;

        QBuffer testFile( &VCardTestData::set1 );

        testFile.open( QIODevice::ReadOnly);

        auto rawVCard1 = RawVCard::readNextRawVCard( testFile );
        auto rawVCard2 = RawVCard::readNextRawVCard( testFile );
        auto rawVCard3 = RawVCard::readNextRawVCard( testFile );
        auto rawVCard4 = RawVCard::readNextRawVCard( testFile );
        auto rawVCard5 = RawVCard::readNextRawVCard( testFile );

        testFile.close();

        //
        // Verify size of each raw record
        //
        QCOMPARE( rawVCard1.size(), 5 );
        QCOMPARE( rawVCard2.size(), 5 );
        QCOMPARE( rawVCard3.size(), 9 );
        QCOMPARE( rawVCard4.size(), 5 );
        QVERIFY( rawVCard5.isEmpty() );

        //
        // Verify some random lines
        //
        QCOMPARE( rawVCard1[4], "TEL;TYPE=CELL;TYPE=WORK:+1-555-0101" );
        QCOMPARE( rawVCard2[0], "VERSION:3.0" );
        QCOMPARE( rawVCard3[1], "FN:Frank Dawson" );
        QCOMPARE( rawVCard3[3], "ADR;TYPE=WORK,POSTAL,PARCEL:;;6544 Battleford Drive;Raleigh;NC;27613-3502;U.S.A." ); // Folded line
        QCOMPARE( rawVCard4[2], "N:Davis;Dave;;;" );
}


void TestRawVCard::readNextRawVCard_Embedded()
{
        using namespace glabels::merge;

        QBuffer testFile( &VCardTestData::set2 );

        testFile.open( QIODevice::ReadOnly);

        auto rawVCard1 = RawVCard::readNextRawVCard( testFile );
        auto rawVCard2 = RawVCard::readNextRawVCard( testFile );
        auto rawVCard3 = RawVCard::readNextRawVCard( testFile );
        auto rawVCard4 = RawVCard::readNextRawVCard( testFile );
        auto rawVCard5 = RawVCard::readNextRawVCard( testFile );

        testFile.close();

        //
        // Verify size of each raw record
        //
        QCOMPARE( rawVCard1.size(), 5 );
        QCOMPARE( rawVCard2.size(), 5 );
        QCOMPARE( rawVCard3.size(), 9 );
        QCOMPARE( rawVCard4.size(), 5 );
        QVERIFY( rawVCard5.isEmpty() );

        //
        // Verify some random lines
        //
        QCOMPARE( rawVCard1[4], "TEL;TYPE=CELL;TYPE=WORK:+1-555-0101" );
        QCOMPARE( rawVCard2[0], "VERSION:3.0" );
        QCOMPARE( rawVCard3[1], "FN:Frank Dawson" );
        QCOMPARE( rawVCard3[3], "ADR;TYPE=WORK,POSTAL,PARCEL:;;6544 Battleford Drive;Raleigh;NC;27613-3502;U.S.A." ); // Folded line
        QCOMPARE( rawVCard4[2], "N:Davis;Dave;;;" );
}


void TestRawVCard::parseRawVCard()
{
        using namespace glabels::merge;

        QBuffer testFile( &VCardTestData::set1 );

        testFile.open( QIODevice::ReadOnly);

        auto vCard = RawVCard::readNextRawVCard( testFile );
        while ( !vCard.isEmpty()  )
        {
                for ( auto& line : vCard )
                {
                        auto lineTokens = RawVCard::tokenizeLine( line );
                }
        }

        testFile.close();
}
