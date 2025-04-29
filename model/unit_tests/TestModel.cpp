/*  TestModel.cpp
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

#include "TestModel.h"

#include "model/Model.h"
#include "model/ModelBoxObject.h"
#include "model/ModelEllipseObject.h"
#include "model/ModelLineObject.h"
#include "model/ModelTextObject.h"
#include "model/FrameRect.h"
#include "model/FrameContinuous.h"
#include "model/Region.h"
#include "model/Settings.h"

#include "merge/Factory.h"
#include "merge/Merge.h"
#include "merge/None.h"
#include "merge/TextCsv.h"
#include "merge/TextCsvKeys.h"

#include <QDebug>
#include <QRegularExpression>


QTEST_MAIN(TestModel)

using namespace glabels::model;
using namespace glabels::merge;


void TestModel::initTestCase()
{
	Factory::init();
	Settings::init();
}


void TestModel::model()
{
	Model model;
	QVERIFY( model.isModified() );
	model.clearModified();
	QVERIFY( !model.isModified() );

	QVERIFY( model.shortName().contains( QRegularExpression( "^Untitled[1-9][0-9]*$" ) ) );
	model.setFileName( "dir/file1.ext" );
	QCOMPARE( model.fileName(), QString( "dir/file1.ext" ) );
	QCOMPARE( model.shortName(), QString( "file1" ) );
	QVERIFY( !model.isModified() );

	QCOMPARE( model.w(), Distance( 0 ) );
	QCOMPARE( model.h(), Distance( 0 ) );

	Template tmplate( "Test Brand", "part", "desc", "testPaperId", 100, 400 );
	FrameRect* frame = new FrameRect( 100, 200, 5, 0, 0, "rect1" );
	QVERIFY( frame->w() != frame->h() );
	tmplate.addFrame( frame );
	model.setTmplate( &tmplate ); // Copies
	QCOMPARE( model.tmplate()->brand(), QString( "Test Brand" ) );
	QCOMPARE( model.tmplate()->part(), QString( "part" ) );
	QCOMPARE( model.tmplate()->description(), QString( "desc" ) );
	QCOMPARE( model.tmplate()->paperId(), QString( "testPaperId" ) );
	QCOMPARE( model.tmplate()->pageWidth(), Distance( 100 ) );
	QCOMPARE( model.tmplate()->pageHeight(), Distance( 400 ) );
	QVERIFY( model.isModified() );

	QVERIFY( model.frame()->id() == frame->id() );
	QCOMPARE( model.w(), Distance( 100 ) );
	QCOMPARE( model.h(), Distance( 200 ) );
	QCOMPARE( model.w(), frame->w() );
	QCOMPARE( model.h(), frame->h() );

	model.clearModified();
	QVERIFY( !model.isModified() );

	QVERIFY( !model.rotate() );
	model.setRotate( false );
	QVERIFY( !model.rotate() );
	QVERIFY( !model.isModified() );
	model.setRotate( true );
	QVERIFY( model.rotate() );
	QVERIFY( model.isModified() );

	QCOMPARE( model.w(), frame->h() );
	QCOMPARE( model.h(), frame->w() );

	model.setRotate( false );
	QVERIFY( !model.rotate() );

	model.clearModified();
	QVERIFY( !model.isModified() );

	model.setH( 300 ); // Default does nothing
	QCOMPARE( model.h(), Distance( 200 ) );
	QVERIFY( model.isModified() ); // Set anyway

	// Continuous frame implements setH()
	Template tmplate2( "Test Brand2", "part2", "desc2", "testPaperId2", 100, 400 );
	FrameContinuous* frame2 = new FrameContinuous( 100, 0, 500, 200, "continuous1" );
	QCOMPARE( frame2->h(), Distance( 200 ) );
	tmplate2.addFrame( frame2 );
	model.setTmplate( &tmplate2 );
	QVERIFY( model.frame()->id() == frame2->id() );
	QCOMPARE( model.w(), Distance( 100 ) );
	QCOMPARE( model.h(), Distance( 200 ) );
	QCOMPARE( model.w(), frame2->w() );
	QCOMPARE( model.h(), frame2->h() );

	model.clearModified();
	QVERIFY( !model.isModified() );

	model.setH( 300 );
	QCOMPARE( model.h(), Distance( 300 ) );
	QVERIFY( model.isModified() );

	//
	// Objects
	//
	ColorNode black( Qt::black );
	ModelObject* ellipse = new ModelEllipseObject( 1, 0, 100, 100, false, 1, black, black );
	ModelObject* box = new ModelBoxObject( 1, 100, 100, 100, false, 1, black, black );
	ModelObject* line = new ModelLineObject( 1, 200, 99 /*dx*/, 1 /*dy*/, 1.0, black );
	ModelObject* text = new ModelTextObject( 1, 201, 100, 30, false, "", "Sans", 10, QFont::Normal, false, false, black, Qt::AlignLeft, Qt::AlignTop, QTextOption::WordWrap, 1, false );

	model.clearModified();
	QVERIFY( !model.isModified() );

	model.addObject( ellipse );
	QVERIFY( model.isModified() );
	model.addObject( box );
	model.addObject( line );
	model.addObject( text );

	QCOMPARE( model.objectList().size(), 4 );

	ModelObject* line2 = new ModelLineObject( 1, 231, 100 /*dx*/, 1 /*dy*/, 1.0, black );
	model.addObject( line2 );
	QCOMPARE( model.objectList().size(), 5 );

	model.clearModified();
	QVERIFY( !model.isModified() );

	model.deleteObject( line2 );
	QCOMPARE( model.objectList().size(), 4 );
	QVERIFY( model.isModified() );

	ModelObject* object;

	object = model.objectAt( 1 /*scale*/, 1, 200 );
	QVERIFY( object );
	QVERIFY( dynamic_cast<ModelLineObject*>(object) );
	QCOMPARE( object->id(), line->id() );

	object = model.objectAt( 1 /*scale*/, 100, 150 );
	QVERIFY( object );
	QVERIFY( dynamic_cast<ModelBoxObject*>(object) );
	QCOMPARE( object->id(), box->id() );

	object = model.objectAt( 1 /*scale*/, 50, 0 );
	QVERIFY( object );
	QVERIFY( dynamic_cast<ModelEllipseObject*>(object) );
	QCOMPARE( object->id(), ellipse->id() );

	object = model.objectAt( 1 /*scale*/, 1 + 3, 201 + 3 ); // Allow for text offset
	QVERIFY( object );
	QVERIFY( dynamic_cast<ModelTextObject*>(object) );
	QCOMPARE( object->id(), text->id() );

	//
	// Selection
	//
	QVERIFY( model.isSelectionEmpty() );
	QVERIFY( !model.isSelectionAtomic() );
	QVERIFY( model.getSelection().isEmpty() );
	QVERIFY( !model.getFirstSelectedObject() );

	QVERIFY( !model.canSelectionText() );
	QVERIFY( !model.canSelectionFill() );
	QVERIFY( !model.canSelectionLineColor() );
	QVERIFY( !model.canSelectionLineWidth() );

	model.selectAll();
	QVERIFY( !model.isSelectionEmpty() );
	QVERIFY( !model.isSelectionAtomic() );
	QVERIFY( !model.getSelection().isEmpty() );
	QCOMPARE( model.getSelection().size(), 4 );
	QCOMPARE( model.getSelection().first()->id(), ellipse->id() );
	QCOMPARE( model.getSelection().at(1)->id(), box->id() );
	QCOMPARE( model.getSelection().at(2)->id(), line->id() );
	QCOMPARE( model.getSelection().at(3)->id(), text->id() );
	QVERIFY( model.getFirstSelectedObject() );
	QCOMPARE( model.getFirstSelectedObject()->id(), ellipse->id() );

	QVERIFY( model.canSelectionText() );
	QVERIFY( model.canSelectionFill() );
	QVERIFY( model.canSelectionLineColor() );
	QVERIFY( model.canSelectionLineWidth() );

	model.unselectAll();
	QVERIFY( model.isSelectionEmpty() );
	QVERIFY( !model.isSelectionAtomic() );
	QVERIFY( model.getSelection().isEmpty() );
	QVERIFY( !model.getFirstSelectedObject() );

	model.selectObject( text );
	QVERIFY( !model.isSelectionEmpty() );
	QVERIFY( model.isSelectionAtomic() );
	QCOMPARE( model.getSelection().size(), 1 );
	QCOMPARE( model.getFirstSelectedObject()->id(), text->id() );

	QVERIFY( model.canSelectionText() );
	QVERIFY( !model.canSelectionFill() );
	QVERIFY( !model.canSelectionLineColor() );
	QVERIFY( !model.canSelectionLineWidth() );

	model.unselectObject( text );
	QVERIFY( model.isSelectionEmpty() );

	model.selectObject( line );
	QVERIFY( !model.isSelectionEmpty() );
	QVERIFY( model.isSelectionAtomic() );
	QCOMPARE( model.getSelection().size(), 1 );
	QCOMPARE( model.getFirstSelectedObject()->id(), line->id() );

	QVERIFY( !model.canSelectionText() );
	QVERIFY( !model.canSelectionFill() );
	QVERIFY( model.canSelectionLineColor() );
	QVERIFY( model.canSelectionLineWidth() );

	model.unselectAll();
	QVERIFY( model.isSelectionEmpty() );

	double margin = 0.5; // Allow 0.5pt margin
	Region region( 1 - margin, 302 - margin, 101 + margin /*x2*/, 302 + margin /*y2*/ ); // Outside all objects
	model.selectRegion( region );
	QVERIFY( model.getSelection().isEmpty() );
	QVERIFY( model.isSelectionEmpty() );

	region.setY1( 0 - margin ); // Ellipse
	region.setY2( 100 + margin );
	model.selectRegion( region );
	QVERIFY( !model.isSelectionEmpty() );
	QVERIFY( model.isSelectionAtomic() );
	QCOMPARE( model.getSelection().size(), 1 );
	QCOMPARE( model.getFirstSelectedObject()->id(), ellipse->id() );

	QVERIFY( !model.canSelectionText() );
	QVERIFY( model.canSelectionFill() );
	QVERIFY( model.canSelectionLineColor() );
	QVERIFY( model.canSelectionLineWidth() );

	region.setY1( 200 - margin ); // Line
	region.setY2( 201 + margin );
	model.selectRegion( region );
	QVERIFY( !model.isSelectionEmpty() );
	QVERIFY( !model.isSelectionAtomic() ); // Accumulative
	QCOMPARE( model.getSelection().size(), 2 );
	QCOMPARE( model.getSelection().at(0)->id(), ellipse->id() );
	QCOMPARE( model.getSelection().at(1)->id(), line->id() );

	model.unselectObject( ellipse );
	QVERIFY( !model.isSelectionEmpty() );
	QVERIFY( model.isSelectionAtomic() );
	QCOMPARE( model.getSelection().size(), 1 );
	QCOMPARE( model.getFirstSelectedObject()->id(), line->id() );

	model.unselectAll();
	QVERIFY( model.isSelectionEmpty() );

	// TODO: Operations on selections etc
}


void TestModel::saveRestore()
{
	Model* model = new Model;
	QVERIFY( model->isModified() );
	model->clearModified();
	QVERIFY( !model->isModified() );

	//
	// Set template/frame
	//
	Template tmplate( "Test Brand", "part", "desc", "testPaperId", 110, 410 );
	FrameRect* frame = new FrameRect( 120, 220, 5, 0, 0, "rect1" );
	tmplate.addFrame( frame );
	model->setTmplate( &tmplate ); // Copies
	QCOMPARE( model->tmplate()->brand(), QString( "Test Brand" ) );
	QVERIFY( model->isModified() );

	model->clearModified();
	QVERIFY( !model->isModified() );

	//
	// Set merge
	//
	Merge* merge = Factory::createMerge( TextCsvKeys::id() );
	QCOMPARE( merge->id(), TextCsvKeys::id() );

	model->setMerge( merge );
	QCOMPARE( model->merge(), merge );
	QVERIFY( model->isModified() );

	//
	// Add some variables
	//
	model->clearModified();
	QVERIFY( !model->isModified() );

	Variable i( Variable::Type::INTEGER, "i", "2", Variable::Increment::PER_ITEM, "2" );
	Variable f( Variable::Type::FLOATING_POINT, "f", "6.54", Variable::Increment::PER_COPY, "0.12" );
	model->variables()->addVariable( i );
	QVERIFY( model->isModified() );
	model->variables()->addVariable( f );
	QVERIFY( model->isModified() );

	model->clearModified();
	QVERIFY( !model->isModified() );

	QTemporaryFile csv;
	csv.open();
	csv.write( "id,text\n1,text1\n2,text2\n3,text3\n" );
	csv.close();

	merge->setSource( csv.fileName() );
	QCOMPARE( merge->source(), csv.fileName() );
	QCOMPARE( merge->recordList().size(), 3 );
	QVERIFY( model->isModified() );

	model->clearModified();
	QVERIFY( !model->isModified() );

	//
	// Add some objects
	//
	ColorNode black( Qt::black );
	ModelObject* object1 = new ModelLineObject( 1, 1, 90, 80, 1.0, black );
	model->addObject( object1 );
	QVERIFY( model->isModified() );
	QCOMPARE( model->objectList().size(), 1 );
	QCOMPARE( model->objectList().first(), object1 );

	model->clearModified();
	QVERIFY( !model->isModified() );

	ModelObject* object2 = new ModelTextObject( 2, 2, 70, 30, false, "", "Sans", 10, QFont::Normal, false, false, black, Qt::AlignLeft, Qt::AlignTop, QTextOption::WordWrap, 1, false );
	model->addObject( object2 );
	QVERIFY( model->isModified() );
	QCOMPARE( model->objectList().size(), 2 );
	QCOMPARE( model->objectList().last(), object2 );

	QString modelShortName = model->shortName(); // If no fileName set then model expects to have have this called before being saved/restored (otherwise get differing untitled names)

	//
	// Test
	//
	Model* saved = model->save();
	QVERIFY( saved->isModified() );
	QCOMPARE( saved->merge(), model->merge() ); // Shared
	QCOMPARE( saved->variables(), model->variables() ); // Shared
	QCOMPARE( saved->isModified(), model->isModified() );
	QCOMPARE( saved->shortName(), modelShortName );
	QCOMPARE( saved->shortName(), model->shortName() );
	QCOMPARE( saved->fileName(), model->fileName() );
	QCOMPARE( saved->rotate(), model->rotate() );
	QCOMPARE( saved->objectList().size(), model->objectList().size() );
	QVERIFY( saved->objectList().at(0) != object1 ); // Objects copied
	QVERIFY( saved->objectList().at(1) != object2 ); // Objects copied
	QCOMPARE( saved->objectList().at(0)->x0(), model->objectList().at(0)->x0() );
	QCOMPARE( saved->objectList().at(0)->y0(), model->objectList().at(0)->y0() );
	QCOMPARE( saved->objectList().at(1)->x0(), model->objectList().at(1)->x0() );
	QCOMPARE( saved->objectList().at(1)->y0(), model->objectList().at(1)->y0() );

	// Modify original
	Template tmplate2( "Test Brand2", "part2", "desc2", "testPaperId2", 230, 630 );
	FrameRect* frame2 = new FrameRect( 240, 340, 5, 0, 0, "rect2" );
	tmplate2.addFrame( frame2 );
	model->setTmplate( &tmplate2 );
	QCOMPARE( model->tmplate()->brand(), QString( "Test Brand2" ) );
	QCOMPARE( model->w(), Distance( 240 ) );
	QCOMPARE( model->h(), Distance( 340 ) );

	model->setFileName( "dir/file1.ext" );
	QCOMPARE( model->shortName(), QString( "file1" ) );

	model->setRotate( true );
	QVERIFY( model->rotate() );
	QCOMPARE( model->w(), Distance( 340 ) );
	QCOMPARE( model->h(), Distance( 240 ) );

	model->deleteObject( model->objectList().first() );
	QCOMPARE( model->objectList().size(), 1 );
	QCOMPARE( model->objectList().first(), object2 );

	model->objectList().first()->setY0( model->objectList().first()->y0() + 1 );

	Merge* merge2 = Factory::createMerge( TextCsv::id() );
	QCOMPARE( merge2->id(), TextCsv::id() );
	QTemporaryFile csv2; csv2.open(); csv2.write( "21,text21\n22,text22\n23,text23\n24,text24\n" ); csv2.close();
	merge2->setSource( csv2.fileName() );
	QCOMPARE( merge2->source(), csv2.fileName() );
	QCOMPARE( merge2->recordList().size(), 4 );

	model->setMerge( merge2 ); // Deletes original so saved->merge() now invalid
	QCOMPARE( model->merge(), merge2 );

	Model* modified = model->save();
	QCOMPARE( modified->merge(), merge2 ); // Shared

	Variable c( Variable::Type::COLOR, "c", "blue", Variable::Increment::PER_PAGE );
	model->variables()->addVariable( c );
	QCOMPARE( model->variables(), saved->variables() ); // Shared.

	// Verify differences
	QVERIFY( model->shortName() != modelShortName );
	QVERIFY( model->shortName() != saved->shortName() );
	QVERIFY( model->fileName() != saved->fileName() );
	QVERIFY( model->tmplate()->brand() != saved->tmplate()->brand() );
	QVERIFY( model->rotate() != saved->rotate() );
	QVERIFY( model->w() != saved->w() );
	QVERIFY( model->h() != saved->h() );
	QVERIFY( model->objectList().size() != saved->objectList().size() );
	QVERIFY( model->objectList().at(0)->x0() != saved->objectList().at(0)->x0() );
	QVERIFY( model->objectList().at(0)->y0() != saved->objectList().at(0)->y0() );
	QCOMPARE( model->objectList().at(0)->x0(), saved->objectList().at(1)->x0() ); // Unchanged
	QVERIFY( model->objectList().at(0)->y0() != saved->objectList().at(1)->y0() );

	// Restore
	model->restore( saved );
	QCOMPARE( model->shortName(), modelShortName );
	QCOMPARE( model->shortName(), saved->shortName() );
	QCOMPARE( model->fileName(), saved->fileName() );
	QCOMPARE( model->tmplate()->brand(), saved->tmplate()->brand() );
	QCOMPARE( model->rotate(), saved->rotate() );
	QCOMPARE( model->w(), saved->w() );
	QCOMPARE( model->h(), saved->h() );
	QCOMPARE( model->objectList().size(), saved->objectList().size() );
	QCOMPARE( model->objectList().size(), 2 );
	QCOMPARE( model->objectList().at(0)->x0(), saved->objectList().at(0)->x0() );
	QCOMPARE( model->objectList().at(0)->y0(), saved->objectList().at(0)->y0() );
	QCOMPARE( model->objectList().at(1)->x0(), saved->objectList().at(1)->x0() );
	QCOMPARE( model->objectList().at(1)->y0(), saved->objectList().at(1)->y0() );

	QCOMPARE( model->merge(), merge2 ); // Unchanged
	QVERIFY( model->merge() != saved->merge() ); // NOTE saved->merge() now points to deleted object
	QCOMPARE( model->variables(), saved->variables() ); // Unchanged

	// Unrestore
	model->restore( modified );
	QVERIFY( model->shortName() != modelShortName );
	QVERIFY( model->shortName() != saved->shortName() );
	QVERIFY( model->fileName() != saved->fileName() );
	QVERIFY( model->tmplate()->brand() != saved->tmplate()->brand() );
	QVERIFY( model->rotate() != saved->rotate() );
	QVERIFY( model->w() != saved->w() );
	QVERIFY( model->h() != saved->h() );
	QCOMPARE( model->objectList().size(), 1 );
	QVERIFY( model->objectList().size() != saved->objectList().size() );
	QVERIFY( model->objectList().at(0)->x0() != saved->objectList().at(0)->x0() );
	QVERIFY( model->objectList().at(0)->y0() != saved->objectList().at(0)->y0() );
	QCOMPARE( model->merge(), merge2 ); // Same
	QCOMPARE( model->variables(), saved->variables() ); // Same

	QCOMPARE( model->shortName(), modified->shortName() );
	QCOMPARE( model->fileName(), modified->fileName() );
	QCOMPARE( model->tmplate()->brand(), modified->tmplate()->brand() );
	QCOMPARE( model->rotate(), modified->rotate() );
	QCOMPARE( model->w(), modified->w() );
	QCOMPARE( model->h(), modified->h() );
	QCOMPARE( model->objectList().size(), modified->objectList().size() );
	QCOMPARE( model->objectList().at(0)->x0(), modified->objectList().at(0)->x0() );
	QCOMPARE( model->objectList().at(0)->y0(), modified->objectList().at(0)->y0() );

	delete model->merge(); // Final instance owned by us
	delete model->variables(); // Instance owned by us
	delete model;
	delete saved;
	delete modified;
}
