/*  MergeView.cpp
 *
 *  Copyright (C) 2016  Jaye Evins <evins@snaught.com>
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

#include "MergeView.h"

#include "merge/Factory.h"

#include "model/FileUtil.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QtDebug>


namespace glabels
{
	const QChar NEWLINE_CHAR = QChar(0x23CE);

	///
	/// Constructor
	///
	MergeView::MergeView( QWidget *parent )
		: QWidget(parent), mModel(nullptr), mUndoRedoModel(nullptr), mBlock(false), mOldFormatComboIndex(0)
	{
		setupUi( this );

		titleLabel->setText( QString( "<span style='font-size:18pt;'>%1</span>" ).arg( tr("Merge") ) );

		mMergeFormatNames = merge::Factory::nameList();
		formatCombo->addItems( mMergeFormatNames );
	}


	///
	/// Destructor
	///
	MergeView::~MergeView()
	{
		// empty
	}


	///
	/// Set Model
	///
	void MergeView::setModel( model::Model* model, UndoRedoModel* undoRedoModel )
	{
		mModel = model;
		mUndoRedoModel = undoRedoModel;

		// Initialize CWD
		mCwd = mModel->dirPath();

		onMergeChanged();
		connect( mModel, SIGNAL(mergeChanged()), this, SLOT(onMergeChanged()) );
	}


	///
	/// Merge changed handler
	///
	void MergeView::onMergeChanged()
	{
		QString name = merge::Factory::idToName( mModel->merge()->id() );
		int index = mMergeFormatNames.indexOf( name );
		mOldFormatComboIndex = index;
		formatCombo->setCurrentIndex( index );

		QString fn;
		
		switch ( merge::Factory::idToType( mModel->merge()->id() ) )
		{
		case merge::Factory::NONE:
		case merge::Factory::FIXED:
			locationLabel->setEnabled( false );
			locationLineEdit->setText( "" );
			locationBrowseButton->setVisible( false );
			break;

		case merge::Factory::FILE:
			locationLabel->setEnabled( true );
			fn = model::FileUtil::makeRelativeIfInDir( mModel->dir(), mModel->merge()->source() );
			locationLineEdit->setText( fn );
			locationBrowseButton->setVisible( true );
			break;

		default:
			qWarning( "MergeView::onMergeChanged()::Should not be reached!" );
			break;
		}

		recordsTable->clear();
		recordsTable->setColumnCount( 0 );
		loadHeaders( mModel->merge() );
		loadTable( mModel->merge() );

		connect( mModel->merge(), SIGNAL(sourceChanged()),
		         this, SLOT(onMergeSourceChanged()) );
		
		connect( mModel->merge(), SIGNAL(selectionChanged()),
		         this, SLOT(onMergeSelectionChanged()) );

		connect( recordsTable, SIGNAL(cellChanged(int,int)),
		         this, SLOT(onCellChanged(int,int)) );
	}


	///
	/// Merge source changed handler
	///
	void MergeView::onMergeSourceChanged()
	{
		QString fn = model::FileUtil::makeRelativeIfInDir( mModel->dir(), mModel->merge()->source() );
		locationLineEdit->setText( fn );

		recordsTable->clear();
		recordsTable->setColumnCount( 0 );
		loadHeaders( mModel->merge() );
		loadTable( mModel->merge() );
	}


	///
	/// Merge selection changed handler
	///
	void MergeView::onMergeSelectionChanged()
	{
		mBlock = true;  // Don't recurse
	
		const QList<merge::Record*>& records = mModel->merge()->recordList();

		int iRow = 0;
		foreach ( merge::Record* record, records )
		{
			QTableWidgetItem* item = recordsTable->item( iRow, 0 );
			item->setCheckState( record->isSelected() ? Qt::Checked : Qt::Unchecked );

			iRow++;
		}

		mBlock = false;
	}


	///
	/// Format combo changed handler
	///
	void MergeView::onFormatComboActivated()
	{
		int index = formatCombo->currentIndex();
		if ( index != mOldFormatComboIndex )
		{
			mOldFormatComboIndex = index;

			QString id = merge::Factory::indexToId(index);
			mModel->setMerge( merge::Factory::createMerge( id ) );
		}
	}


	///
	/// Location button clicked handler
	///
	void MergeView::onLocationBrowseButtonClicked()
	{
		QString fileName =
			QFileDialog::getOpenFileName( this,
			                              tr("Select merge file"),
			                              mCwd,
			                              tr("All files (*)") );
		if ( !fileName.isEmpty() )
		{
			mModel->merge()->setSource( fileName );
			mCwd = QFileInfo( fileName ).absolutePath(); // Update CWD
		}
	}


	///
	/// Select all button clicked handler
	///
	void MergeView::onSelectAllButtonClicked()
	{
		mModel->merge()->selectAll();
	}


	///
	/// Unselect all button clicked handler
	///
	void MergeView::onUnselectAllButtonClicked()
	{
		mModel->merge()->unselectAll();
	}


	///
	/// Cell changed handler
	///
	void MergeView::onCellChanged( int iRow, int iCol )
	{
		if ( !mBlock )
		{
			QTableWidgetItem* item = recordsTable->item( iRow, 0 );
			bool state = (item->checkState() == Qt::Unchecked) ? false : true;
		
			mModel->merge()->setSelected( iRow, state );
		}
	}


	///
	/// Load headers
	///
	void MergeView::loadHeaders( merge::Merge* merge )
	{
		mPrimaryKey = merge->primaryKey();
		mKeys = merge->keys();

		if ( mKeys.size() > 0 )
		{
			recordsTable->setColumnCount( mKeys.size() + 1 );  // Include extra column

			// First column = primary Key
			auto* item = new QTableWidgetItem( mPrimaryKey );
			item->setFlags( Qt::ItemIsEnabled );
			recordsTable->setHorizontalHeaderItem( 0, item );

			// Starting on second column, one column per key, skip primary Key
			int iCol = 1;
			foreach ( QString key, mKeys )
			{
				if ( key != mPrimaryKey )
				{
					auto* item = new QTableWidgetItem( key );
					item->setFlags( Qt::ItemIsEnabled );
					recordsTable->setHorizontalHeaderItem( iCol, item );

					iCol++;
				}

			}

			// Extra dummy column to fill any extra horizontal space
			auto* fillItem = new QTableWidgetItem();
			fillItem->setFlags( Qt::NoItemFlags );
			recordsTable->setHorizontalHeaderItem( iCol, fillItem );
			recordsTable->horizontalHeader()->setStretchLastSection( true );
		}
	}


	///
	/// Load table
	///
	void MergeView::loadTable( merge::Merge* merge )
	{
		mBlock = true;

		const QList<merge::Record*>& records = merge->recordList();
		recordsTable->setRowCount( records.size() );

		int iRow = 0;
		foreach ( merge::Record* record, records )
		{
			// First column for primary field
			auto* item = new QTableWidgetItem();
			if ( record->contains( mPrimaryKey ) )
			{
				auto text = printableTextForView( (*record)[mPrimaryKey] );
				item->setText( text );
			}
			item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsUserCheckable );
			item->setCheckState( record->isSelected() ? Qt::Checked : Qt::Unchecked );
			recordsTable->setItem( iRow, 0, item );
			recordsTable->resizeColumnToContents( 0 );

			// Starting on 2nd column, 1 column per field, skip primary field
			int iCol = 1;
			foreach ( QString key, mKeys )
			{
				if ( key != mPrimaryKey )
				{
					if ( record->contains( key ) )
					{
						auto text = printableTextForView( (*record)[key] );
						auto* item = new QTableWidgetItem( text );
						item->setFlags( Qt::ItemIsEnabled );
						recordsTable->setItem( iRow, iCol, item );
						recordsTable->resizeColumnToContents( iCol );
					}

					iCol++;
				}
			}

			// Extra dummy column to fill any extra horizontal space
			auto* fillItem = new QTableWidgetItem();
			fillItem->setFlags( Qt::NoItemFlags );
			recordsTable->setItem( iRow, iCol, fillItem );

			iRow++;
		}

		mBlock = false;
	}


	///
	/// modify text to be printable e.g. replace newlines
	///
	QString MergeView::printableTextForView( QString text )
	{
		// Replace windows style newlines
		text.replace("\r\n", NEWLINE_CHAR);

		// Replace unix style newlines
		text.replace("\n", NEWLINE_CHAR);

		return text;
	}
} // namespace glabels
