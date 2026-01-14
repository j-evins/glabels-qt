//  TestXmlUtil.cpp
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


#include "TestXmlUtil.hpp"

#include "model/XmlUtil.hpp"


QTEST_MAIN(TestXmlUtil)


void TestXmlUtil::getStringAttr()
{
        using namespace glabels::model;

        // Test XML
        QString xml = "<root a='A test string' />";

        // Setup document and extract root node
        QDomDocument doc;
        QCOMPARE( doc.setContent( xml, false ), true );
        QDomElement node = doc.documentElement();
        QCOMPARE( node.tagName(), QString("root") );

        //
        // Tests
        //
        QCOMPARE( XmlUtil::getStringAttr( node, "a", "Default" ), QString( "A test string" ) );

        // non-existant attribute, use default
        QCOMPARE( XmlUtil::getStringAttr( node, "b", "Default" ), QString( "Default" ) );
}


void TestXmlUtil::getDoubleAttr()
{
        using namespace glabels::model;

        // Test XML
        QString xml = "<root a='0' b='0.' c='1' d='1.5' e='-1.5e-1' f='x' />";

        // Setup document and extract root node
        QDomDocument doc;
        QCOMPARE( doc.setContent( xml, false ), true );
        QDomElement node = doc.documentElement();
        QCOMPARE( node.tagName(), QString("root") );

        //
        // Tests
        //
        QCOMPARE( XmlUtil::getDoubleAttr( node, "a", 3.14 ), 0.0 );
        QCOMPARE( XmlUtil::getDoubleAttr( node, "b", 3.14 ), 0.0 );
        QCOMPARE( XmlUtil::getDoubleAttr( node, "c", 3.14 ), 1.0 );
        QCOMPARE( XmlUtil::getDoubleAttr( node, "d", 3.14 ), 1.5 );
        QCOMPARE( XmlUtil::getDoubleAttr( node, "e", 3.14 ), -0.15 );

        // bad value, use default
        QCOMPARE( XmlUtil::getDoubleAttr( node, "f", 3.14 ), 3.14 );

        // non-existant attribute, use default
        QCOMPARE( XmlUtil::getDoubleAttr( node, "g", 3.14 ), 3.14 );
}


void TestXmlUtil::getBoolAttr()
{
        using namespace glabels::model;

        // Test XML
        QString xml = "<root a='0' b='1' c='true' d='false' e='x' />";

        // Setup document and extract root node
        QDomDocument doc;
        QCOMPARE( doc.setContent( xml, false ), true );
        QDomElement node = doc.documentElement();
        QCOMPARE( node.tagName(), QString("root") );

        //
        // Tests
        //
        QCOMPARE( XmlUtil::getBoolAttr( node, "a", false ), false );
        QCOMPARE( XmlUtil::getBoolAttr( node, "a", true ),  false );
        QCOMPARE( XmlUtil::getBoolAttr( node, "b", false ), true );
        QCOMPARE( XmlUtil::getBoolAttr( node, "b", true ),  true );
        QCOMPARE( XmlUtil::getBoolAttr( node, "c", false ), true );
        QCOMPARE( XmlUtil::getBoolAttr( node, "c", true ),  true );
        QCOMPARE( XmlUtil::getBoolAttr( node, "d", false ), false );
        QCOMPARE( XmlUtil::getBoolAttr( node, "d", true ),  false );

        // bad value, use default
        QCOMPARE( XmlUtil::getBoolAttr( node, "e", false ), false );
        QCOMPARE( XmlUtil::getBoolAttr( node, "e", true ),  true );

        // non-existant attribute, use default
        QCOMPARE( XmlUtil::getBoolAttr( node, "f", false ), false );
        QCOMPARE( XmlUtil::getBoolAttr( node, "f", true ),  true );
}


void TestXmlUtil::getIntAttr()
{
        using namespace glabels::model;

        // Test XML
        QString xml = "<root a='0' b='1' c='-11' d='1234567890' e='x' />";

        // Setup document and extract root node
        QDomDocument doc;
        QCOMPARE( doc.setContent( xml, false ), true );
        QDomElement node = doc.documentElement();
        QCOMPARE( node.tagName(), QString("root") );

        //
        // Tests
        //
        QCOMPARE( XmlUtil::getIntAttr( node, "a", 456 ), 0 );
        QCOMPARE( XmlUtil::getIntAttr( node, "b", 456 ), 1 );
        QCOMPARE( XmlUtil::getIntAttr( node, "c", 456 ), -11 );
        QCOMPARE( XmlUtil::getIntAttr( node, "d", 456 ), 1234567890 );

        // bad value, use default
        QCOMPARE( XmlUtil::getIntAttr( node, "e", 456 ), 456 );

        // non-existant attribute, use default
        QCOMPARE( XmlUtil::getIntAttr( node, "f", 456 ), 456 );
}


void TestXmlUtil::getUIntAttr()
{
        using namespace glabels::model;

        // Test XML
        QString xml = "<root a='0' b='1' c='1234567890' d='0xBAADBEEF' e='0xc001000f' f='0xillegal' />";

        // Setup document and extract root node
        QDomDocument doc;
        QCOMPARE( doc.setContent( xml, false ), true );
        QDomElement node = doc.documentElement();
        QCOMPARE( node.tagName(), QString("root") );

        //
        // Tests
        //
        QCOMPARE( XmlUtil::getUIntAttr( node, "a", 0xdefa17 ), 0u );
        QCOMPARE( XmlUtil::getUIntAttr( node, "b", 0xdefa17 ), 1u );
        QCOMPARE( XmlUtil::getUIntAttr( node, "c", 0xdefa17 ), 1234567890u );
        QCOMPARE( XmlUtil::getUIntAttr( node, "d", 0xdefa17 ), 0xbaadbeef );
        QCOMPARE( XmlUtil::getUIntAttr( node, "e", 0xdefa17 ), 0xc001000f );

        // bad value, use default
        QCOMPARE( XmlUtil::getUIntAttr( node, "f", 0xdefa17 ), 0xdefa17u );

        // non-existant attribute, use default
        QCOMPARE( XmlUtil::getUIntAttr( node, "g", 0xdefa17 ), 0xdefa17u );
}


void TestXmlUtil::getLengthAttr()
{
        using namespace glabels::model;

        // Test XML
        QString xml = "<root a='0' b='-1' c='10in' d='1.5mm' e='-1.5e-1cm' f='3pt' g='1.2bad' h='x' />";

        // Setup document and extract root node
        QDomDocument doc;
        QCOMPARE( doc.setContent( xml, false ), true );
        QDomElement node = doc.documentElement();
        QCOMPARE( node.tagName(), QString("root") );

        //
        // Tests
        //
        QCOMPARE( XmlUtil::getLengthAttr( node, "a", Distance::pt(1234) ), Distance::pt(0.0) );
        QCOMPARE( XmlUtil::getLengthAttr( node, "b", Distance::pt(1234) ), Distance::pt(-1.0) );
        QCOMPARE( XmlUtil::getLengthAttr( node, "c", Distance::pt(1234) ), Distance::pt(720) );
        QCOMPARE( XmlUtil::getLengthAttr( node, "d", Distance::pt(1234) ), Distance::mm(1.5) );
        QCOMPARE( XmlUtil::getLengthAttr( node, "e", Distance::pt(1234) ), Distance::cm(-0.15) );
        QCOMPARE( XmlUtil::getLengthAttr( node, "f", Distance::pt(1234) ), Distance::pt(3) );

        // bad value, use default
        QCOMPARE( XmlUtil::getLengthAttr( node, "g", Distance::pt(1234) ), Distance::pt(1234) );
        QCOMPARE( XmlUtil::getLengthAttr( node, "h", Distance::pt(1234) ), Distance::pt(1234) );

        // non-existant attribute, use default
        QCOMPARE( XmlUtil::getLengthAttr( node, "i", Distance::pt(1234) ), Distance::pt(1234) );
}


void TestXmlUtil::getWeightAttr()
{
        using namespace glabels::model;

        // Test XML
        QString xml = "<root a='bold' b='normal' c='xxxx' />";

        // Setup document and extract root node
        QDomDocument doc;
        QCOMPARE( doc.setContent( xml, false ), true );
        QDomElement node = doc.documentElement();
        QCOMPARE( node.tagName(), QString("root") );

        //
        // Tests
        //
        QCOMPARE( XmlUtil::getWeightAttr( node, "a", QFont::Normal ), QFont::Bold );
        QCOMPARE( XmlUtil::getWeightAttr( node, "a", QFont::Bold ),   QFont::Bold );
        QCOMPARE( XmlUtil::getWeightAttr( node, "b", QFont::Normal ), QFont::Normal );
        QCOMPARE( XmlUtil::getWeightAttr( node, "b", QFont::Bold ),   QFont::Normal );

        // bad value, use default
        QCOMPARE( XmlUtil::getWeightAttr( node, "c", QFont::Normal ), QFont::Normal );
        QCOMPARE( XmlUtil::getWeightAttr( node, "c", QFont::Bold ),   QFont::Bold );

        // non-existant attribute, use default
        QCOMPARE( XmlUtil::getWeightAttr( node, "d", QFont::Normal ), QFont::Normal );
        QCOMPARE( XmlUtil::getWeightAttr( node, "d", QFont::Bold ),   QFont::Bold );
}


void TestXmlUtil::getAlignmentAttr()
{
        using namespace glabels::model;

        // Test XML
        QString xml = "<root a='left' b='hcenter' c='right' d='bottom' e='vcenter' f='top' g='yyyy' />";

        // Setup document and extract root node
        QDomDocument doc;
        QCOMPARE( doc.setContent( xml, false ), true );
        QDomElement node = doc.documentElement();
        QCOMPARE( node.tagName(), QString("root") );

        //
        // Tests
        //
        QCOMPARE( XmlUtil::getAlignmentAttr( node, "a", Qt::AlignRight ),  Qt::AlignLeft );
        QCOMPARE( XmlUtil::getAlignmentAttr( node, "b", Qt::AlignRight ),  Qt::AlignHCenter );
        QCOMPARE( XmlUtil::getAlignmentAttr( node, "c", Qt::AlignLeft ),   Qt::AlignRight );
        QCOMPARE( XmlUtil::getAlignmentAttr( node, "d", Qt::AlignTop ),    Qt::AlignBottom );
        QCOMPARE( XmlUtil::getAlignmentAttr( node, "e", Qt::AlignTop ),    Qt::AlignVCenter );
        QCOMPARE( XmlUtil::getAlignmentAttr( node, "f", Qt::AlignBottom ), Qt::AlignTop );

        // bad value, use default
        QCOMPARE( XmlUtil::getAlignmentAttr( node, "g", Qt::AlignTop ),    Qt::AlignTop );
        QCOMPARE( XmlUtil::getAlignmentAttr( node, "g", Qt::AlignLeft ),   Qt::AlignLeft );

        // non-existant attribute, use default
        QCOMPARE( XmlUtil::getAlignmentAttr( node, "h", Qt::AlignBottom ), Qt::AlignBottom );
}


void TestXmlUtil::getWrapModeAttr()
{
        using namespace glabels::model;

        // Test XML
        QString xml = "<root a='word' b='anywhere' c='none' d='zzzz' />";

        // Setup document and extract root node
        QDomDocument doc;
        QCOMPARE( doc.setContent( xml, false ), true );
        QDomElement node = doc.documentElement();
        QCOMPARE( node.tagName(), QString("root") );

        //
        // Tests
        //
        QCOMPARE( XmlUtil::getWrapModeAttr( node, "a", QTextOption::NoWrap ),       QTextOption::WordWrap );
        QCOMPARE( XmlUtil::getWrapModeAttr( node, "b", QTextOption::NoWrap ),       QTextOption::WrapAnywhere );
        QCOMPARE( XmlUtil::getWrapModeAttr( node, "c", QTextOption::WordWrap ),     QTextOption::NoWrap );

        // bad value, use default
        QCOMPARE( XmlUtil::getWrapModeAttr( node, "d", QTextOption::NoWrap ),       QTextOption::NoWrap );
        QCOMPARE( XmlUtil::getWrapModeAttr( node, "d", QTextOption::WordWrap ),     QTextOption::WordWrap );

        // non-existant attribute, use default
        QCOMPARE( XmlUtil::getWrapModeAttr( node, "e", QTextOption::NoWrap ),       QTextOption::NoWrap );
        QCOMPARE( XmlUtil::getWrapModeAttr( node, "e", QTextOption::WrapAnywhere ), QTextOption::WrapAnywhere );
}
