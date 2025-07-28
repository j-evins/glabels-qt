/*  PrintView.cpp
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


#include "PrintView.h"

#include "model/Settings.h"

#include <QPrinter>
#include <QPrintDialog>
#include <QtDebug>


namespace glabels
{

	///
	/// Constructor
	///
	PrintView::PrintView( QWidget *parent )
		: QWidget(parent), mModel(nullptr), mBlocked(false)
	{
		setupUi( this );

		titleLabel->setText( QString( "<span style='font-size:18pt;'>%1</span>" ).arg( tr("Print") ) );

		preview->setRenderer( &mRenderer );
	}


	///
	/// Set Model
	///
	void PrintView::setModel( model::Model* model )
	{
		mModel = model;
		mRenderer.setModel( mModel );

		connect( mModel, SIGNAL(changed()), this, SLOT(onModelChanged()) );

		onModelChanged();
	}


	///
	/// Model changed handler
	///
	void PrintView::onModelChanged()
	{
		printRangeStartPositionSpin->setRange( 1, mModel->frame()->nLabels() );
		printRangeLastPositionSpin->setRange( 1, mModel->frame()->nLabels() );
		mergeStartPositionSpin->setRange( 1, mModel->frame()->nLabels() );

		printRangeBox->setVisible( mModel->merge()->keys().empty() );
		mergeBox->setVisible( !mModel->merge()->keys().empty() );
		mergeOnlyOptions->setVisible( !mModel->merge()->keys().empty() );

		onFormChanged();
	}


	///
	/// Update view
	///
	void PrintView::updateView()
	{
		if ( mRenderer.nPages() == 1 )
		{
			if ( mRenderer.nItems() == 1 )
			{
				printDescriptionLabel->setText( tr("(Will print a total of 1 item on 1 page.)") );
			}
			else
			{
				printDescriptionLabel->setText( tr("(Will print a total of %1 items on 1 page.)")
				                                .arg(mRenderer.nItems()) );
			}
		}
		else
		{
			printDescriptionLabel->setText( tr("(Will print a total of %1 items on %2 pages.)")
			                                .arg(mRenderer.nItems()).arg(mRenderer.nPages()) );
		}

		pageSpin->setRange( 1, mRenderer.nPages() );
		nPagesLabel->setText( QString::number( mRenderer.nPages() ) );

		mRenderer.setIPage( pageSpin->value() - 1 ); // Update preview
	}


	///
	/// Form changed handler
	///
	void PrintView::onFormChanged()
	{
		if ( !mBlocked )
		{
			mBlocked = true;
			
			if ( mModel->merge()->keys().empty() )
			{
				// Simple project (no merge)
				if ( printRangePagesRadio->isChecked() )
				{
					// Print full sheets of labels
					int nItemsPerPage = mModel->frame()->nLabels();

					printRangePagesSpin->setEnabled( true );

					printRangeStartPositionSpin->setEnabled( false );
					printRangeLastPositionSpin->setEnabled( false );

					printRangeStartPositionSpin->setMaximum( nItemsPerPage );
					printRangeLastPositionSpin->setMinimum( 1 );

					printRangeStartPositionSpin->setValue( 1 );
					printRangeLastPositionSpin->setValue( nItemsPerPage );

					mRenderer.setNCopies( printRangePagesSpin->value()*nItemsPerPage );
					mRenderer.setStartItem( 0 );
				}
				else
				{
					// Print a partial sheet of labels
					int iStart = printRangeStartPositionSpin->value();
					int iLast  = printRangeLastPositionSpin->value();

					printRangePagesSpin->setEnabled( false );

					printRangeStartPositionSpin->setEnabled( true );
					printRangeLastPositionSpin->setEnabled( true );

					printRangeStartPositionSpin->setMaximum( iLast );
					printRangeLastPositionSpin->setMinimum( iStart );

					mRenderer.setNCopies( iLast - iStart + 1 );
					mRenderer.setStartItem( iStart - 1 );
				}
			}
			else
			{
				// Project with merge
				bool isMultipleCopies = mergeCopiesSpin->value() > 1;

				mergeCollateCombo->setEnabled ( isMultipleCopies );
				mergeGroupCombo->setEnabled ( isMultipleCopies );
				
				mRenderer.setNCopies( mergeCopiesSpin->value() );
				mRenderer.setIsCollated( mergeCollateCombo->currentIndex() == 1 );
				mRenderer.setAreGroupsContiguous( mergeGroupCombo->currentIndex() == 0 );
				mRenderer.setStartItem( mergeStartPositionSpin->value() - 1 );
			}

			mRenderer.setPrintOutlines( printOutlinesCheck->isChecked() );
			mRenderer.setPrintCropMarks( printCropMarksCheck->isChecked() );
			mRenderer.setPrintReverse( printReverseCheck->isChecked() );

			updateView();
		
			mBlocked = false;
		}
	}


	///
	/// Print Button Clicked handler
	///
	void PrintView::onPrintButtonClicked()
	{
		QPrinter printer( QPrinter::HighResolution );
		printer.setColorMode( QPrinter::Color );
		printer.setPrinterName( model::Settings::recentPrinter() );

		QPrintDialog printDialog( &printer, this );
		printDialog.setOption( QAbstractPrintDialog::PrintToFile,        true );
		printDialog.setOption( QAbstractPrintDialog::PrintSelection,     false );
		printDialog.setOption( QAbstractPrintDialog::PrintPageRange,     false );
		printDialog.setOption( QAbstractPrintDialog::PrintShowPageSize,  true );
		printDialog.setOption( QAbstractPrintDialog::PrintCollateCopies, false );
		printDialog.setOption( QAbstractPrintDialog::PrintCurrentPage,   false );

		if ( printDialog.exec() == QDialog::Accepted )
		{
			mRenderer.print( &printer );

			model::Settings::setRecentPrinter( printer.printerName() );
		}
	}


} // namespace glabels
