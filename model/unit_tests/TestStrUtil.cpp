/*  TestColorNode.cpp
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

#include "TestStrUtil.h"

#include "model/StrUtil.h"

#include <QtDebug>


QTEST_MAIN(TestStrUtil)


using namespace glabels::model;


void TestStrUtil::formatFraction()
{
	QCOMPARE( StrUtil::formatFraction( 3.5 ),       QString( "3¹/₂" ) );
	QCOMPARE( StrUtil::formatFraction( 11.5 ),      QString( "11¹/₂" ) );
	QCOMPARE( StrUtil::formatFraction( 0.5 ),       QString( "¹/₂" ) );
	QCOMPARE( StrUtil::formatFraction( 1.25 ),      QString( "1¹/₄" ) );
	QCOMPARE( StrUtil::formatFraction( 2.125 ),     QString( "2¹/₈" ) );
	QCOMPARE( StrUtil::formatFraction( 2.875 ),     QString( "2⁷/₈" ) );
	QCOMPARE( StrUtil::formatFraction( 3.875 ),     QString( "3⁷/₈" ) );
	QCOMPARE( StrUtil::formatFraction( 4.0625 ),    QString( "4¹/₁₆" ) );
	QCOMPARE( StrUtil::formatFraction( 5.3125 ),    QString( "5⁵/₁₆" ) );
	QCOMPARE( StrUtil::formatFraction( 6.03125 ),   QString( "6¹/₃₂" ) );
	QCOMPARE( StrUtil::formatFraction( 7.84375 ),   QString( "7²⁷/₃₂" ) );

	QCOMPARE( StrUtil::formatFraction( 0 ),         QString( "0" ) );
	QCOMPARE( StrUtil::formatFraction( 2 ),         QString( "2" ) );
	QCOMPARE( StrUtil::formatFraction( 3 ),         QString( "3" ) );
	QCOMPARE( StrUtil::formatFraction( 11 ),        QString( "11" ) );

	QCOMPARE( StrUtil::formatFraction( 3.1415926 ), QString( "3.142" ) );
}
