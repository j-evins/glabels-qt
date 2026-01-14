//  TestModelImageObject.cpp
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


#include "TestModelImageObject.hpp"
#include "Test_Constants.hpp"

#include "model/Model.hpp"
#include "model/ModelImageObject.hpp"
#include "model/Size.hpp"

#include "merge/Factory.hpp"
#include "merge/TextCsvKeys.hpp"
#include "merge/Record.hpp"

#include <QDebug>


QTEST_MAIN(TestModelImageObject)

using namespace glabels::model;
using namespace glabels::merge;


void TestModelImageObject::initTestCase()
{
        Factory::init();
}


void TestModelImageObject::readImageFile()
{
        QByteArray pngArray;
        QImage png;
        QString svgTemplate = QDir::tempPath().append( "/TestModelImageObject_XXXXXX.svg" ); // Note: directory separators canonicalized to slash by Qt path methods

        Model model;

        // Needed for relative file names to work
        QString modelFileName = QDir::tempPath().append( "/TestModelImageObject.glabels" );
        model.setFileName( modelFileName );

        ModelImageObject object;

        ///
        /// Merge object, no shadow
        ///
        object.setX0( 1 );
        object.setY0( 1 );
        object.setSize( 8, 8 );
        object.setFilenameNode( TextNode( true, "image" ) );

        model.addObject( object.clone() );

        ///
        /// Variable object, green pgn, gray shadow
        ///
        object.setY0( 11 );
        object.setShadow( true );
        object.setShadowColorNode( ColorNode( Qt::gray ) );
        object.setShadowOpacity( 1 );
        TextNode( true, "var" );
        object.setFilenameNode( TextNode( true, "var" ) );

        // Green 8x8 square pgn
        pngArray = QByteArray::fromBase64( glabels::test::green_8x8_png );
        QVERIFY( png.loadFromData( pngArray, "PNG" ) );
        QTemporaryFile pngGreen; pngGreen.open(); pngGreen.close(); png.save( pngGreen.fileName(), "PNG" );
        QFileInfo pngGreenFileInfo( pngGreen.fileName() );

        Variable var( Variable::Type::STRING, "var", pngGreenFileInfo.fileName(), Variable::Increment::PER_ITEM ); // Relative path
        model.variables().addVariable( var );

        model.addObject( object.clone() );

        ///
        /// Variable object 2, magenta svg, yellow shadow
        ///
        object.setY0( 21 );
        object.setShadow( true );
        object.setShadowColorNode( ColorNode( Qt::yellow ) );
        object.setShadowOpacity( 1 );
        object.setFilenameNode( TextNode( true, "var2" ) );

        // Magenta 8x8 square svg
        QTemporaryFile svgMagenta( svgTemplate ); svgMagenta.open(); svgMagenta.write( glabels::test::magenta_8x8_svg ); svgMagenta.close();
        QFileInfo svgMagentaFileInfo( svgMagenta.fileName() );

        Variable var2( Variable::Type::STRING, "var2", svgMagentaFileInfo.fileName(), Variable::Increment::PER_ITEM ); // Absolute path
        model.variables().addVariable( var2 );

        model.addObject( object.clone() );

        ///
        /// Filename object, yellow png, cyan shadow
        ///
        object.setY0( 31 );
        object.setShadow( true );
        object.setShadowColorNode( ColorNode( Qt::cyan ) );
        object.setShadowOpacity( 1 );

        // Yellow 8x8 square pgn
        pngArray = QByteArray::fromBase64( glabels::test::yellow_8x8_png );
        QVERIFY( png.loadFromData( pngArray, "PNG" ) );
        QTemporaryFile pngYellowFile; pngYellowFile.open(); pngYellowFile.close(); png.save( pngYellowFile.fileName(), "PNG" );

        QFileInfo pngYellowFileInfo( pngYellowFile.fileName() );

        // Need to set object parent for relative paths to work
        object.setParent( &model );

        object.setFilenameNode( TextNode( false, pngYellowFileInfo.fileName() ) ); // Relative path

        model.addObject( object.clone() );

        ///
        /// Filename object, cyan svg, magenta shadow
        ///
        object.setY0( 41 );
        object.setSize( 8, 8 );
        object.setShadow( true );
        object.setShadowColorNode( ColorNode( Qt::magenta ) );
        object.setShadowOpacity( 1 );

        // Cyan 8x8 square svg
        QTemporaryFile svgCyanFile( svgTemplate ); svgCyanFile.open(); svgCyanFile.write( glabels::test::cyan_8x8_svg ); svgCyanFile.close();

        QFileInfo svgCyanFileInfo( svgCyanFile.fileName() );
        object.setFilenameNode( TextNode( false, svgCyanFileInfo.filePath() ) ); // Absolute path

        model.addObject( object.clone() );

        ///
        /// Set up merge
        ///

        // Blue 8x8 square pgn
        pngArray = QByteArray::fromBase64( glabels::test::blue_8x8_png );
        QVERIFY( png.loadFromData( pngArray, "PNG" ) );
        QTemporaryFile png1; png1.open(); png1.close(); png.save( png1.fileName(), "PNG" );
        QTemporaryFile png2; png2.open(); png2.close(); png.save( png2.fileName(), "PNG" );

        // Red 8x8 square svg
        QTemporaryFile svg1( svgTemplate ); svg1.open(); svg1.write( glabels::test::red_8x8_svg ); svg1.close();
        QTemporaryFile svg2( svgTemplate ); svg2.open(); svg2.write( glabels::test::red_8x8_svg ); svg2.close();

        QFileInfo png1FileInfo( png1.fileName() );
        QFileInfo png2FileInfo( png2.fileName() );
        QFileInfo svg1FileInfo( svg1.fileName() );
        QFileInfo svg2FileInfo( svg2.fileName() );

        QTemporaryFile csv;
        csv.open();
        csv.write( "id,image,type\n" );
        csv.write( "1," ); csv.write( png1FileInfo.fileName().toUtf8() ); csv.write( ",png\n" );
        csv.write( "2," ); csv.write( png1FileInfo.filePath().toUtf8() ); csv.write( ",png\n" );
        csv.write( "3," ); csv.write( svg1FileInfo.fileName().toUtf8() ); csv.write( ",svg\n" );
        csv.write( "4," ); csv.write( svg2FileInfo.filePath().toUtf8() ); csv.write( ",svg\n" );
        csv.write( "5," ); csv.write( svg2FileInfo.fileName().toUtf8() ); csv.write( ",svg\n" );
        csv.write( "6," ); csv.write( png2FileInfo.fileName().toUtf8() ); csv.write( ",png\n" );
        csv.write( "7," ); csv.write( svg1FileInfo.filePath().toUtf8() ); csv.write( ",svg\n" );
        csv.write( "8," ); csv.write( png2FileInfo.filePath().toUtf8() ); csv.write( ",png\n" );
        csv.close();

        Merge* merge = Factory::createMerge( TextCsvKeys::id() );
        QVERIFY( merge );
        QCOMPARE( merge->id(), TextCsvKeys::id() );
        merge->setSource( csv.fileName() );
        model.setMerge( merge );

        ///
        /// Draw
        ///
        const QList<Record> records = merge->selectedRecords();
        QCOMPARE( records.size(), 8 );

        QImage paintDevice( 10, 10 * model.objectList().size() * records.size(), QImage::Format_RGB32 );
        paintDevice.fill( Qt::white );
        QPainter painter( &paintDevice );

        int i, cnt;
        int yTranslate = 10 * model.objectList().size();
        for ( i = 0, cnt = records.size(); i < cnt; i++ )
        {
                model.draw( &painter, false, records[i], model.variables() );
                painter.translate( 0, yTranslate );
        }

        QColor color, white = Qt::white, grayShadow = Qt::gray, yellowShadow = Qt::yellow, cyanShadow = Qt::cyan, magentaShadow = Qt::magenta;
        for ( i = 0, cnt = records.size(); i < cnt; i++ )
        {
                // Merge
                qDebug() << "record" << i;
                color = records[i].value( "type" ) == "png" ? Qt::blue : Qt::red;
                QCOMPARE( paintDevice.pixelColor( 1, 0 + i * yTranslate ), white );
                QCOMPARE( paintDevice.pixelColor( 1, 1 + i * yTranslate ), color );
                QCOMPARE( paintDevice.pixelColor( 1, 8 + i * yTranslate ), color );
                QCOMPARE( paintDevice.pixelColor( 1, 9 + i * yTranslate ), white );
                QCOMPARE( paintDevice.pixelColor( 9, 9 + i * yTranslate ), white ); // No shadow

                // Variable
                color = Qt::green;
                QCOMPARE( paintDevice.pixelColor( 1, 10 + i * yTranslate ), white );
                QCOMPARE( paintDevice.pixelColor( 1, 11 + i * yTranslate ), color );
                QCOMPARE( paintDevice.pixelColor( 1, 18 + i * yTranslate ), color );
                QCOMPARE( paintDevice.pixelColor( 1, 19 + i * yTranslate ), white );
                QCOMPARE( paintDevice.pixelColor( 9, 19 + i * yTranslate ), grayShadow );

                // Variable 2
                color = Qt::magenta;
                QCOMPARE( paintDevice.pixelColor( 1, 20 + i * yTranslate ), white );
                QCOMPARE( paintDevice.pixelColor( 1, 21 + i * yTranslate ), color );
                QCOMPARE( paintDevice.pixelColor( 1, 28 + i * yTranslate ), color );
                QCOMPARE( paintDevice.pixelColor( 1, 29 + i * yTranslate ), white );
                QCOMPARE( paintDevice.pixelColor( 9, 29 + i * yTranslate ), yellowShadow );

                // Filename pgn
                color = Qt::yellow;
                QCOMPARE( paintDevice.pixelColor( 1, 30 + i * yTranslate ), white );
                QCOMPARE( paintDevice.pixelColor( 1, 31 + i * yTranslate ), color );
                QCOMPARE( paintDevice.pixelColor( 1, 38 + i * yTranslate ), color );
                QCOMPARE( paintDevice.pixelColor( 1, 39 + i * yTranslate ), white );
                QCOMPARE( paintDevice.pixelColor( 9, 39 + i * yTranslate ), cyanShadow );

                // Filename svg
                color = Qt::cyan;
                QCOMPARE( paintDevice.pixelColor( 1, 40 + i * yTranslate ), white );
                QCOMPARE( paintDevice.pixelColor( 1, 41 + i * yTranslate ), color );
                QCOMPARE( paintDevice.pixelColor( 1, 48 + i * yTranslate ), color );
                QCOMPARE( paintDevice.pixelColor( 1, 49 + i * yTranslate ), white );
                QCOMPARE( paintDevice.pixelColor( 9, 49 + i * yTranslate ), magentaShadow );
        }
}
