/*  TestXmlLabel.cpp
 *
 *  Copyright (C) 2018  Jaye Evins <evins@snaught.com>
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

#include "TestXmlLabel.h"
#include "Test_Constants.h"

#include "model/XmlLabelCreator.h"
#include "model/XmlLabelParser.h"

#include "barcode/Backends.h"
#include "model/ColorNode.h"
#include "model/FrameRect.h"
#include "model/Markup.h"
#include "model/Model.h"
#include "model/PageRenderer.h"
#include "model/Size.h"

#include "model/ModelBarcodeObject.h"
#include "model/ModelBoxObject.h"
#include "model/ModelEllipseObject.h"
#include "model/ModelLineObject.h"
#include "model/ModelImageObject.h"
#include "model/ModelTextObject.h"

#include "model/Db.h"
#include "merge/Factory.h"
#include "merge/Merge.h"
#include "merge/TextCsvKeys.h"

#include <QtDebug>


QTEST_MAIN(TestXmlLabel)

using namespace glabels::model;
using namespace glabels::barcode;
using namespace glabels::merge;


namespace
{
	const double FONT_SCALE_FACTOR {0.75};
}


void TestXmlLabel::initTestCase()
{
	Settings::init();
	Db::init();
	Factory::init();
	Backends::init();
}


void TestXmlLabel::serializeDeserialize()
{
	Model* model = new Model();

	QList<ModelObject*> objects, outObjects;
	QByteArray buffer, outBuffer;

	//
	// Empty object list
	//
	QCOMPARE( objects.count(), 0 );
	XmlLabelCreator::serializeObjects( objects, model, buffer );
	outObjects = XmlLabelParser::deserializeObjects( buffer, model );
	QCOMPARE( objects.count(), outObjects.count() );
	QCOMPARE( objects, outObjects );
	XmlLabelCreator::serializeObjects( outObjects, model, outBuffer );
	QCOMPARE( buffer, outBuffer );

	//
	// All objects list
	//
	QImage png;
	QVERIFY( png.loadFromData( QByteArray::fromBase64( glabels::test::blue_8x8_png ), "PNG" ) );

	QString svgTemplate = QDir::tempPath().append( "/TestXmlLabel_XXXXXX.svg" ); // Note: directory separators canonicalized to slash by Qt path methods
	QTemporaryFile svgRelative( svgTemplate ); svgRelative.open(); svgRelative.write( glabels::test::cyan_8x8_svg ); svgRelative.close();

	bool lock = true, noLock = false, shadow = true, noShadow = false;
	ColorNode black( Qt::black ), white( Qt::white ), red( Qt::red ), green( Qt::green ), blue( Qt::blue );
	QTransform tMatrix( 1, 0, 0, 1, 50.0, 50.0 ), sMatrix( 0.5, 0, 0, 1.0, 0, 0 );
	Style bcStyle = Backends::defaultStyle();

	objects << new ModelBoxObject( 0, 1, 10, 20, lock, 2, red, green, tMatrix, shadow, 1, 2, 0.7, black );
	objects << new ModelEllipseObject( 1, 2, 30, 40, noLock, 3, black, white, sMatrix, shadow, 2, 3, 0.8, blue );
	objects << new ModelImageObject( 2, 3, 50, 50, lock, TextNode( false, "" ), tMatrix, noShadow, 3, 4, 0.9, white );
	objects << new ModelImageObject( 3, 4, 60, 70, noLock, "image3.png", png, sMatrix, shadow, 6, 4, 0.9, black );
	objects << new ModelImageObject( 4, 5, 70, 80, lock, "image4.svg", glabels::test::red_8x8_svg );
	objects << new ModelImageObject( 5, 6, 80, 90, noLock, TextNode( true, "${key}" ), tMatrix, shadow );
	objects << new ModelImageObject( 6, 7, 90, 100, lock, TextNode( false, "image6.jpg" ) ); // Will give warning on parse that embedded file missing
	objects << new ModelLineObject( 7, 8, 100, 110, 4, green, sMatrix, shadow, 5, 5, 0.5, red );
	objects << new ModelTextObject( 8, 9, 110, 120, lock, "text", "Serif", 12, QFont::Bold, true, true, red,
									Qt::AlignHCenter, Qt::AlignBottom, QTextOption::NoWrap, 1.3, false, sMatrix, shadow, 5, 5, 0.5, red );
	objects << new ModelBarcodeObject( 9, 10, 50, 50, noLock, bcStyle, true, true, QString("1234"), black, tMatrix );
	objects << new ModelImageObject( 10, 11, 8, 8, lock, TextNode( false, svgRelative.fileName() ) );

	QCOMPARE( objects.count(), 11 );

	buffer.clear();
	XmlLabelCreator::serializeObjects( objects, model, buffer );

	QVERIFY( svgRelative.remove() ); // Delete to make sure it's not read from file on parse

	QTest::ignoreMessage( QtWarningMsg, QRegularExpression( "^Embedded file \"[^\"]+image6.jpg\" missing\\. Trying actual file\\.$" ) );
	outObjects = XmlLabelParser::deserializeObjects( buffer, model );
	QCOMPARE( objects.count(), outObjects.count() );

	QString modelDirPath = model->dir().path() + "/";

	for ( int i = 0; i < objects.count(); i++ )
	{
		qDebug() << "object" << i;
		QVERIFY( objects.at(i)->id() != outObjects.at(i)->id() ); // Ids are generated and unique
		QCOMPARE( objects.at(i)->x0(), outObjects.at(i)->x0() );
		QCOMPARE( objects.at(i)->x0().pt(), (double)i );
		QCOMPARE( objects.at(i)->y0(), outObjects.at(i)->y0() );
		QCOMPARE( objects.at(i)->y0().pt(), (double)(i + 1) );
		QCOMPARE( objects.at(i)->w().pt(), outObjects.at(i)->w().pt() ); // Use `pt()` so invoke `qFuzzyCompare(double, double)` otherwise get rounding difference for Barcode
		QCOMPARE( objects.at(i)->h().pt(), outObjects.at(i)->h().pt() ); // Fuzzy
		QCOMPARE( objects.at(i)->lockAspectRatio(), outObjects.at(i)->lockAspectRatio() );
		QCOMPARE( objects.at(i)->lockAspectRatio(), (bool)((i + 1) % 2) );
		QCOMPARE( objects.at(i)->matrix(), outObjects.at(i)->matrix() );
		QCOMPARE( objects.at(i)->shadow(), outObjects.at(i)->shadow() );
		QCOMPARE( objects.at(i)->shadowX(), outObjects.at(i)->shadowX() );
		QCOMPARE( objects.at(i)->shadowY(), outObjects.at(i)->shadowY() );
		QCOMPARE( objects.at(i)->shadowOpacity(), outObjects.at(i)->shadowOpacity() );
		QVERIFY( objects.at(i)->shadowColorNode() == outObjects.at(i)->shadowColorNode() );
		QCOMPARE( objects.at(i)->naturalSize().w().pt(), outObjects.at(i)->naturalSize().w().pt() ); // Fuzzy
		QCOMPARE( objects.at(i)->naturalSize().h().pt(), outObjects.at(i)->naturalSize().h().pt() ); // Fuzzy

		QCOMPARE( objects.at(i)->text(), outObjects.at(i)->text() );
		QCOMPARE( objects.at(i)->fontFamily(), outObjects.at(i)->fontFamily() );
		QCOMPARE( objects.at(i)->fontSize(), outObjects.at(i)->fontSize() );
		QCOMPARE( objects.at(i)->fontWeight(), outObjects.at(i)->fontWeight() );
		QCOMPARE( objects.at(i)->fontItalicFlag(), outObjects.at(i)->fontItalicFlag() );
		QCOMPARE( objects.at(i)->fontUnderlineFlag(), outObjects.at(i)->fontUnderlineFlag() );
		QVERIFY( objects.at(i)->textColorNode() == outObjects.at(i)->textColorNode() );
		QCOMPARE( objects.at(i)->textHAlign(), outObjects.at(i)->textHAlign() );
		QCOMPARE( objects.at(i)->textVAlign(), outObjects.at(i)->textVAlign() );
		QCOMPARE( objects.at(i)->textWrapMode(), outObjects.at(i)->textWrapMode() );
		QCOMPARE( objects.at(i)->textLineSpacing(), outObjects.at(i)->textLineSpacing() );
		QCOMPARE( objects.at(i)->textAutoShrink(), outObjects.at(i)->textAutoShrink() );

		QCOMPARE( objects.at(i)->filenameNode().isField(), outObjects.at(i)->filenameNode().isField() );
		if ( i == 6 /*image6.jpg*/ )
		{
			// Not in data so absolute path set
			QCOMPARE( modelDirPath + objects.at(i)->filenameNode().data(), outObjects.at(i)->filenameNode().data() );
		}
		else
		{
			QCOMPARE( objects.at(i)->filenameNode().data(), outObjects.at(i)->filenameNode().data() );
		}

		if ( objects.at(i)->image() )
		{
			QCOMPARE( *(objects.at(i)->image()), *(outObjects.at(i)->image()) );
		}
		else
		{
			QCOMPARE( objects.at(i)->image(), outObjects.at(i)->image() );
		}
		QCOMPARE( objects.at(i)->svg(), outObjects.at(i)->svg() );

		QCOMPARE( objects.at(i)->lineWidth(), outObjects.at(i)->lineWidth() );
		QVERIFY( objects.at(i)->lineColorNode() == outObjects.at(i)->lineColorNode() );
		QVERIFY( objects.at(i)->fillColorNode() == outObjects.at(i)->fillColorNode() );

		QCOMPARE( objects.at(i)->bcData(), outObjects.at(i)->bcData() );
		QCOMPARE( objects.at(i)->bcTextFlag(), outObjects.at(i)->bcTextFlag() );
		QCOMPARE( objects.at(i)->bcChecksumFlag(), outObjects.at(i)->bcChecksumFlag() );
		QVERIFY( objects.at(i)->bcColorNode() == outObjects.at(i)->bcColorNode() );
		QVERIFY( !( objects.at(i)->bcStyle() != outObjects.at(i)->bcStyle() ) ); // Only != operator
		QCOMPARE( objects.at(i)->bcFormatDigits(), outObjects.at(i)->bcFormatDigits() );

		QCOMPARE( objects.at(i)->canText(), outObjects.at(i)->canText() );
		QCOMPARE( objects.at(i)->canFill(), outObjects.at(i)->canFill() );
		QCOMPARE( objects.at(i)->canLineColor(), outObjects.at(i)->canLineColor() );
		QCOMPARE( objects.at(i)->canLineWidth(), outObjects.at(i)->canLineWidth() );
	}

	outBuffer.clear();
	XmlLabelCreator::serializeObjects( outObjects, model, outBuffer );

	QCOMPARE( buffer, outBuffer );

	delete model->merge();
	delete model->variables();
	delete model;
}


void TestXmlLabel::writeReadFile()
{
	Model* model = new Model();

	// Make subdir in temp dir to use as model dir
	QTemporaryDir subDir;
	QVERIFY( subDir.isValid() );

	QString glabelsTemplate = subDir.path().append( "/TestXmlLabel_XXXXXX.glabels" ); // Note: directory separators canonicalized to slash by Qt path methods
	QTemporaryFile glabels( glabelsTemplate );
	glabels.open(); glabels.close();

	model->setFileName( glabels.fileName() );

	// Make subdir in subdir
	QString subSubTemplate = subDir.path().append( "/TestXmlLabel_XXXXXX" );
	QTemporaryDir subSubDir( subSubTemplate );
	QVERIFY( subSubDir.isValid() );

	QString relPath = model->dir().relativeFilePath( subSubDir.path() );
	QVERIFY( !relPath.contains( '/' ) ); // Make sure subdir of model dir

	QImage png;
	QVERIFY( png.loadFromData( QByteArray::fromBase64( glabels::test::blue_8x8_png ), "PNG" ) );

	// Make png file in temp dir (ie not in model dir)
	QImage pngAbsoluteImage;
	QVERIFY( pngAbsoluteImage.loadFromData( QByteArray::fromBase64( glabels::test::green_8x8_png ), "PNG" ) );
	QTemporaryFile pngAbsolute; pngAbsolute.open(); pngAbsolute.close(); pngAbsoluteImage.save( pngAbsolute.fileName(), "PNG" );

	// Make png file in model dir
	QImage pngRelativeImage;
	QVERIFY( pngRelativeImage.loadFromData( QByteArray::fromBase64( glabels::test::yellow_8x8_png ), "PNG" ) );
	QString pngTemplate = model->dir().path().append( "/TestXmlLabel_XXXXXX.png" );
	QTemporaryFile pngRelative( pngTemplate ); pngRelative.open(); pngRelative.close(); pngRelativeImage.save( pngRelative.fileName(), "PNG" );

	// Make svg file in subdir of model dir
	QString svgTemplate = subSubDir.path().append( "/TestXmlLabel_XXXXXX.svg" );
	QTemporaryFile svgRelative( svgTemplate ); svgRelative.open(); svgRelative.write( glabels::test::cyan_8x8_svg ); svgRelative.close();

	bool lock = true, noLock = false, shadow = true, noShadow = false;
	ColorNode black( Qt::black ), white( Qt::white ), red( Qt::red ), green( Qt::green ), blue( Qt::blue );
	QTransform tMatrix( 1, 0, 0, 1, 50.0, 50.0 ), sMatrix( 0.5, 0, 0, 1.0, 0, 0 );
	Style bcStyle = Backends::defaultStyle();

	///
	/// Add objects
	///
	model->addObject( new ModelBoxObject( 0, 1, 10, 20, noLock, 2, red, green, tMatrix, shadow, 1, 2, 0.7, black ) );
	model->addObject( new ModelEllipseObject( 1, 2, 30, 40, lock, 3, black, white, sMatrix, shadow, 2, 3, 0.8, blue ) );
	model->addObject( new ModelImageObject( 2, 3, 50, 50, noLock, TextNode( false, "" ), tMatrix, noShadow, 3, 4, 0.9, white ) );
	model->addObject( new ModelImageObject( 3, 4, 60, 70, lock, "image3.png", png, sMatrix, shadow, 6, 4, 0.9, black ) );
	model->addObject( new ModelImageObject( 4, 5, 70, 80, noLock, "image4.svg", glabels::test::red_8x8_svg ) );
	model->addObject( new ModelImageObject( 5, 6, 80, 90, lock, TextNode( true, "${key}" ), tMatrix, shadow ) );
	model->addObject( new ModelImageObject( 6, 7, 90, 100, noLock, TextNode( false, "image6.jpg" ) ) ); // Will give warning on parse that embedded file missing
	model->addObject( new ModelTextObject( 7, 8, 110, 120, lock, "text", "Serif", 12, QFont::Bold, true, true, red,
											Qt::AlignHCenter, Qt::AlignBottom, QTextOption::NoWrap, 1.3, false, sMatrix, shadow, 5, 5, 0.5, red ) );
	model->addObject( new ModelLineObject( 8, 9, 100, 110, 4, green, sMatrix, shadow, 5, 5, 0.5, red ) );
	model->addObject( new ModelBarcodeObject( 9, 10, 50, 50, lock, bcStyle, true, true, QString("1234"), black, tMatrix ) );
	model->addObject( new ModelImageObject( 10, 11, 8, 8, noLock, TextNode( false, pngAbsolute.fileName() ) ) );
	model->addObject( new ModelImageObject( 11, 12, 8, 8, lock, TextNode( false, pngRelative.fileName() ) ) );
	model->addObject( new ModelImageObject( 12, 13, 8, 8, noLock, TextNode( false, svgRelative.fileName() ) ) );

	QCOMPARE( model->objectList().size(), 13 );

	///
	/// Add template
	///
	Template tmplate( "Test Brand", "part", "desc", "testPaperId", 110, 410 );
	FrameRect* frame = new FrameRect( 120, 220, 5, 0, 0, "rect1" );
	tmplate.addFrame( frame );
	model->setTmplate( &tmplate ); // Copies

	///
	/// Add variables
	///
	Variables vars;
	Variable s( Variable::Type::STRING, "s", "initial", Variable::Increment::NEVER );
	Variable c( Variable::Type::COLOR, "c", "red", Variable::Increment::PER_COPY );
	Variable i( Variable::Type::INTEGER, "i", "123", Variable::Increment::PER_ITEM, "1" );
	Variable f( Variable::Type::FLOATING_POINT, "f", "12.3", Variable::Increment::PER_PAGE, "0.2" );
	model->variables()->addVariable( s );
	model->variables()->addVariable( c );
	model->variables()->addVariable( i );
	model->variables()->addVariable( f );
	QCOMPARE( model->variables()->size(), 4 );

	//
	// Add merge
	//
	Merge* merge = Factory::createMerge( TextCsvKeys::id() );
	QCOMPARE( merge->id(), TextCsvKeys::id() );

	model->setMerge( merge );
	QCOMPARE( model->merge(), merge );

	QString csvTemplate = subDir.path().append( "/TestXmlLabel_XXXXXX.csv" );
	QTemporaryFile csv( csvTemplate );
	csv.open();
	csv.write( "id,text\n1,text1\n2,text2\n3,text3\n" );
	csv.close();

	merge->setSource( csv.fileName() );
	QCOMPARE( merge->source(), csv.fileName() );

	QCOMPARE( merge->recordList().size(), 3 );

	model->setRotate( true );
	QVERIFY( model->rotate() );

	///
	/// Write to file and read
	///
	XmlLabelCreator::writeFile( model, glabels.fileName() );

	QCOMPARE( model->dir(), QFileInfo( glabels.fileName() ).dir() );

	// Copy before deletion else nulled
	QString pngAbsoluteFileName = pngAbsolute.fileName();
	QString pngRelativeFileName = model->dir().relativeFilePath( pngRelative.fileName() );
	QString svgRelativeFileName = model->dir().relativeFilePath( svgRelative.fileName() );

	QFileInfo pngAbsoluteFileInfo( pngAbsoluteFileName );
	QVERIFY( pngAbsoluteFileInfo.isAbsolute() );
	QFileInfo pngRelativeFileInfo( pngRelativeFileName );
	QVERIFY( pngRelativeFileInfo.isRelative() );
	QFileInfo svgRelativeFileInfo( svgRelativeFileName );
	QVERIFY( svgRelativeFileInfo.isRelative() );

	// Delete to make sure they're not read from file on parse
	QVERIFY( pngAbsolute.remove() );
	QVERIFY( pngRelative.remove() );
	QVERIFY( svgRelative.remove() );

	QTest::ignoreMessage( QtWarningMsg, QRegularExpression( "^Embedded file \"[^\"]+image6.jpg\" missing\\. Trying actual file\\.$" ) );
	Model* readModel = XmlLabelParser::readFile( glabels.fileName() );
	QVERIFY( readModel );
	QCOMPARE( readModel->dir(), model->dir() );
	QCOMPARE( readModel->fileName(), model->fileName() );

	QCOMPARE( readModel->tmplate()->brand(), model->tmplate()->brand() );
	QCOMPARE( readModel->tmplate()->part(), model->tmplate()->part() );
	QCOMPARE( readModel->tmplate()->description(), model->tmplate()->description() );
	QCOMPARE( readModel->tmplate()->paperId(), model->tmplate()->paperId() );
	QCOMPARE( readModel->tmplate()->pageWidth().pt(), model->tmplate()->pageWidth().pt() );
	QCOMPARE( readModel->tmplate()->pageHeight().pt(), model->tmplate()->pageHeight().pt() );

	QCOMPARE( readModel->frame()->id(), model->frame()->id() );
	QCOMPARE( readModel->frame()->w().pt(), model->frame()->w().pt() );
	QCOMPARE( readModel->frame()->h().pt(), model->frame()->h().pt() );

	QCOMPARE( readModel->rotate(), model->rotate() );
	QCOMPARE( readModel->w(), model->w() );
	QCOMPARE( readModel->h(), model->h() );

	const QList<ModelObject*>& readObjects = readModel->objectList();
	const QList<ModelObject*>& modelObjects = model->objectList();
	QCOMPARE( readObjects.size(), modelObjects.size() );

	QString modelDirPath = model->dir().path() + "/";

	for ( int i = 0; i < readObjects.count(); i++ )
	{
		qDebug() << "object" << i;
		QVERIFY( readObjects.at(i)->id() != modelObjects.at(i)->id() ); // Ids are generated and unique
		QCOMPARE( readObjects.at(i)->x0(), modelObjects.at(i)->x0() );
		QCOMPARE( readObjects.at(i)->x0().pt(), (double)i );
		QCOMPARE( readObjects.at(i)->y0(), modelObjects.at(i)->y0() );
		QCOMPARE( readObjects.at(i)->y0().pt(), (double)(i + 1) );
		QCOMPARE( readObjects.at(i)->w().pt(), modelObjects.at(i)->w().pt() ); // Use `pt()` so invoke `qFuzzyCompare(double, double)` otherwise get rounding difference for Barcode
		QCOMPARE( readObjects.at(i)->h().pt(), modelObjects.at(i)->h().pt() ); // Fuzzy
		QCOMPARE( readObjects.at(i)->lockAspectRatio(), modelObjects.at(i)->lockAspectRatio() );
		QCOMPARE( readObjects.at(i)->lockAspectRatio(), (bool)(i % 2) );
		QCOMPARE( readObjects.at(i)->matrix(), modelObjects.at(i)->matrix() );
		QCOMPARE( readObjects.at(i)->shadow(), modelObjects.at(i)->shadow() );
		QCOMPARE( readObjects.at(i)->shadowX(), modelObjects.at(i)->shadowX() );
		QCOMPARE( readObjects.at(i)->shadowY(), modelObjects.at(i)->shadowY() );
		QCOMPARE( readObjects.at(i)->shadowOpacity(), modelObjects.at(i)->shadowOpacity() );
		QVERIFY( readObjects.at(i)->shadowColorNode() == modelObjects.at(i)->shadowColorNode() );
		QCOMPARE( readObjects.at(i)->naturalSize().w().pt(), modelObjects.at(i)->naturalSize().w().pt() ); // Fuzzy
		QCOMPARE( readObjects.at(i)->naturalSize().h().pt(), modelObjects.at(i)->naturalSize().h().pt() ); // Fuzzy

		QCOMPARE( readObjects.at(i)->text(), modelObjects.at(i)->text() );
		QCOMPARE( readObjects.at(i)->fontFamily(), modelObjects.at(i)->fontFamily() );
		QCOMPARE( readObjects.at(i)->fontSize(), modelObjects.at(i)->fontSize() );
		QCOMPARE( readObjects.at(i)->fontWeight(), modelObjects.at(i)->fontWeight() );
		QCOMPARE( readObjects.at(i)->fontItalicFlag(), modelObjects.at(i)->fontItalicFlag() );
		QCOMPARE( readObjects.at(i)->fontUnderlineFlag(), modelObjects.at(i)->fontUnderlineFlag() );
		QVERIFY( readObjects.at(i)->textColorNode() == modelObjects.at(i)->textColorNode() );
		QCOMPARE( readObjects.at(i)->textHAlign(), modelObjects.at(i)->textHAlign() );
		QCOMPARE( readObjects.at(i)->textVAlign(), modelObjects.at(i)->textVAlign() );
		QCOMPARE( readObjects.at(i)->textWrapMode(), modelObjects.at(i)->textWrapMode() );
		QCOMPARE( readObjects.at(i)->textLineSpacing(), modelObjects.at(i)->textLineSpacing() );
		QCOMPARE( readObjects.at(i)->textAutoShrink(), modelObjects.at(i)->textAutoShrink() );

		QCOMPARE( readObjects.at(i)->filenameNode().isField(), modelObjects.at(i)->filenameNode().isField() );
		if ( i == 6 /*image6.jpg*/ )
		{
			// Not in data so absolute path set
			QCOMPARE( readObjects.at(i)->filenameNode().data(), modelDirPath + modelObjects.at(i)->filenameNode().data() );
		}
		else if ( modelObjects.at(i)->filenameNode().data().startsWith( modelDirPath ) )
		{
			// Made relative to model dir
			QCOMPARE( modelDirPath + readObjects.at(i)->filenameNode().data(), modelObjects.at(i)->filenameNode().data() );
		}
		else
		{
			QCOMPARE( readObjects.at(i)->filenameNode().data(), modelObjects.at(i)->filenameNode().data() );
		}

		if ( readObjects.at(i)->image() )
		{
			QCOMPARE( *(readObjects.at(i)->image()), *(modelObjects.at(i)->image()) );
		}
		else
		{
			QCOMPARE( readObjects.at(i)->image(), modelObjects.at(i)->image() );
		}
		QCOMPARE( readObjects.at(i)->svg(), modelObjects.at(i)->svg() );

		QCOMPARE( readObjects.at(i)->lineWidth(), modelObjects.at(i)->lineWidth() );
		QVERIFY( readObjects.at(i)->lineColorNode() == modelObjects.at(i)->lineColorNode() );
		QVERIFY( readObjects.at(i)->fillColorNode() == modelObjects.at(i)->fillColorNode() );

		QCOMPARE( readObjects.at(i)->bcData(), modelObjects.at(i)->bcData() );
		QCOMPARE( readObjects.at(i)->bcTextFlag(), modelObjects.at(i)->bcTextFlag() );
		QCOMPARE( readObjects.at(i)->bcChecksumFlag(), modelObjects.at(i)->bcChecksumFlag() );
		QVERIFY( readObjects.at(i)->bcColorNode() == modelObjects.at(i)->bcColorNode() );
		QVERIFY( !( readObjects.at(i)->bcStyle() != modelObjects.at(i)->bcStyle() ) ); // Only != operator
		QCOMPARE( readObjects.at(i)->bcFormatDigits(), modelObjects.at(i)->bcFormatDigits() );

		QCOMPARE( readObjects.at(i)->canText(), modelObjects.at(i)->canText() );
		QCOMPARE( readObjects.at(i)->canFill(), modelObjects.at(i)->canFill() );
		QCOMPARE( readObjects.at(i)->canLineColor(), modelObjects.at(i)->canLineColor() );
		QCOMPARE( readObjects.at(i)->canLineWidth(), modelObjects.at(i)->canLineWidth() );
	}

	QCOMPARE( readObjects[10]->filenameNode().data(), pngAbsoluteFileName );
	QCOMPARE( readObjects[11]->filenameNode().data(), pngRelativeFileName );
	QCOMPARE( readObjects[12]->filenameNode().data(), svgRelativeFileName );

	QCOMPARE( readModel->variables()->size(), model->variables()->size() );
	for ( const auto& modelV : *model->variables() )
	{
		QVERIFY( readModel->variables()->hasVariable( modelV.name() ) );
		const auto& readV = readModel->variables()->value( modelV.name() );
		QCOMPARE( readV.type(), modelV.type() );
		QCOMPARE( readV.initialValue(), modelV.initialValue() );
		if ( readV.type() == Variable::Type::INTEGER || readV.type() == Variable::Type::FLOATING_POINT )
		{
			QCOMPARE( readV.stepSize(), modelV.stepSize() );
			QCOMPARE( readV.increment(), modelV.increment() );
		}
		QCOMPARE( readV.value(), modelV.value() );
	}

	QCOMPARE( readModel->merge()->id(), model->merge()->id() );
	QCOMPARE( readModel->merge()->source(), model->merge()->source() );
	QCOMPARE( readModel->merge()->recordList().size(), model->merge()->recordList().size() );
	for ( int i = 0; i < readModel->merge()->recordList().size(); i++ )
	{
		QCOMPARE( readModel->merge()->recordList().at(i)->keys(), model->merge()->recordList().at(i)->keys() );
		QCOMPARE( readModel->merge()->recordList().at(i)->values(), model->merge()->recordList().at(i)->values() );
	}

	delete readModel->merge();
	delete readModel->variables();
	delete readModel;

	delete model->merge();
	delete model->variables();
	delete model;
}


void TestXmlLabel::parser_3ReadFile()
{
	// Current path is "build/model/unit_tests" so go up 3 levels
	QFileInfo glabelsFileInfo( "../../../model/unit_tests/data/glabels-3/crew-orientation-name-tags-7.glabels" );
	QVERIFY( glabelsFileInfo.isReadable() );

	Model* model = XmlLabelParser::readFile( glabelsFileInfo.filePath() );
	QVERIFY( model );

	QCOMPARE( model->fileName(), glabelsFileInfo.filePath() );

	QCOMPARE( model->tmplate()->brand(), QString( "Avery" ) );
	QCOMPARE( model->tmplate()->part(), QString( "5395" ) );
	QCOMPARE( model->tmplate()->description(), QString( "Name Badge Labels" ) );
	QCOMPARE( model->tmplate()->paperId(), QString( "US-Letter" ) );
	QCOMPARE( model->tmplate()->pageWidth().in(), 8.5 );
	QCOMPARE( model->tmplate()->pageHeight().in(), 11.0 );

	QCOMPARE( model->frame()->id(), QString( "0" ) );
	const FrameRect* frameRect = dynamic_cast<const FrameRect*>( model->frame() );
	QVERIFY( frameRect );
	QCOMPARE( frameRect->w().in(), 3.375 );
	QCOMPARE( frameRect->h().in(), 2.33333 );
	QCOMPARE( frameRect->r().in(), 0.1875 );
	QCOMPARE( frameRect->xWaste().in(), 0.0625 );
	QCOMPARE( frameRect->yWaste().in(), 0.0625 );

	QCOMPARE( model->frame()->markups().size(), 1 );
	MarkupMargin* markupMargin = dynamic_cast<MarkupMargin*>( model->frame()->markups()[0] );
	QVERIFY( markupMargin );
	QCOMPARE( markupMargin->xSize().in(), 0.0625 );
	QCOMPARE( markupMargin->ySize().in(), 0.0625 );

	QCOMPARE( model->frame()->layouts().size(), 1 );
	QCOMPARE( model->frame()->layouts()[0].nx(), 2 );
	QCOMPARE( model->frame()->layouts()[0].ny(), 4 );
	QCOMPARE( model->frame()->layouts()[0].x0().in(), 0.6875 );
	QCOMPARE( model->frame()->layouts()[0].y0().in(), 0.583333 );
	QCOMPARE( model->frame()->layouts()[0].dx().in(), 3.75 );
	QCOMPARE( model->frame()->layouts()[0].dy().in(), 2.5 );

	QCOMPARE( model->rotate(), false );

	QCOMPARE( model->objectList().size(), 4 );

	ModelTextObject* modelTextObject0 = dynamic_cast<ModelTextObject*>( model->objectList()[0] );
	QVERIFY( modelTextObject0 );
	QCOMPARE( modelTextObject0->x0().in(), 0.150603 );
	QCOMPARE( modelTextObject0->y0().in(), 0.2625 );
	// Width and height set to naturalSize()
	QCOMPARE( modelTextObject0->lockAspectRatio(), false );
	QCOMPARE( modelTextObject0->matrix(), QTransform( 1, 0, 0, 1, 0, 0 ) );
	QCOMPARE( modelTextObject0->shadow(), false );
	QCOMPARE( modelTextObject0->text(), QString( "Hello, my name is" ) );
	QCOMPARE( modelTextObject0->fontFamily(), QString( "Sans" ) );
	QCOMPARE( modelTextObject0->fontSize(), 16 * FONT_SCALE_FACTOR );
	QCOMPARE( modelTextObject0->fontWeight(), QFont::Bold );
	QCOMPARE( modelTextObject0->fontItalicFlag(), false );
	QCOMPARE( modelTextObject0->textLineSpacing(), 1.0 );
	QCOMPARE( modelTextObject0->textAutoShrink(), false );
	QCOMPARE( modelTextObject0->textColorNode().color(), QColor::fromRgba( 0xff3366ff ) ); // QColor uses ARGB
	QCOMPARE( modelTextObject0->textHAlign(), Qt::AlignLeft );

	ModelTextObject* modelTextObject1 = dynamic_cast<ModelTextObject*>( model->objectList()[1] );
	QVERIFY( modelTextObject1 );
	QCOMPARE( modelTextObject1->x0().in(), 0.150603 );
	QCOMPARE( modelTextObject1->y0().in(), 0.645 );
	// Width and height set to naturalSize()
	QCOMPARE( modelTextObject1->lockAspectRatio(), false );
	QCOMPARE( modelTextObject1->matrix(), QTransform( 1, 0, 0, 1, 0, 0 ) );
	QCOMPARE( modelTextObject1->shadow(), false );
	QCOMPARE( modelTextObject1->text(), QString( "${Name}" ) );
	QCOMPARE( modelTextObject1->fontFamily(), QString( "Sans" ) );
	QCOMPARE( modelTextObject1->fontSize(), 20 * FONT_SCALE_FACTOR );
	QCOMPARE( modelTextObject1->fontWeight(), QFont::Normal );
	QCOMPARE( modelTextObject1->fontItalicFlag(), false );
	QCOMPARE( modelTextObject1->textLineSpacing(), 1.0 );
	QCOMPARE( modelTextObject1->textAutoShrink(), false );
	QCOMPARE( modelTextObject1->textColorNode().color(), QColor::fromRgba( 0xff000000 ) );
	QCOMPARE( modelTextObject1->textHAlign(), Qt::AlignLeft );

	ModelTextObject* modelTextObject2 = dynamic_cast<ModelTextObject*>( model->objectList()[2] );
	QVERIFY( modelTextObject2 );
	QCOMPARE( modelTextObject2->x0().in(), 0.150603 );
	QCOMPARE( modelTextObject2->y0().in(), 1.14 );
	// Width and height set to naturalSize()
	QCOMPARE( modelTextObject2->lockAspectRatio(), false );
	QCOMPARE( modelTextObject2->matrix(), QTransform( 1, 0, 0, 1, 0, 0 ) );
	QCOMPARE( modelTextObject2->shadow(), false );
	QCOMPARE( modelTextObject2->text(), QString( "Department: ${Department}" ) );
	QCOMPARE( modelTextObject2->fontFamily(), QString( "Sans" ) );
	QCOMPARE( modelTextObject2->fontSize(), 11 * FONT_SCALE_FACTOR );
	QCOMPARE( modelTextObject2->fontWeight(), QFont::Normal );
	QCOMPARE( modelTextObject2->fontItalicFlag(), false );
	QCOMPARE( modelTextObject2->textLineSpacing(), 1.0 );
	QCOMPARE( modelTextObject2->textAutoShrink(), false );
	QCOMPARE( modelTextObject2->textColorNode().color(), QColor::fromRgba( 0xff000000 ) );
	QCOMPARE( modelTextObject2->textHAlign(), Qt::AlignLeft );

	ModelBarcodeObject* modelBarcodeObject3 = dynamic_cast<ModelBarcodeObject*>( model->objectList()[3] );
	QVERIFY( modelBarcodeObject3 );
	QCOMPARE( modelBarcodeObject3->x0().in(), 0.150603 );
	QCOMPARE( modelBarcodeObject3->y0().in(), 1.395 );
	QCOMPARE( modelBarcodeObject3->w().in(), 3.06944 );
	QCOMPARE( modelBarcodeObject3->h().in(), 0.847222 );
	QCOMPARE( modelBarcodeObject3->lockAspectRatio(), false );
	QCOMPARE( modelBarcodeObject3->matrix(), QTransform( 1, 0, 0, 1, 0, 0 ) );
	QCOMPARE( modelBarcodeObject3->shadow(), false );

	QCOMPARE( modelBarcodeObject3->bcData(), QString( "${SN}" ) );
	QVERIFY( modelBarcodeObject3->bcTextFlag() );
	QVERIFY( modelBarcodeObject3->bcChecksumFlag() );
	QCOMPARE( modelBarcodeObject3->bcColorNode().color(), QColor::fromRgba( 0xff000000 ) );
	QCOMPARE( modelBarcodeObject3->bcStyle().fullId(), QString( "code39" ) );
	QCOMPARE( modelBarcodeObject3->bcFormatDigits(), 10 );

	QVERIFY( model->merge() );
	QVERIFY( !model->merge()->source().isEmpty() ); // Merge source hacked to work relatively so not realistic
	QCOMPARE( model->merge()->recordList().size(), 4 );

	QCOMPARE( model->merge()->recordList()[0]->keys().size(), 3 );
	QList<QString> keys, values0, values1, values2, values3;
	keys << "Department" << "Name" << "SN";
	values0 << "Management" << "Jim Kirk" << "SC937-0176 CEC";
	values1 << "Sciences" << "Mr. Spock" << "S179-276SP";
	values2 << "Medicine" << "Leonard McCoy" << "unknown";
	values3 << "Engineering" << "Montgomery Scott" << "SE-197-54T";

	QCOMPARE( model->merge()->recordList()[0]->keys(), keys );
	QCOMPARE( model->merge()->recordList()[0]->values(), values0 );
	QCOMPARE( model->merge()->recordList()[1]->keys(), keys );
	QCOMPARE( model->merge()->recordList()[1]->values(), values1 );
	QCOMPARE( model->merge()->recordList()[2]->keys(), keys );
	QCOMPARE( model->merge()->recordList()[2]->values(), values2 );
	QCOMPARE( model->merge()->recordList()[3]->keys(), keys );
	QCOMPARE( model->merge()->recordList()[3]->values(), values3 );

	delete model->merge();
	delete model->variables();
	delete model;
}


void TestXmlLabel::parser_3Barcode()
{
	QTemporaryFile glabels( "TestXmlLabel_XXXXXX.glabels" );
	glabels.open();
	glabels.write( "<?xml version=\"1.0\"?>" );
	glabels.write( "<Glabels-document xmlns=\"http://glabels.org/xmlns/2.3/\">" );
	glabels.write( "<Objects>" );
	glabels.write( "<Object-barcode backend=\"built-in\" style=\"Code39Ext\"/>" );
	glabels.write( "<Object-barcode backend=\"libiec16022\" style=\"IEC16022\"/>" );
	glabels.write( "<Object-barcode backend=\"libqrencode\" style=\"IEC18004\"/>" );
	glabels.write( "<Object-barcode backend=\"gnu-barcode\" style=\"UPC-A+2\"/>" );
	glabels.write( "<Object-barcode backend=\"zint\" style=\"GS1-128\" data=\"[20]01\"/>" );
	glabels.write( "</Objects>" );
	glabels.write( "</Glabels-document>" );
	glabels.close();

	Model* model = XmlLabelParser::readFile( glabels.fileName() );
	QVERIFY( model );

	QCOMPARE( model->objectList().size(), 5 );

	ModelBarcodeObject* modelBarcodeObject;

	modelBarcodeObject = dynamic_cast<ModelBarcodeObject*>( model->objectList()[0] );
	QVERIFY( modelBarcodeObject );
	QCOMPARE( modelBarcodeObject->bcStyle().fullId(), QString( "code39ext" ) );

	modelBarcodeObject = dynamic_cast<ModelBarcodeObject*>( model->objectList()[1] );
	QVERIFY( modelBarcodeObject );
	QCOMPARE( modelBarcodeObject->bcStyle().fullId(), QString( "datamatrix" ) );

	modelBarcodeObject = dynamic_cast<ModelBarcodeObject*>( model->objectList()[2] );
	QVERIFY( modelBarcodeObject );
	if ( Backends::style( "qrencode", "qrcode" ) != Backends::defaultStyle() )
	{
		QCOMPARE( modelBarcodeObject->bcStyle().fullId(), QString( "qrencode::qrcode" ) );
	}
	else if ( Backends::style( "zint", "qr" ) != Backends::defaultStyle() )
	{
		QCOMPARE( modelBarcodeObject->bcStyle().fullId(), QString( "zint::qr" ) );
	}
	else
	{
		QCOMPARE( modelBarcodeObject->bcStyle().fullId(), QString( "code39" ) );
	}

	modelBarcodeObject = dynamic_cast<ModelBarcodeObject*>( model->objectList()[3] );
	QVERIFY( modelBarcodeObject );
	if ( Backends::style( "gnu-barcode", "upc-a+2" ) != Backends::defaultStyle() )
	{
		QCOMPARE( modelBarcodeObject->bcStyle().fullId(), QString( "gnu-barcode::upc-a+2" ) );
	}
	else
	{
		QCOMPARE( modelBarcodeObject->bcStyle().fullId(), QString( "code39" ) );
	}

	modelBarcodeObject = dynamic_cast<ModelBarcodeObject*>( model->objectList()[4] );
	QVERIFY( modelBarcodeObject );
	if ( Backends::style( "zint", "gs1-128" ) != Backends::defaultStyle() )
	{
		QCOMPARE( modelBarcodeObject->bcStyle().fullId(), QString( "zint::gs1-128" ) );
	}
	else
	{
		QCOMPARE( modelBarcodeObject->bcStyle().fullId(), QString( "code39" ) );
	}

	delete model->merge();
	delete model->variables();
	delete model;
}
