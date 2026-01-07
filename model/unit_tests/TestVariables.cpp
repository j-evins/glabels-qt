//  TestVariables.cpp
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


#include "TestVariables.hpp"

#include "model/Variables.hpp"

#include <QDebug>


QTEST_MAIN(TestVariables)

using namespace glabels::model;


void TestVariables::variables()
{
        Variables vars;

        Variable i( Variable::Type::INTEGER, "i", "3", Variable::Increment::PER_ITEM, "3" );
        QCOMPARE( i.value(), QString( "3" ) );

        Variable i2( Variable::Type::INTEGER, "i2", "100", Variable::Increment::PER_COPY, "2" );
        QCOMPARE( i2.value(), QString( "100" ) );

        Variable f( Variable::Type::FLOATING_POINT, "f", "0.0", Variable::Increment::PER_PAGE, "0.1" );
        QCOMPARE( f.value(), QString( "0" ) );

        Variable s( Variable::Type::STRING, "s", "initial", Variable::Increment::PER_ITEM, "1" );
        QCOMPARE( s.value(), QString( "initial" ) );

        Variable c( Variable::Type::COLOR, "c", "white", Variable::Increment::PER_ITEM, "01" );
        QCOMPARE( c.value(), QString( "white" ) );

        QVERIFY( !vars.hasVariable( "i" ) );
        QVERIFY( !vars.hasVariable( "i2" ) );

        // Add, delete
        vars.addVariable( i );
        QVERIFY( vars.hasVariable( "i" ) );
        QCOMPARE( vars["i"].value(), i.value() );
        vars.deleteVariable( "i" );
        QVERIFY( !vars.hasVariable( "i" ) );
        QCOMPARE( vars["i"].value(), QString() );

        // Add, replace
        vars.addVariable( i );
        QVERIFY( vars.hasVariable( "i" ) );
        QCOMPARE( vars["i"].value(), i.value() );
        vars.replaceVariable( "i", i2 );
        QVERIFY( !vars.hasVariable( "i" ) );
        QVERIFY( vars.hasVariable( "i2" ) );
        QCOMPARE( vars["i2"].value(), i2.value() );
        QCOMPARE( vars["i"].value(), QString() );
        vars.deleteVariable( "i2" );
        QVERIFY( !vars.hasVariable( "i2" ) );

        // Increment
        vars.addVariable( i );
        vars.addVariable( i2 );
        vars.addVariable( f );
        vars.addVariable( s );
        vars.addVariable( c );

        QVERIFY( vars.hasVariable( "i" ) ); // PER_ITEM
        QVERIFY( vars.hasVariable( "i2" ) ); // PER_COPY
        QVERIFY( vars.hasVariable( "f" ) ); // PER_PAGE
        QVERIFY( vars.hasVariable( "s" ) );
        QVERIFY( vars.hasVariable( "c" ) );

        QCOMPARE( vars["i"].value(), QString( "3" ) );
        QCOMPARE( vars["i2"].value(), QString( "100" ) ); // PRE_COPY
        QCOMPARE( vars["f"].value(), QString( "0" ) ); // PER_PAGE
        QCOMPARE( vars["s"].value(), QString( "initial" ) );
        QCOMPARE( vars["c"].value(), QString( "white" ) );

        vars.resetVariables();

        QCOMPARE( vars["i"].value(), QString( "3" ) ); // PER_ITEM
        QCOMPARE( vars["i2"].value(), QString( "100" ) ); // PRE_COPY
        QCOMPARE( vars["f"].value(), QString( "0" ) ); // PER_PAGE
        QCOMPARE( vars["s"].value(), QString( "initial" ) );
        QCOMPARE( vars["c"].value(), QString( "white" ) );

        vars.incrementVariablesOnItem();

        QCOMPARE( vars["i"].value(), QString( "6" ) ); // PER_ITEM
        QCOMPARE( vars["i2"].value(), QString( "100" ) ); // PER_COPY
        QCOMPARE( vars["f"].value(), QString( "0" ) ); // PER_PAGE
        QCOMPARE( vars["s"].value(), QString( "initial" ) );
        QCOMPARE( vars["c"].value(), QString( "white" ) );

        vars.incrementVariablesOnItem();

        QCOMPARE( vars["i"].value(), QString( "9" ) ); // PER_ITEM
        QCOMPARE( vars["i2"].value(), QString( "100" ) ); // PER_COPY
        QCOMPARE( vars["f"].value(), QString( "0" ) ); // PER_PAGE
        QCOMPARE( vars["s"].value(), QString( "initial" ) );
        QCOMPARE( vars["c"].value(), QString( "white" ) );

        vars.incrementVariablesOnCopy();

        QCOMPARE( vars["i"].value(), QString( "9" ) ); // PER_ITEM
        QCOMPARE( vars["i2"].value(), QString( "102" ) ); // PER_COPY
        QCOMPARE( vars["f"].value(), QString( "0" ) ); // PER_PAGE
        QCOMPARE( vars["s"].value(), QString( "initial" ) );
        QCOMPARE( vars["c"].value(), QString( "white" ) );

        vars.incrementVariablesOnCopy();

        QCOMPARE( vars["i"].value(), QString( "9" ) ); // PER_ITEM
        QCOMPARE( vars["i2"].value(), QString( "104" ) ); // PER_COPY
        QCOMPARE( vars["f"].value(), QString( "0" ) ); // PER_PAGE
        QCOMPARE( vars["s"].value(), QString( "initial" ) );
        QCOMPARE( vars["c"].value(), QString( "white" ) );

        vars.incrementVariablesOnPage();

        QCOMPARE( vars["i"].value(), QString( "9" ) ); // PER_ITEM
        QCOMPARE( vars["i2"].value(), QString( "104" ) ); // PER_COPY
        QCOMPARE( vars["f"].value(), QString( "0.1" ) ); // PER_PAGE
        QCOMPARE( vars["s"].value(), QString( "initial" ) );
        QCOMPARE( vars["c"].value(), QString( "white" ) );

        vars.incrementVariablesOnPage();

        QCOMPARE( vars["i"].value(), QString( "9" ) ); // PER_ITEM
        QCOMPARE( vars["i2"].value(), QString( "104" ) ); // PER_COPY
        QCOMPARE( vars["f"].value(), QString( "0.2" ) ); // PER_PAGE
        QCOMPARE( vars["s"].value(), QString( "initial" ) );
        QCOMPARE( vars["c"].value(), QString( "white" ) );

        vars.resetVariables();

        QCOMPARE( vars["i"].value(), QString( "3" ) ); // PER_ITEM
        QCOMPARE( vars["i2"].value(), QString( "100" ) ); // PRE_COPY
        QCOMPARE( vars["f"].value(), QString( "0" ) ); // PER_PAGE
        QCOMPARE( vars["s"].value(), QString( "initial" ) );
        QCOMPARE( vars["c"].value(), QString( "white" ) );
}
