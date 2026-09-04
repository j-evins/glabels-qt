//  TestVCardContact.cpp
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


#include "TestVCardContact.hpp"

#include "VCardTestData.hpp"

#include "backends/merge/RawVCard.hpp"
#include "backends/merge/VCardContact.hpp"

#include <QBuffer>


QTEST_MAIN(TestVCardContact)


void TestVCardContact::vCardContactSet1()
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

        auto contact1 = VCardContact::fromRawVCard( rawVCard1 );
        auto contact2 = VCardContact::fromRawVCard( rawVCard2 );
        auto contact3 = VCardContact::fromRawVCard( rawVCard3 );
        auto contact4 = VCardContact::fromRawVCard( rawVCard4 );

        //
        // Verify fields
        //
        QCOMPARE( contact1.fn,                 "Alice Anderson" );
        QCOMPARE( contact1.n,                  "Anderson;Alice;;;" );
        QCOMPARE( contact1.nFields.family,     "Anderson" );
        QCOMPARE( contact1.nFields.given,      "Alice" );
        QCOMPARE( contact1.nFields.additional, "" );
        QCOMPARE( contact1.nFields.prefixes,   "" );
        QCOMPARE( contact1.nFields.suffixes,   "" );
        QCOMPARE( contact1.email.other[0],     "alice@example.com" );
        QCOMPARE( contact1.tel.work.cell[0],   "+1-555-0101" );

        QCOMPARE( contact2.fn,                 "Bob Brown" );
        QCOMPARE( contact2.n,                  "Brown;Bob;;;" );
        QCOMPARE( contact2.nFields.family,     "Brown" );
        QCOMPARE( contact2.nFields.given,      "Bob" );
        QCOMPARE( contact2.nFields.additional, "" );
        QCOMPARE( contact2.nFields.prefixes,   "" );
        QCOMPARE( contact2.nFields.suffixes,   "" );
        QCOMPARE( contact2.email.other[0],     "bob@example.com" );
        QCOMPARE( contact2.tel.other.cell[0],  "+1-555-0102" );

        QCOMPARE( contact3.fn,                 "Frank Dawson" );
        QCOMPARE( contact3.org,                "Lotus Development Corporation" );
        QCOMPARE( contact3.tel.work.msg[0],    "+1-919-676-9515" );
        QCOMPARE( contact3.tel.work.fax[0],    "+1-919-676-9564" );
        QCOMPARE( contact3.email.other[0],     "Frank_Dawson@Lotus.com" );
        QCOMPARE( contact3.email.other[1],     "fdawson@earthlink.net" );
        QCOMPARE( contact3.url,                "http://home.earthlink.net/~fdawson" );

        QCOMPARE( contact4.fn,                 "Dave Davis" );
        QCOMPARE( contact4.n,                  "Davis;Dave;;;" );
        QCOMPARE( contact4.nFields.family,     "Davis" );
        QCOMPARE( contact4.nFields.given,      "Dave" );
        QCOMPARE( contact4.nFields.additional, "" );
        QCOMPARE( contact4.nFields.prefixes,   "" );
        QCOMPARE( contact4.nFields.suffixes,   "" );
        QCOMPARE( contact4.email.other[0],     "dave@example.com" );
        QCOMPARE( contact4.tel.other.cell[0],  "+1-555-0104" );
}
