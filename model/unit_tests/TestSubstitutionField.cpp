/*  TestSubstitutionField.cpp
 *
 *  Copyright (C) 2017  Jaye Evins <evins@snaught.com>
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

#include "TestSubstitutionField.h"

#include "model/SubstitutionField.h"


QTEST_MAIN(TestSubstitutionField)


void TestSubstitutionField::parseValid()
{
	using namespace glabels;

	//
	// Valid substitution fields (concatenated in single input string)
	//
	QString input = "${1234}${abc:=ABC}${x:%08.2f}${y:%08.2f:=12.34}${ADDR2:n}${FüññýßútLæg@lÑªmê}${Also_a legal-name}";
	model::ParserState s( input );
	
	model::SubstitutionField f1;
	QCOMPARE( model::SubstitutionField::parse( s, f1 ), true );
	QCOMPARE( f1.fieldName(),    QString( "1234" ) );
	QCOMPARE( f1.newLine(),      false );

	model::SubstitutionField f2;
	QCOMPARE( model::SubstitutionField::parse( s, f2 ), true );
	QCOMPARE( f2.fieldName(),    QString( "abc" ) );
	QCOMPARE( f2.defaultValue(), QString( "ABC" ) );
	QCOMPARE( f2.newLine(),      false );

	model::SubstitutionField f3;
	QCOMPARE( model::SubstitutionField::parse( s, f3 ), true );
	QCOMPARE( f3.fieldName(),    QString( "x" ) );
	QCOMPARE( f3.format(),       QString( "%08.2f" ) );
	QCOMPARE( f3.formatType(),   QChar('f') );
	QCOMPARE( f3.newLine(),      false );

	model::SubstitutionField f4;
	QCOMPARE( model::SubstitutionField::parse( s, f4 ), true );
	QCOMPARE( f4.fieldName(),    QString( "y" ) );
	QCOMPARE( f4.defaultValue(), QString( "12.34" ) );
	QCOMPARE( f4.format(),       QString( "%08.2f" ) );
	QCOMPARE( f4.formatType(),   QChar('f') );
	QCOMPARE( f4.newLine(),      false );

	model::SubstitutionField f5;
	QCOMPARE( model::SubstitutionField::parse( s, f5 ), true );
	QCOMPARE( f5.fieldName(),    QString( "ADDR2" ) );
	QCOMPARE( f5.newLine(),      true );

	model::SubstitutionField f6;
	QCOMPARE( model::SubstitutionField::parse( s, f6 ), true );
	QCOMPARE( f6.fieldName(),    QString( "FüññýßútLæg@lÑªmê" ) );
	QCOMPARE( f6.newLine(),      false );

	model::SubstitutionField f7;
	QCOMPARE( model::SubstitutionField::parse( s, f7 ), true );
	QCOMPARE( f7.fieldName(),    QString( "Also_a legal-name" ) );
	QCOMPARE( f7.newLine(),      false );
}


void TestSubstitutionField::parseInvalid()
{
	using namespace glabels;

	//
	// Ordinary text
	//
	QString input5 = "Abcdefg";
	model::ParserState s5( input5 );
	model::SubstitutionField f5;
	QCOMPARE( model::SubstitutionField::parse( s5, f5 ), false );
	QCOMPARE( s5.pos(), model::ParserState( input5 ).pos() ); // Should not advance string reference

	//
	// Invalid substitution fields (which are treated as ordinary text)
	//
	QString input6 = "$abc";
	model::ParserState s6( input6 );
	model::SubstitutionField f6;
	QCOMPARE( model::SubstitutionField::parse( s6, f6 ), false );
	QCOMPARE( s6.pos(), model::ParserState( input6 ).pos() ); // Should not advance string reference

	QString input7 = "${abc";
	model::ParserState s7( input7 );
	model::SubstitutionField f7;
	QCOMPARE( model::SubstitutionField::parse( s7, f7 ), false );
	QCOMPARE( s7.pos(), model::ParserState( input7 ).pos() ); // Should not advance string reference

	QString input8 = "${abc:}";
	model::ParserState s8( input8 );
	model::SubstitutionField f8;
	QCOMPARE( model::SubstitutionField::parse( s8, f8 ), false );
	QCOMPARE( s8.pos(), model::ParserState( input8 ).pos() ); // Should not advance string reference

	// Even though format is invalid, let it slide.  Overall structure still good.  Format will be ignored.
	QString input9 = "${abc:%3.2}";
	model::ParserState s9( input9 );
	model::SubstitutionField f9;
	QCOMPARE( model::SubstitutionField::parse( s9, f9 ), true );

	QString input10 = "${embedded\nnew-line}";
	model::ParserState s10( input10 );
	model::SubstitutionField f10;
	QCOMPARE( model::SubstitutionField::parse( s10, f10 ), false );
	QCOMPARE( s10.pos(), model::ParserState( input10 ).pos() ); // Should not advance string reference

	QString input11 = "${how-about-a\ttab}";
	model::ParserState s11( input11 );
	model::SubstitutionField f11;
	QCOMPARE( model::SubstitutionField::parse( s11, f11 ), false );
	QCOMPARE( s11.pos(), model::ParserState( input11 ).pos() ); // Should not advance string reference
}


void TestSubstitutionField::construction()
{
	using namespace glabels;

	model::SubstitutionField f1( "${1234}" );
	QCOMPARE( f1.fieldName(),    QString( "1234" ) );

	model::SubstitutionField f2( "${abc:=ABC}" );
	QCOMPARE( f2.fieldName(),    QString( "abc" ) );
	QCOMPARE( f2.defaultValue(), QString( "ABC" ) );

	model::SubstitutionField f3( "${x:%08.2f}" );
	QCOMPARE( f3.fieldName(),    QString( "x" ) );
	QCOMPARE( f3.format(),       QString( "%08.2f" ) );
	QCOMPARE( f3.formatType(),   QChar('f') );

	model::SubstitutionField f4( "${y:%08.2f:=12.34}" );
	QCOMPARE( f4.fieldName(),    QString( "y" ) );
	QCOMPARE( f4.defaultValue(), QString( "12.34" ) );
	QCOMPARE( f4.format(),       QString( "%08.2f" ) );
	QCOMPARE( f4.formatType(),   QChar('f') );
}


void TestSubstitutionField::simpleEvaluation()
{
	using namespace glabels;

	model::Variables variables;
	
	model::SubstitutionField f1( "${1}" );
	model::SubstitutionField f2( "${2}" );
	model::SubstitutionField f3( "${3}" );
	model::SubstitutionField f4( "${4}" );

	merge::Record record1;
	record1[ "1" ] = "Abcdefg";
	record1[ "2" ] = "Hijklmn";
	record1[ "3" ] = "Opqrstu";
	record1[ "4" ] = "Vwxyz!@";

	QCOMPARE( f1.evaluate( &record1, &variables ), QString( "Abcdefg" ) );
	QCOMPARE( f2.evaluate( &record1, &variables ), QString( "Hijklmn" ) );
	QCOMPARE( f3.evaluate( &record1, &variables ), QString( "Opqrstu" ) );
	QCOMPARE( f4.evaluate( &record1, &variables ), QString( "Vwxyz!@" ) );

	merge::Record record2;
	record2[ "1" ] = "1234567";
	record2[ "2" ] = "FooBar";
	record2[ "3" ] = "8901234";
	record2[ "4" ] = "#$%^&*";

	QCOMPARE( f1.evaluate( &record2, &variables ), QString( "1234567" ) );
	QCOMPARE( f2.evaluate( &record2, &variables ), QString( "FooBar" ) );
	QCOMPARE( f3.evaluate( &record2, &variables ), QString( "8901234" ) );
	QCOMPARE( f4.evaluate( &record2, &variables ), QString( "#$%^&*" ) );
}


void TestSubstitutionField::defaultValueEvaluation()
{
	using namespace glabels;

	model::Variables variables;
	
	model::SubstitutionField f1( "${1:=foo1}" );
	model::SubstitutionField f2( "${2:=foo2}" );
	model::SubstitutionField f3( "${3:=foo3}" );
	model::SubstitutionField f4( "${4:=foo4}" );

	merge::Record record1;
	record1[ "1" ] = "Abcdefg";
	record1[ "2" ] = "Hijklmn";
	record1[ "3" ] = "Opqrstu";
	record1[ "4" ] = "Vwxyz!@";

	QCOMPARE( f1.evaluate( &record1, &variables ), QString( "Abcdefg" ) );
	QCOMPARE( f2.evaluate( &record1, &variables ), QString( "Hijklmn" ) );
	QCOMPARE( f3.evaluate( &record1, &variables ), QString( "Opqrstu" ) );
	QCOMPARE( f4.evaluate( &record1, &variables ), QString( "Vwxyz!@" ) );

	merge::Record record2; // All fields empty

	QCOMPARE( f1.evaluate( &record2, &variables ), QString( "foo1" ) );
	QCOMPARE( f2.evaluate( &record2, &variables ), QString( "foo2" ) );
	QCOMPARE( f3.evaluate( &record2, &variables ), QString( "foo3" ) );
	QCOMPARE( f4.evaluate( &record2, &variables ), QString( "foo4" ) );

	merge::Record record3;
	record3[ "1" ] = "xyzzy";
	// Field "2" empty
	// Field "3" empty
	record3[ "4" ] = "plugh";
	
	QCOMPARE( f1.evaluate( &record3, &variables ), QString( "xyzzy" ) );
	QCOMPARE( f2.evaluate( &record3, &variables ), QString( "foo2" ) );
	QCOMPARE( f3.evaluate( &record3, &variables ), QString( "foo3" ) );
	QCOMPARE( f4.evaluate( &record3, &variables ), QString( "plugh" ) );
}


void TestSubstitutionField::formattedStringEvaluation()
{
	using namespace glabels;

	model::Variables variables;
	
	model::SubstitutionField f1( "${1:%10s}" );
	model::SubstitutionField f2( "${2:%10s}" );
	model::SubstitutionField f3( "${3:%10s}" );
	model::SubstitutionField f4( "${4:%10s}" );

	model::SubstitutionField f5( "${5:%-10s}" );
	model::SubstitutionField f6( "${6:%-10s}" );
	model::SubstitutionField f7( "${7:%-10s}" );
	model::SubstitutionField f8( "${8:%-10s}" );

	merge::Record record1;
	record1[ "1" ] = "0";
	record1[ "2" ] = "1";
	record1[ "3" ] = "-1";
	record1[ "4" ] = "3.14";

	record1[ "5" ] = "0";
	record1[ "6" ] = "100";
	record1[ "7" ] = "-100";
	record1[ "8" ] = "3.14";
	
	QCOMPARE( f1.evaluate( &record1, &variables ), QString( "         0" ) );
	QCOMPARE( f2.evaluate( &record1, &variables ), QString( "         1" ) );
	QCOMPARE( f3.evaluate( &record1, &variables ), QString( "        -1" ) );
	QCOMPARE( f4.evaluate( &record1, &variables ), QString( "      3.14" ) );

	QCOMPARE( f5.evaluate( &record1, &variables ), QString( "0         " ) );
	QCOMPARE( f6.evaluate( &record1, &variables ), QString( "100       " ) );
	QCOMPARE( f7.evaluate( &record1, &variables ), QString( "-100      " ) );
	QCOMPARE( f8.evaluate( &record1, &variables ), QString( "3.14      " ) );
}


void TestSubstitutionField::formattedFloatEvaluation()
{
	using namespace glabels;

	model::Variables variables;
	
	model::SubstitutionField f1( "${1:%+5.2f}" );
	model::SubstitutionField f2( "${2:%+5.2f}" );
	model::SubstitutionField f3( "${3:%+5.2f}" );
	model::SubstitutionField f4( "${4:%+5.2f}" );

	model::SubstitutionField f5( "${5:%+5.2e}" );
	model::SubstitutionField f6( "${6:%+5.2e}" );
	model::SubstitutionField f7( "${7:%+5.2e}" );
	model::SubstitutionField f8( "${8:%+5.2e}" );

	merge::Record record1;
	record1[ "1" ] = "0";
	record1[ "2" ] = "1";
	record1[ "3" ] = "-1";
	record1[ "4" ] = "3.14";

	record1[ "5" ] = "0";
	record1[ "6" ] = "100";
	record1[ "7" ] = "-100";
	record1[ "8" ] = "3.14";
	
	QCOMPARE( f1.evaluate( &record1, &variables ), QString( "+0.00" ) );
	QCOMPARE( f2.evaluate( &record1, &variables ), QString( "+1.00" ) );
	QCOMPARE( f3.evaluate( &record1, &variables ), QString( "-1.00" ) );
	QCOMPARE( f4.evaluate( &record1, &variables ), QString( "+3.14" ) );

	QCOMPARE( f5.evaluate( &record1, &variables ), QString( "+0.00e+00" ) );
	QCOMPARE( f6.evaluate( &record1, &variables ), QString( "+1.00e+02" ) );
	QCOMPARE( f7.evaluate( &record1, &variables ), QString( "-1.00e+02" ) );
	QCOMPARE( f8.evaluate( &record1, &variables ), QString( "+3.14e+00" ) );
}


void TestSubstitutionField::formattedIntEvaluation()
{
	using namespace glabels;

	model::Variables variables;
	
	model::SubstitutionField f1( "${1:%08d}" );
	model::SubstitutionField f2( "${2:%08d}" );
	model::SubstitutionField f3( "${3:%08d}" );
	model::SubstitutionField f4( "${4:%08d}" );

	model::SubstitutionField f5( "${5:%08x}" );
	model::SubstitutionField f6( "${6:%08x}" );
	model::SubstitutionField f7( "${7:%08x}" );
	model::SubstitutionField f8( "${8:%08x}" );

	merge::Record record1;
	record1[ "1" ] = "0";
	record1[ "2" ] = "1";
	record1[ "3" ] = "-1";
	record1[ "4" ] = "3.14";

	record1[ "5" ] = "100";
	record1[ "6" ] = "0x100";
	record1[ "7" ] = "-1";
	record1[ "8" ] = "314";
	
	QCOMPARE( f1.evaluate( &record1, &variables ), QString( "00000000" ) );
	QCOMPARE( f2.evaluate( &record1, &variables ), QString( "00000001" ) );
	QCOMPARE( f3.evaluate( &record1, &variables ), QString( "-0000001" ) );
	QCOMPARE( f4.evaluate( &record1, &variables ), QString( "00000000" ) ); // Invalid integer value

	QCOMPARE( f5.evaluate( &record1, &variables ), QString( "00000064" ) ); // 100(decimal) == 64(hex)
	QCOMPARE( f6.evaluate( &record1, &variables ), QString( "00000100" ) );
	QCOMPARE( f7.evaluate( &record1, &variables ), QString( "00000000" ) ); // Invalid unsigned integer
	QCOMPARE( f8.evaluate( &record1, &variables ), QString( "0000013a" ) ); // 314(decimal) == 13a(hex)
}


void TestSubstitutionField::newLineEvaluation()
{
	using namespace glabels;

	model::Variables variables;
	
	model::SubstitutionField addr2( "${ADDR2:n}" );
	QCOMPARE( addr2.fieldName(),    QString( "ADDR2" ) );
	QCOMPARE( addr2.newLine(),      true );

	merge::Record record1;
	record1[ "ADDR2" ] = "Apt. 5B";

	merge::Record record2;
	record2[ "ADDR2" ] = ""; // ADDR2 Empty
	
	merge::Record record3;
	// ADDR2 not defined

	QCOMPARE( addr2.evaluate( &record1, &variables ), QString( "\nApt. 5B" ) ); // Prepends a newline
	QCOMPARE( addr2.evaluate( &record2, &variables ), QString( "" ) ); // Evaluates empty
	QCOMPARE( addr2.evaluate( &record3, &variables ), QString( "" ) ); // Evaluates empty
}
