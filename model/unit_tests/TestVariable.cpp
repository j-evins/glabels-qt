//  TestVariable.cpp
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


#include "TestVariable.hpp"

#include "model/Variable.hpp"

#include <QDebug>


QTEST_MAIN(TestVariable)

using namespace glabels::model;


void TestVariable::variable()
{
        {
                Variable var;

                QCOMPARE( var.type(), Variable::Type::STRING );
                QCOMPARE( var.name(), QString() );
                QCOMPARE( var.initialValue(), QString() );
                QCOMPARE( var.increment(), Variable::Increment::NEVER );
                QCOMPARE( var.stepSize(), QString( "0" ) );
                QCOMPARE( var.value(), QString() );

                var.resetValue();
                QCOMPARE( var.type(), Variable::Type::STRING );
                QCOMPARE( var.name(), QString() );
                QCOMPARE( var.initialValue(), QString() );
                QCOMPARE( var.increment(), Variable::Increment::NEVER );
                QCOMPARE( var.stepSize(), QString( "0" ) );
                QCOMPARE( var.value(), QString() );

                var.incrementValueOnItem();
                QCOMPARE( var.value(), QString() );

                var.incrementValueOnCopy();
                QCOMPARE( var.value(), QString() );

                var.incrementValueOnPage();
                QCOMPARE( var.value(), QString() );
        }
        {
                Variable var( Variable::Type::STRING, "s", "initial", Variable::Increment::PER_ITEM, "2" );

                QCOMPARE( var.type(), Variable::Type::STRING );
                QCOMPARE( var.name(), QString( "s" ) );
                QCOMPARE( var.initialValue(), QString( "initial" ) );
                QCOMPARE( var.increment(), Variable::Increment::PER_ITEM );
                QCOMPARE( var.stepSize(), QString( "2" ) );
                QCOMPARE( var.value(), QString( "initial" ) );

                var.resetValue();
                QCOMPARE( var.name(), QString( "s" ) );
                QCOMPARE( var.initialValue(), QString( "initial" ) );
                QCOMPARE( var.increment(), Variable::Increment::PER_ITEM );
                QCOMPARE( var.stepSize(), QString( "2" ) );
                QCOMPARE( var.value(), QString( "initial" ) );

                var.incrementValueOnItem();
                QCOMPARE( var.value(), QString( "initial" ) );

                var.incrementValueOnCopy();
                QCOMPARE( var.value(), QString( "initial" ) );

                var.incrementValueOnPage();
                QCOMPARE( var.value(), QString( "initial" ) );

                var.resetValue();
                QCOMPARE( var.name(), QString( "s" ) );
                QCOMPARE( var.initialValue(), QString( "initial" ) );
                QCOMPARE( var.increment(), Variable::Increment::PER_ITEM );
                QCOMPARE( var.stepSize(), QString( "2" ) );
                QCOMPARE( var.value(), QString( "initial" ) );
        }
        {
                Variable var( Variable::Type::INTEGER, "i", "123", Variable::Increment::PER_ITEM, "1" );

                QCOMPARE( var.type(), Variable::Type::INTEGER );
                QCOMPARE( var.name(), QString( "i" ) );
                QCOMPARE( var.initialValue(), QString( "123" ) );
                QCOMPARE( var.increment(), Variable::Increment::PER_ITEM );
                QCOMPARE( var.stepSize(), QString( "1" ) );
                QCOMPARE( var.value(), QString( "123" ) );

                var.resetValue();
                QCOMPARE( var.type(), Variable::Type::INTEGER );
                QCOMPARE( var.name(), QString( "i" ) );
                QCOMPARE( var.initialValue(), QString( "123" ) );
                QCOMPARE( var.increment(), Variable::Increment::PER_ITEM );
                QCOMPARE( var.stepSize(), QString( "1" ) );
                QCOMPARE( var.value(), QString( "123" ) );

                var.incrementValueOnItem();
                QCOMPARE( var.value(), QString( "124" ) );

                var.incrementValueOnCopy();
                QCOMPARE( var.value(), QString( "124" ) );

                var.incrementValueOnPage();
                QCOMPARE( var.value(), QString( "124" ) );

                var.incrementValueOnItem();
                QCOMPARE( var.value(), QString( "125" ) );

                var.incrementValueOnCopy();
                QCOMPARE( var.value(), QString( "125" ) );

                var.incrementValueOnPage();
                QCOMPARE( var.value(), QString( "125" ) );

                var.resetValue();
                QCOMPARE( var.type(), Variable::Type::INTEGER );
                QCOMPARE( var.name(), QString( "i" ) );
                QCOMPARE( var.initialValue(), QString( "123" ) );
                QCOMPARE( var.increment(), Variable::Increment::PER_ITEM );
                QCOMPARE( var.stepSize(), QString( "1" ) );
                QCOMPARE( var.value(), QString( "123" ) );
        }
        {
                Variable var( Variable::Type::INTEGER, "i", "1", Variable::Increment::PER_PAGE, "2" );

                QCOMPARE( var.type(), Variable::Type::INTEGER );
                QCOMPARE( var.name(), QString( "i" ) );
                QCOMPARE( var.initialValue(), QString( "1" ) );
                QCOMPARE( var.increment(), Variable::Increment::PER_PAGE );
                QCOMPARE( var.stepSize(), QString( "2" ) );
                QCOMPARE( var.value(), QString( "1" ) );

                var.resetValue();
                QCOMPARE( var.type(), Variable::Type::INTEGER );
                QCOMPARE( var.name(), QString( "i" ) );
                QCOMPARE( var.initialValue(), QString( "1" ) );
                QCOMPARE( var.increment(), Variable::Increment::PER_PAGE );
                QCOMPARE( var.stepSize(), QString( "2" ) );
                QCOMPARE( var.value(), QString( "1" ) );

                var.incrementValueOnItem();
                QCOMPARE( var.value(), QString( "1" ) );

                var.incrementValueOnCopy();
                QCOMPARE( var.value(), QString( "1" ) );

                var.incrementValueOnPage();
                QCOMPARE( var.value(), QString( "3" ) );

                var.incrementValueOnItem();
                QCOMPARE( var.value(), QString( "3" ) );

                var.incrementValueOnCopy();
                QCOMPARE( var.value(), QString( "3" ) );

                var.incrementValueOnPage();
                QCOMPARE( var.value(), QString( "5" ) );

                var.resetValue();
                QCOMPARE( var.type(), Variable::Type::INTEGER );
                QCOMPARE( var.name(), QString( "i" ) );
                QCOMPARE( var.initialValue(), QString( "1" ) );
                QCOMPARE( var.increment(), Variable::Increment::PER_PAGE );
                QCOMPARE( var.stepSize(), QString( "2" ) );
                QCOMPARE( var.value(), QString( "1" ) );
        }
        {
                Variable var( Variable::Type::FLOATING_POINT, "f", "1.2", Variable::Increment::PER_COPY, "0.2" );

                QCOMPARE( var.type(), Variable::Type::FLOATING_POINT );
                QCOMPARE( var.name(), QString( "f" ) );
                QCOMPARE( var.initialValue(), QString( "1.2" ) );
                QCOMPARE( var.increment(), Variable::Increment::PER_COPY );
                QCOMPARE( var.stepSize(), QString( "0.2" ) );
                QCOMPARE( var.value(), QString( "1.2" ) );

                var.resetValue();
                QCOMPARE( var.type(), Variable::Type::FLOATING_POINT );
                QCOMPARE( var.name(), QString( "f" ) );
                QCOMPARE( var.initialValue(), QString( "1.2" ) );
                QCOMPARE( var.increment(), Variable::Increment::PER_COPY );
                QCOMPARE( var.stepSize(), QString( "0.2" ) );
                QCOMPARE( var.value(), QString( "1.2" ) );

                var.incrementValueOnItem();
                QCOMPARE( var.value(), QString( "1.2" ) );

                var.incrementValueOnCopy();
                QCOMPARE( var.value(), QString( "1.4" ) );

                var.incrementValueOnPage();
                QCOMPARE( var.value(), QString( "1.4" ) );

                var.incrementValueOnItem();
                QCOMPARE( var.value(), QString( "1.4" ) );

                var.incrementValueOnCopy();
                QCOMPARE( var.value(), QString( "1.6" ) );

                var.incrementValueOnPage();
                QCOMPARE( var.value(), QString( "1.6" ) );

                var.resetValue();
                QCOMPARE( var.type(), Variable::Type::FLOATING_POINT );
                QCOMPARE( var.name(), QString( "f" ) );
                QCOMPARE( var.initialValue(), QString( "1.2" ) );
                QCOMPARE( var.increment(), Variable::Increment::PER_COPY );
                QCOMPARE( var.stepSize(), QString( "0.2" ) );
                QCOMPARE( var.value(), QString( "1.2" ) );
        }
        {
                Variable var( Variable::Type::COLOR, "c", "white", Variable::Increment::PER_PAGE );

                QCOMPARE( var.type(), Variable::Type::COLOR );
                QCOMPARE( var.name(), QString( "c" ) );
                QCOMPARE( var.initialValue(), QString( "white" ) );
                QCOMPARE( var.increment(), Variable::Increment::PER_PAGE );
                QCOMPARE( var.stepSize(), QString( "0" ) );
                QCOMPARE( var.value(), QString( "white" ) );

                var.resetValue();
                QCOMPARE( var.name(), QString( "c" ) );
                QCOMPARE( var.initialValue(), QString( "white" ) );
                QCOMPARE( var.increment(), Variable::Increment::PER_PAGE );
                QCOMPARE( var.stepSize(), QString( "0" ) );
                QCOMPARE( var.value(), QString( "white" ) );

                var.incrementValueOnItem();
                QCOMPARE( var.value(), QString( "white" ) );

                var.incrementValueOnCopy();
                QCOMPARE( var.value(), QString( "white" ) );

                var.incrementValueOnPage();
                QCOMPARE( var.value(), QString( "white" ) );

                var.resetValue();
                QCOMPARE( var.name(), QString( "c" ) );
                QCOMPARE( var.initialValue(), QString( "white" ) );
                QCOMPARE( var.increment(), Variable::Increment::PER_PAGE );
                QCOMPARE( var.stepSize(), QString( "0" ) );
                QCOMPARE( var.value(), QString( "white" ) );
        }
}


void TestVariable::statics()
{
        QCOMPARE( Variable::typeToI18nString( Variable::Type::STRING ), QString( "String" ) );
        QCOMPARE( Variable::typeToI18nString( Variable::Type::INTEGER ), QString( "Integer" ) );
        QCOMPARE( Variable::typeToI18nString( Variable::Type::FLOATING_POINT ), QString( "Floating Point" ) );
        QCOMPARE( Variable::typeToI18nString( Variable::Type::COLOR ), QString( "Color" ) );
        QCOMPARE( Variable::typeToI18nString( (Variable::Type)4 ), QString( "String" ) );

        QCOMPARE( Variable::typeToIdString( Variable::Type::STRING ), QString( "string" ) );
        QCOMPARE( Variable::typeToIdString( Variable::Type::INTEGER ), QString( "integer" ) );
        QCOMPARE( Variable::typeToIdString( Variable::Type::FLOATING_POINT ), QString( "float" ) );
        QCOMPARE( Variable::typeToIdString( Variable::Type::COLOR ), QString( "color" ) );
        QCOMPARE( Variable::typeToIdString( (Variable::Type)4 ), QString( "string" ) );

        QCOMPARE( Variable::idStringToType( "string" ), Variable::Type::STRING );
        QCOMPARE( Variable::idStringToType( "integer"), Variable::Type::INTEGER );
        QCOMPARE( Variable::idStringToType( "float" ), Variable::Type::FLOATING_POINT );
        QCOMPARE( Variable::idStringToType( "color" ), Variable::Type::COLOR );
        QCOMPARE( Variable::idStringToType( "non_existent" ), Variable::Type::STRING );

        QCOMPARE( Variable::incrementToI18nString( Variable::Increment::NEVER ), QString( "Never" ) );
        QCOMPARE( Variable::incrementToI18nString( Variable::Increment::PER_ITEM ), QString( "Per item" ) );
        QCOMPARE( Variable::incrementToI18nString( Variable::Increment::PER_COPY ), QString( "Per copy" ) );
        QCOMPARE( Variable::incrementToI18nString( Variable::Increment::PER_PAGE ), QString( "Per page" ) );
        QCOMPARE( Variable::incrementToI18nString( (Variable::Increment)4 ), QString( "Never" ) );

        QCOMPARE( Variable::incrementToIdString( Variable::Increment::NEVER ), QString( "never" ) );
        QCOMPARE( Variable::incrementToIdString( Variable::Increment::PER_ITEM ), QString( "per_item" ) );
        QCOMPARE( Variable::incrementToIdString( Variable::Increment::PER_COPY ), QString( "per_copy" ) );
        QCOMPARE( Variable::incrementToIdString( Variable::Increment::PER_PAGE ), QString( "per_page" ) );
        QCOMPARE( Variable::incrementToIdString( (Variable::Increment)4 ), QString( "never" ) );

        QCOMPARE( Variable::idStringToIncrement( "never" ), Variable::Increment::NEVER );
        QCOMPARE( Variable::idStringToIncrement( "per_item" ), Variable::Increment::PER_ITEM );
        QCOMPARE( Variable::idStringToIncrement( "per_copy" ), Variable::Increment::PER_COPY );
        QCOMPARE( Variable::idStringToIncrement( "per_page" ), Variable::Increment::PER_PAGE );
        QCOMPARE( Variable::idStringToIncrement( "non_existent" ), Variable::Increment::NEVER );
}
