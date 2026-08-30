//  TestVCardSimpleParser.cpp
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


#include "TestVCardSimpleParser.hpp"

#include "backends/merge/VCardSimpleParser.hpp"

#include <QBuffer>


namespace
{
        QByteArray multiCardTestData =
                "BEGIN:VCARD\r\n"
                "VERSION:3.0\r\n"
                "FN:Alice Anderson\r\n"
                "N:Anderson;Alice;;;\r\n"
                "EMAIL;TYPE=INTERNET:alice@example.com\r\n"
                "TEL;TYPE=CELL:+1-555-0101\r\n"
                "END:VCARD\r\n"
                "BEGIN:VCARD\r\n"
                "VERSION:3.0\r\n"
                "FN:Bob Brown\r\n"
                "N:Brown;Bob;;;\r\n"
                "EMAIL;TYPE=INTERNET:bob@example.com\r\n"
                "TEL;TYPE=CELL:+1-555-0102\r\n"
                "END:VCARD\r\n"
                "BEGIN:vCard\r\n"
                "VERSION:3.0\r\n"
                "FN:Frank Dawson\r\n"
                "ORG:Lotus Development Corporation\r\n"
                "ADR;TYPE=WORK,POSTAL,PARCEL:;;6544 Battleford Drive\r\n" // Folded onto next line
                " ;Raleigh;NC;27613-3502;U.S.A.\r\n"                      // ...folded line
                "TEL;TYPE=VOICE,MSG,WORK:+1-919-676-9515\r\n"
                "TEL;TYPE=FAX,WORK:+1-919-676-9564\r\n"
                "EMAIL;TYPE=INTERNET,PREF:Frank_Dawson@Lotus.com\r\n"
                "EMAIL;TYPE=INTERNET:fdawson@earthlink.net\r\n"
                "URL:http://home.earthlink.net/~fdawson\r\n"
                "END:vCard\r\n"
                "BEGIN:VCARD\r\n"
                "VERSION:3.0\r\n"
                "FN:Dave Davis\r\n"
                "N:Davis;Dave;;;\r\n"
                "EMAIL;TYPE=INTERNET:dave@example.com\r\n"
                "TEL;TYPE=CELL:+1-555-0104\r\n"
                "END:VCARD\r\n";

        // Simulates VCards embedded in a text document (e.g. email)
        QByteArray embeddedMultiCardTestData =
                "...Some text before the first VCard...\r\n"
                "...\r\n"
                "BEGIN:VCARD\r\n"
                "VERSION:3.0\r\n"
                "FN:Alice Anderson\r\n"
                "N:Anderson;Alice;;;\r\n"
                "EMAIL;TYPE=INTERNET:alice@example.com\r\n"
                "TEL;TYPE=CELL:+1-555-0101\r\n"
                "END:VCARD\r\n"
                "...\r\n"
                "...Some text between VCards...\r\n"
                "...\r\n"
                "BEGIN:VCARD\r\n"
                "VERSION:3.0\r\n"
                "FN:Bob Brown\r\n"
                "N:Brown;Bob;;;\r\n"
                "EMAIL;TYPE=INTERNET:bob@example.com\r\n"
                "TEL;TYPE=CELL:+1-555-0102\r\n"
                "END:VCARD\r\n"
                "BEGIN:vCard\r\n"
                "VERSION:3.0\r\n"
                "FN:Frank Dawson\r\n"
                "ORG:Lotus Development Corporation\r\n"
                "ADR;TYPE=WORK,POSTAL,PARCEL:;;6544 Battleford Drive\r\n" // Folded onto next line
                " ;Raleigh;NC;27613-3502;U.S.A.\r\n"                      // ...folded line
                "TEL;TYPE=VOICE,MSG,WORK:+1-919-676-9515\r\n"
                "TEL;TYPE=FAX,WORK:+1-919-676-9564\r\n"
                "EMAIL;TYPE=INTERNET,PREF:Frank_Dawson@Lotus.com\r\n"
                "EMAIL;TYPE=INTERNET:fdawson@earthlink.net\r\n"
                "URL:http://home.earthlink.net/~fdawson\r\n"
                "END:vCard\r\n"
                "BEGIN:VCARD\r\n"
                "VERSION:3.0\r\n"
                "FN:Dave Davis\r\n"
                "N:Davis;Dave;;;\r\n"
                "EMAIL;TYPE=INTERNET:dave@example.com\r\n"
                "TEL;TYPE=CELL:+1-555-0104\r\n"
                "END:VCARD\r\n"
                "...after last VCard...\r\n"
                "...\r\n"
                "...\r\n";
}



QTEST_MAIN(TestVCardSimpleParser)


void TestVCardSimpleParser::extractRawVCard()
{
        using namespace glabels::merge;

        QBuffer testFile( &multiCardTestData );

        testFile.open( QIODevice::ReadOnly);

        auto rawVCard1 = VCardSimpleParser::extractRawVCard( testFile );
        auto rawVCard2 = VCardSimpleParser::extractRawVCard( testFile );
        auto rawVCard3 = VCardSimpleParser::extractRawVCard( testFile );
        auto rawVCard4 = VCardSimpleParser::extractRawVCard( testFile );
        auto rawVCard5 = VCardSimpleParser::extractRawVCard( testFile );

        testFile.close();

        //
        // Verify size of each raw record
        //
        QCOMPARE( rawVCard1.size(), 7 );
        QCOMPARE( rawVCard2.size(), 7 );
        QCOMPARE( rawVCard3.size(), 11 );
        QCOMPARE( rawVCard4.size(), 7 );
        QVERIFY( rawVCard5.isEmpty() );

        //
        // Verify some random lines
        //
        QCOMPARE( rawVCard1[5], "TEL;TYPE=CELL:+1-555-0101" );
        QCOMPARE( rawVCard2[1], "VERSION:3.0" );
        QCOMPARE( rawVCard3[2], "FN:Frank Dawson" );
        QCOMPARE( rawVCard3[4], "ADR;TYPE=WORK,POSTAL,PARCEL:;;6544 Battleford Drive;Raleigh;NC;27613-3502;U.S.A." ); // Folded line
        QCOMPARE( rawVCard4[3], "N:Davis;Dave;;;" );
}


void TestVCardSimpleParser::extractRawVCard_Embedded()
{
        using namespace glabels::merge;

        QBuffer testFile( &embeddedMultiCardTestData );

        testFile.open( QIODevice::ReadOnly);

        auto rawVCard1 = VCardSimpleParser::extractRawVCard( testFile );
        auto rawVCard2 = VCardSimpleParser::extractRawVCard( testFile );
        auto rawVCard3 = VCardSimpleParser::extractRawVCard( testFile );
        auto rawVCard4 = VCardSimpleParser::extractRawVCard( testFile );
        auto rawVCard5 = VCardSimpleParser::extractRawVCard( testFile );

        testFile.close();

        //
        // Verify size of each raw record
        //
        QCOMPARE( rawVCard1.size(), 7 );
        QCOMPARE( rawVCard2.size(), 7 );
        QCOMPARE( rawVCard3.size(), 11 );
        QCOMPARE( rawVCard4.size(), 7 );
        QVERIFY( rawVCard5.isEmpty() );

        //
        // Verify some random lines
        //
        QCOMPARE( rawVCard1[5], "TEL;TYPE=CELL:+1-555-0101" );
        QCOMPARE( rawVCard2[1], "VERSION:3.0" );
        QCOMPARE( rawVCard3[2], "FN:Frank Dawson" );
        QCOMPARE( rawVCard3[4], "ADR;TYPE=WORK,POSTAL,PARCEL:;;6544 Battleford Drive;Raleigh;NC;27613-3502;U.S.A." ); // Folded line
        QCOMPARE( rawVCard4[3], "N:Davis;Dave;;;" );
}


void TestVCardSimpleParser::parseRawVCard()
{
        using namespace glabels::merge;

        QBuffer testFile( &multiCardTestData );

        testFile.open( QIODevice::ReadOnly);

        auto vCard = VCardSimpleParser::extractRawVCard( testFile );
        while ( !vCard.isEmpty()  )
        {
                auto record = VCardSimpleParser::parseRawVCard( vCard );

                vCard = VCardSimpleParser::extractRawVCard( testFile );
        }

        testFile.close();
}
