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

#include "MergeTableModel.h"

#include "merge/Factory.h"

#include "model/FileUtil.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>


namespace glabels
{
	const QChar NEWLINE_CHAR = QChar(0x23CE);

	///
	/// Constructor
	///
	MergeView::MergeView( QWidget *parent )
		: QWidget(parent), mModel(nullptr), mUndoRedoModel(nullptr), mOldFormatComboIndex(0)
	{
		setupUi( this );

		titleLabel->setText( QString( "<span style='font-size:18pt;'>%1</span>" ).arg( tr("Merge") ) );

		mMergeFormatNames = merge::Factory::nameList();
		formatCombo->addItems( mMergeFormatNames );
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

		recordsTableView->setModel( new MergeTableModel( mModel->merge() ) );
		recordsTableView->resizeColumnsToContents();

		connect( mModel->merge(), SIGNAL(sourceChanged()),
		         this, SLOT(onMergeSourceChanged()) );
	}


	///
	/// Merge source changed handler
	///
	void MergeView::onMergeSourceChanged()
	{
		QString fn = model::FileUtil::makeRelativeIfInDir( mModel->dir(), mModel->merge()->source() );
		locationLineEdit->setText( fn );

		recordsTableView->setModel( new MergeTableModel( mModel->merge() ) );
		recordsTableView->resizeColumnsToContents();
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
	/// Reload button clicked handler
	///
	void MergeView::onReloadButtonClicked()
	{
		mModel->merge()->reloadSource();
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
