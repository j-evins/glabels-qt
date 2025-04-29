/*  Model.cpp
 *
 *  Copyright (C) 2013-2016  Jaye Evins <evins@snaught.com>
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

#include "Model.h"

#include "ModelObject.h"
#include "ModelTextObject.h"
#include "ModelImageObject.h"
#include "Region.h"
#include "Size.h"
#include "XmlLabelCreator.h"
#include "XmlLabelParser.h"

#include "merge/None.h"

#include <QApplication>
#include <QClipboard>
#include <QFileInfo>
#include <QMimeData>
#include <QtDebug>


namespace glabels
{
	namespace model
	{

		//
		// Private
		//
		namespace
		{
			const QString MIME_TYPE = "application/x-glabels-objects";
		}


		///
		/// Default constructor.
		///
		Model::Model()
		{
			mVariables = new Variables();
			mMerge = new merge::None();

			connect( mVariables, SIGNAL(changed()), this, SLOT(onVariablesChanged()) );
		}


		Model::Model( merge::Merge* merge, Variables* variables )
		{
			mVariables = variables; // Shared
			mMerge = merge; // Shared
		}


		///
		/// Destructor.
		///
		Model::~Model()
		{
			qDeleteAll( mObjectList );
			// Final instance of mMerge and mVariables to be deleted by Model owner
		}


		///
		/// Save model state
		///
		Model* Model::save() const
		{
			auto* savedModel = new Model( mMerge, mVariables ); // mMerge and mVariables shared between models

			if ( mFileName.isEmpty() && mUntitledInstance == 0 )
			{
				qDebug() << "Model::save: Warning: called before mUntitledInstance has been initialized: untitled names will differ";
			}

			savedModel->restore( this );

			return savedModel;
		}


		///
		/// Restore model state
		///
		void Model::restore( const Model *savedModel )
		{
			// Clear current object list
			foreach ( ModelObject* object, mObjectList )
			{
				delete object;
			}
			mObjectList.clear();

			// Now copy state
			mUntitledInstance = savedModel->mUntitledInstance;
			mModified         = savedModel->mModified;
			mFileName         = savedModel->mFileName;
			mTmplate          = savedModel->mTmplate;
			mRotate           = savedModel->mRotate;

			foreach ( ModelObject* savedObject, savedModel->mObjectList )
			{
				ModelObject* object = savedObject->clone();
		
				object->setParent( this );
				mObjectList << object;

				connect( object, SIGNAL(changed()), this, SLOT(onObjectChanged()) );
				connect( object, SIGNAL(moved()), this, SLOT(onObjectMoved()) );
			}

			// Emit signals based on potential changes
			emit changed();
			emit selectionChanged();
			emit modifiedChanged();
			emit nameChanged();
			emit sizeChanged();
		}


		///
		/// Is model modified?
		///
		bool Model::isModified() const
		{
			return mModified;
		}


		///
		/// Get filename
		///
		const QString& Model::fileName() const
		{
			return mFileName;
		}


		///
		/// Set filename
		///
		void Model::setFileName( const QString &fileName )
		{
			if ( mFileName != fileName )
			{
				mFileName = fileName;
				emit nameChanged();
			}
		}


		///
		/// Get template
		///
		const Template* Model::tmplate() const
		{
			return &mTmplate;
		}


		///
		/// Get frame
		///
		const Frame* Model::frame() const
		{
			return mTmplate.frames().constFirst();
		}


		///
		/// Set template
		///
		void Model::setTmplate( const Template* tmplate )
		{
			mTmplate = *tmplate;

			setModified();
		
			emit changed();
			emit sizeChanged();

			Settings::addToRecentTemplateList( tmplate->name() );
		}


		///
		/// Get rotation
		///
		bool Model::rotate() const
		{
			return mRotate;
		}


		///
		/// Set rotation
		///
		void Model::setRotate( bool rotate )
		{
			if (mRotate != rotate)
			{
				mRotate = rotate;

				setModified();

				emit changed();
				emit sizeChanged();
			}
		}


		///
		/// Get width
		///
		Distance Model::w() const
		{
			auto& frames = mTmplate.frames();
			if ( !frames.isEmpty() )
			{
				auto* frame = mTmplate.frames().constFirst();
				return mRotate ? frame->h() : frame->w();
			}
			else
			{
				return Distance::pt(0);
			}
		}


		///
		/// Get height
		///
		Distance Model::h() const
		{
			auto& frames = mTmplate.frames();
			if ( !frames.isEmpty() )
			{
				auto* frame = mTmplate.frames().constFirst();
				return mRotate ? frame->w() : frame->h();
			}
			else
			{
				return Distance::pt(0);
			}
		}


		///
		/// Set height (if variable length)
		///
		void Model::setH( const Distance& h )
		{
			if ( auto* frame = mTmplate.frames().first() )
			{
				frame->setH( h );

				setModified();

				emit changed();
				emit sizeChanged();
			}
		}


		///
		/// Get object list
		///
		const QList<ModelObject*>& Model::objectList() const
		{
			return mObjectList;
		}


		///
		/// Get directory as a QDir.
		///
		QDir Model::dir() const
		{
			if ( mFileName.isEmpty() )
			{
				return QDir::current();
			}
			else
			{
				return QFileInfo( mFileName ).absoluteDir();
			}
		}


		///
		/// Get directory as a path.
		///
		QString Model::dirPath() const
		{
			if ( mFileName.isEmpty() )
			{
				return QDir::currentPath();
			}
			else
			{
				return QFileInfo( mFileName ).absolutePath();
			}
		}


		///
		/// Get short name.
		///
		QString Model::shortName()
		{
			static int untitledCount = 0;
		
			if ( mFileName.isEmpty() )
			{
				if ( mUntitledInstance == 0 )
				{
					mUntitledInstance = ++untitledCount;
				}
				QString numString;
				numString.setNum(mUntitledInstance);;

				return tr("Untitled") + numString;
			}
			else
			{
				QFileInfo fileInfo( mFileName );
				return fileInfo.baseName();
			}
		}


		///
		/// Get variables object
		///
		Variables* Model::variables() const
		{
			return mVariables;
		}


		///
		/// Get merge object
		///
		merge::Merge* Model::merge() const
		{
			return mMerge;
		}


		///
		/// Set merge object
		///
		void Model::setMerge( merge::Merge* merge )
		{
			if ( merge != mMerge )
			{
				delete mMerge;
				mMerge = merge;

				connect( mMerge, SIGNAL(sourceChanged()), this, SLOT(onMergeSourceChanged()) );
				connect( mMerge, SIGNAL(selectionChanged()), this, SLOT(onMergeSelectionChanged()) );

				setModified();
		
				emit changed();
				emit mergeChanged();
				emit mergeSourceChanged();
			}
		}


		///
		/// Set modified status
		///
		void Model::setModified()
		{
			mModified = true;
			emit modifiedChanged();
		}


		///
		/// Clear modified status
		///
		void Model::clearModified()
		{
			mModified = false;
			emit modifiedChanged();
		}


		///
		/// Add object.
		///
		void Model::addObject( ModelObject* object )
		{
			object->setParent( this );
			mObjectList << object;

			connect( object, SIGNAL(changed()), this, SLOT(onObjectChanged()) );
			connect( object, SIGNAL(moved()), this, SLOT(onObjectMoved()) );

			setModified();

			emit changed();
		}


		///
		/// Delete Object
		///
		void Model::deleteObject( ModelObject* object )
		{
			object->unselect();
			mObjectList.removeOne( object );

			disconnect( object, nullptr, this, nullptr );

			setModified();

			emit changed();

			delete object;
		}


		///
		/// Object at x,y
		///
		ModelObject* Model::objectAt( double          scale,
		                              const Distance& x,
		                              const Distance& y ) const
		{
			/* Search object list in reverse order.  I.e. from top to bottom. */
			QList<ModelObject*>::const_iterator it = mObjectList.end();
			while ( it != mObjectList.begin() )
			{
				it--;
				ModelObject* object = *it;
				if ( object->isLocatedAt( scale, x, y ) )
				{
					return object;
				}
			}

			return nullptr;
		}


		///
		/// Handle at x,y
		///
		Handle* Model::handleAt( double          scale,
		                         const Distance& x,
		                         const Distance& y ) const
		{
			foreach( ModelObject* object, mObjectList )
			{
				Handle* handle = object->handleAt( scale, x, y );
				if ( handle )
				{
					return handle;
				}
			}

			return nullptr;
		}


		///
		/// Object Changed Slot
		///
		void Model::onObjectChanged()
		{
			setModified();
			emit changed();
		}


		///
		/// Object Moved Slot
		///
		void Model::onObjectMoved()
		{
			setModified();
			emit changed();
		}


		///
		/// Variables Changed Slot
		///
		void Model::onVariablesChanged()
		{
			setModified();
			emit changed();
			emit variablesChanged();
		}


		///
		/// Merge Source Changed Slot
		///
		void Model::onMergeSourceChanged()
		{
			setModified();
			emit changed();
			emit mergeSourceChanged();
		}


		///
		/// Merge Selection Changed Slot
		///
		void Model::onMergeSelectionChanged()
		{
			emit changed();
			emit mergeSelectionChanged();
		}


		///
		/// Select Object
		///
		void Model::selectObject( ModelObject* object )
		{
			object->select();

			emit selectionChanged();
		}


		///
		/// Unselect Object
		///
		void Model::unselectObject( ModelObject* object )
		{
			object->unselect();

			emit selectionChanged();
		}


		///
		/// Select All Objects
		///
		void Model::selectAll()
		{
			foreach ( ModelObject* object, mObjectList )
			{
				object->select();
			}

			emit selectionChanged();
		}


		///
		/// Unselect All Objects
		///
		void Model::unselectAll()
		{
			foreach ( ModelObject* object, mObjectList )
			{
				object->unselect();
			}

			emit selectionChanged();
		}


		///
		/// Select Region
		///
		void Model::selectRegion( const Region &region )
		{
			Distance rX1 = min( region.x1(), region.x2() );
			Distance rY1 = min( region.y1(), region.y2() );
			Distance rX2 = max( region.x1(), region.x2() );
			Distance rY2 = max( region.y1(), region.y2() );

			foreach ( ModelObject* object, mObjectList )
			{
				Region objectExtent = object->getExtent();

				if ( (objectExtent.x1() >= rX1) &&
				     (objectExtent.x2() <= rX2) &&
				     (objectExtent.y1() >= rY1) &&
				     (objectExtent.y2() <= rY2) )
				{
					object->select();
				}
			}

			emit selectionChanged();
		}


		///
		/// Is Selection Empty?
		///
		bool Model::isSelectionEmpty()
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					return false;
				}
			}

			return true;
		}


		///
		/// Is Selection Atomic?
		///
		bool Model::isSelectionAtomic()
		{
			int nSelected = 0;

			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					nSelected++;
					if ( nSelected > 1 )
					{
						return false;
					}
				}
			}

			return nSelected == 1;
		}


		///
		/// Get List of Selected Objects
		///
		QList<ModelObject*> Model::getSelection()
		{
			QList<ModelObject*> selectedList;

			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					selectedList << object;
				}
			}

			return selectedList;
		}


		///
		/// Get First Object in Selection List
		///
		ModelObject* Model::getFirstSelectedObject()
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					return object;
				}
			}

			return nullptr;
		}


		///
		/// Can Any Objects in Selection Accept Text Properties?
		///
		bool Model::canSelectionText()
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() && object->canText() )
				{
					return true;
				}
			}

			return false;
		}


		///
		/// Can Any Objects in Selection Accept Fill Property?
		///
		bool Model::canSelectionFill()
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() && object->canFill() )
				{
					return true;
				}
			}

			return false;
		}


		///
		/// Can Any Objects in Selection Accept Line Color Property?
		///
		bool Model::canSelectionLineColor()
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() && object->canLineColor() )
				{
					return true;
				}
			}

			return false;
		}


		///
		/// Can Any Objects in Selection Accept Line Width Property?
		///
		bool Model::canSelectionLineWidth()
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() && object->canLineWidth() )
				{
					return true;
				}
			}

			return false;
		}


		///
		/// Delete Selected Objects
		///
		void Model::deleteSelection()
		{
			QList<ModelObject*> selectedList = getSelection();

			foreach ( ModelObject* object, selectedList )
			{
				deleteObject( object );
			}

			setModified();

			emit changed();
			emit selectionChanged();
		}


		///
		/// Raise Selected Objects To Top
		///
		void Model::raiseSelectionToTop()
		{
			QList<ModelObject*> selectedList = getSelection();

			foreach ( ModelObject* object, selectedList )
			{
				mObjectList.removeOne( object );
			}

			// Move to end of list, representing top most object.
			foreach ( ModelObject* object, selectedList )
			{
				mObjectList.push_back( object );
			}

			setModified();

			emit changed();
		}


		///
		/// Lower Selected Objects To Bottom
		///
		void Model::lowerSelectionToBottom()
		{
			QList<ModelObject*> selectedList = getSelection();

			foreach ( ModelObject* object, selectedList )
			{
				mObjectList.removeOne( object );
			}

			// Move to front of list, representing bottom most object.
			foreach ( ModelObject* object, selectedList )
			{
				mObjectList.push_front( object );
			}

			setModified();

			emit changed();
		}


		///
		/// Rotate Selected Objects
		///
		void Model::rotateSelection( double thetaDegs )
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					object->rotate( thetaDegs );
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Rotate Selected Objects Left 90 degrees
		///
		void Model::rotateSelectionLeft()
		{
			rotateSelection( -90.0 );
		}


		///
		/// Rotate Selected Objects Right 90 degrees
		///
		void Model::rotateSelectionRight()
		{
			rotateSelection( 90.0 );
		}


		///
		/// Flip Selected Objects Horizontally
		///
		void Model::flipSelectionHoriz()
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					object->flipHoriz();
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Flip Selected Objects Vertically
		///
		void Model::flipSelectionVert()
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					object->flipVert();
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Align Selected Objects To Their Left Edges
		///
		void Model::alignSelectionLeft()
		{
			if ( isSelectionEmpty() || isSelectionAtomic() )
			{
				return;
			}

			QList<ModelObject*> selectedList = getSelection();

			// Find left-most edge.
			Distance x1_min = 7200; // Start with a very large value: 7200pts = 100in
			foreach ( ModelObject* object, selectedList )
			{
				Region r = object->getExtent();
				if ( r.x1() < x1_min ) x1_min = r.x1();
			}

			// Now adjust the object positions to line up the left edges at left-most edge.
			foreach ( ModelObject* object, selectedList )
			{
				Region r = object->getExtent();
				Distance dx = x1_min - r.x1();
				object->setPositionRelative( dx, 0 );
			}
		
			setModified();

			emit changed();
		}


		///
		/// Align Selected Objects To Their Right Edges
		///
		void Model::alignSelectionRight()
		{
			if ( isSelectionEmpty() || isSelectionAtomic() )
			{
				return;
			}

			QList<ModelObject*> selectedList = getSelection();

			// Find right-most edge.
			Distance x1_max = -7200; // Start with a very large negative value: 7200pts = 100in
			foreach ( ModelObject* object, selectedList )
			{
				Region r = object->getExtent();
				if ( r.x1() > x1_max ) x1_max = r.x1();
			}

			// Now adjust the object positions to line up the right edges at right-most edge.
			foreach ( ModelObject* object, selectedList )
			{
				Region r = object->getExtent();
				Distance dx = x1_max - r.x1();
				object->setPositionRelative( dx, 0 );
			}
		
			setModified();

			emit changed();
		}


		///
		/// Align Selected Objects To Their Horizontal Centers
		///
		void Model::alignSelectionHCenter()
		{
			if ( isSelectionEmpty() || isSelectionAtomic() )
			{
				return;
			}

			QList<ModelObject*> selectedList = getSelection();

			// Find average center of objects.
			Distance xsum = 0;
			int n = 0;
			foreach ( ModelObject* object, selectedList )
			{
				Region r = object->getExtent();
				xsum += (r.x1() + r.x2()) / 2.0;
				n++;
			}
			Distance xavg = xsum / n;

			// Find object closest to average center of objects.
			Distance xcenter = 7200; // Start with very large value.
			Distance dxmin = fabs( xavg - xcenter );
			foreach ( ModelObject* object, selectedList )
			{
				Region r = object->getExtent();
				Distance dx = fabs( xavg - (r.x1() + r.x2())/2.0 );
				if ( dx < dxmin )
				{
					dxmin = dx;
					xcenter = (r.x1() + r.x2()) / 2.0;
				}
			}

			// Now adjust the object positions to line up with the center of this object.
			foreach ( ModelObject* object, selectedList )
			{
				Region r = object->getExtent();
				Distance dx = xcenter - (r.x1() + r.x2())/2.0;
				object->setPositionRelative( dx, 0 );
			}
		
			setModified();

			emit changed();
		}


		///
		/// Align Selected Objects To Their Top Edges
		///
		void Model::alignSelectionTop()
		{
			if ( isSelectionEmpty() || isSelectionAtomic() )
			{
				return;
			}

			QList<ModelObject*> selectedList = getSelection();

			// Find top-most edge.
			Distance y1_min = 7200; // Start with a very large value: 7200pts = 100in
			foreach ( ModelObject* object, selectedList )
			{
				Region r = object->getExtent();
				if ( r.y1() < y1_min ) y1_min = r.y1();
			}

			// Now adjust the object positions to line up the top edges at top-most edge.
			foreach ( ModelObject* object, selectedList )
			{
				Region r = object->getExtent();
				Distance dy = y1_min - r.y1();
				object->setPositionRelative( 0, dy );
			}
		
			setModified();

			emit changed();
		}


		///
		/// Align Selected Objects To Their Bottom Edges
		///
		void Model::alignSelectionBottom()
		{
			if ( isSelectionEmpty() || isSelectionAtomic() )
			{
				return;
			}

			QList<ModelObject*> selectedList = getSelection();

			// Find bottom-most edge.
			Distance y1_max = -7200; // Start with a very large negative value: 7200pts = 100in
			foreach ( ModelObject* object, selectedList )
			{
				Region r = object->getExtent();
				if ( r.y1() > y1_max ) y1_max = r.y1();
			}

			// Now adjust the object positions to line up the bottom edges at bottom-most edge.
			foreach ( ModelObject* object, selectedList )
			{
				Region r = object->getExtent();
				Distance dy = y1_max - r.y1();
				object->setPositionRelative( 0, dy );
			}
		
			setModified();

			emit changed();
		}


		///
		/// Align Selected Objects To Their Vertical Centers Edges
		///
		void Model::alignSelectionVCenter()
		{
			if ( isSelectionEmpty() || isSelectionAtomic() )
			{
				return;
			}

			QList<ModelObject*> selectedList = getSelection();

			// Find average center of objects.
			Distance ysum = 0;
			int n = 0;
			foreach ( ModelObject* object, selectedList )
			{
				Region r = object->getExtent();
				ysum += (r.y1() + r.y2()) / 2.0;
				n++;
			}
			Distance yavg = ysum / n;

			// Find object closest to average center of objects.
			Distance ycenter = 7200; // Start with very large value.
			Distance dymin = fabs( yavg - ycenter );
			foreach ( ModelObject* object, selectedList )
			{
				Region r = object->getExtent();
				Distance dy = fabs( yavg - (r.y1() + r.y2())/2.0 );
				if ( dy < dymin )
				{
					dymin = dy;
					ycenter = (r.y1() + r.y2()) / 2.0;
				}
			}

			// Now adjust the object positions to line up with the center of this object.
			foreach ( ModelObject* object, selectedList )
			{
				Region r = object->getExtent();
				Distance dy = ycenter - (r.y1() + r.y2())/2.0;
				object->setPositionRelative( 0, dy );
			}
		
			setModified();

			emit changed();
		}


		///
		/// Align Selected Objects To Center Of Label Horizontally
		///
		void Model::centerSelectionHoriz()
		{
			Distance xLabelCenter = w() / 2.0;

			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					Region r = object->getExtent();
					Distance xObjectCenter = (r.x1() + r.x2()) / 2.0;
					Distance dx = xLabelCenter - xObjectCenter;
					object->setPositionRelative( dx, 0 );
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Align Selected Objects To Center Of Label Vertically
		///
		void Model::centerSelectionVert()
		{
			Distance yLabelCenter = h() / 2.0;

			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					Region r = object->getExtent();
					Distance yObjectCenter = (r.y1() + r.y2()) / 2.0;
					Distance dy = yLabelCenter - yObjectCenter;
					object->setPositionRelative( 0, dy );
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Move Selected Objects By dx,dy
		///
		void Model::moveSelection( const Distance& dx, const Distance& dy )
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					object->setPositionRelative( dx, dy );
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Set Font Family Of Selected Objects
		///
		void Model::setSelectionFontFamily( const QString &fontFamily )
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					object->setFontFamily( fontFamily );
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Set Font Size Of Selected Objects
		///
		void Model::setSelectionFontSize( double fontSize )
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					object->setFontSize( fontSize );
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Set Font Weight Of Selected Objects
		///
		void Model::setSelectionFontWeight( QFont::Weight fontWeight )
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					object->setFontWeight( fontWeight );
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Set Font Italic Flag Of Selected Objects
		///
		void Model::setSelectionFontItalicFlag( bool fontItalicFlag )
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					object->setFontItalicFlag( fontItalicFlag );
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Set Text Horizontal Alignment Of Selected Objects
		///
		void Model::setSelectionTextHAlign( Qt::Alignment textHAlign )
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					object->setTextHAlign( textHAlign );
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Set Text Vertical Alignment Of Selected Objects
		///
		void Model::setSelectionTextVAlign( Qt::Alignment textVAlign )
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					object->setTextVAlign( textVAlign );
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Set Text Line Spacing Of Selected Objects
		///
		void Model::setSelectionTextLineSpacing( double textLineSpacing )
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					object->setTextLineSpacing( textLineSpacing );
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Set Text Color Node Of Selected Objects
		///
		void Model::setSelectionTextColorNode( ColorNode textColorNode )
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					object->setTextColorNode( textColorNode );
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Set Line Width Of Selected Objects
		///
		void Model::setSelectionLineWidth( const Distance& lineWidth )
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					object->setLineWidth( lineWidth );
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Set Line Color Node Of Selected Objects
		///
		void Model::setSelectionLineColorNode( ColorNode lineColorNode )
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					object->setLineColorNode( lineColorNode );
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Set Fill Color Node Of Selected Objects
		///
		void Model::setSelectionFillColorNode( ColorNode fillColorNode )
		{
			foreach ( ModelObject* object, mObjectList )
			{
				if ( object->isSelected() )
				{
					object->setFillColorNode( fillColorNode );
				}
			}

			setModified();

			emit changed();
		}


		///
		/// Copy selection to clipboard
		///
		void Model::copySelection()
		{
			if ( !isSelectionEmpty() )
			{
				QClipboard *clipboard = QApplication::clipboard();
		
				QByteArray buffer;
				XmlLabelCreator::serializeObjects( getSelection(), this, buffer );

				auto *mimeData = new QMimeData;
				mimeData->setData( MIME_TYPE, buffer );

				clipboard->setMimeData( mimeData );
			}
		}


		///
		/// Cut selection to clipboard
		///
		void Model::cutSelection()
		{
			copySelection();
			deleteSelection();
		}


		///
		/// Can we paste?
		///
		bool Model::canPaste()
		{
			const QClipboard *clipboard = QApplication::clipboard();
			const QMimeData *mimeData = clipboard->mimeData();

			if ( mimeData->hasFormat( MIME_TYPE ) )
			{
				return true;
			}
			else if ( mimeData->hasImage() )
			{
				return true;
			}
			else if ( mimeData->hasText() )
			{
				return true;
			}
			return false;
		}


		///
		/// Paste from clipboard
		///
		void Model::paste()
		{
			const QClipboard *clipboard = QApplication::clipboard();
			const QMimeData *mimeData = clipboard->mimeData();

			if ( mimeData->hasFormat( MIME_TYPE ) )
			{
				// Native objects
				QByteArray buffer = mimeData->data( MIME_TYPE );
				QList <ModelObject*> objects = XmlLabelParser::deserializeObjects( buffer, this );

				unselectAll();
				foreach ( ModelObject* object, objects )
				{
					addObject( object );
					selectObject( object );
				}
			}
			else if ( mimeData->hasImage() )
			{
				// Create object from clipboard image
				auto* object = new ModelImageObject();
				object->setImage( qvariant_cast<QImage>(mimeData->imageData()) );
				object->setSize( object->naturalSize() );
				object->setPosition( (w()-object->w())/2.0, (h()-object->h())/2.0 );
				addObject( object );
				unselectAll();
				selectObject( object );
			}
			else if ( mimeData->hasText() )
			{
				// Create object from clipboard text
				auto* object = new ModelTextObject();
				object->setText( mimeData->text() );
				object->setSize( object->naturalSize() );
				object->setPosition( (w()-object->w())/2.0, (h()-object->h())/2.0 );
				addObject( object );
				unselectAll();
				selectObject( object );
			}
		}


		///
		/// Draw label objects
		///
		void Model::draw( QPainter* painter, bool inEditor, merge::Record* record, Variables* variables ) const
		{
			foreach ( ModelObject* object, mObjectList )
			{
				object->draw( painter, inEditor, record, variables );
			}
		}

	}
}
