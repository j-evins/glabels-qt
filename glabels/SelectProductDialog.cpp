/*  SelectProductDialog.cpp
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


#include "SelectProductDialog.h"

#include "NotebookUtil.h"
#include "TemplatePickerItem.h"

#include "model/Db.h"
#include "model/Settings.h"

#include <QtDebug>


namespace glabels
{
	
	///
	/// Constructor
	///
	SelectProductDialog::SelectProductDialog( QWidget *parent )
		: QDialog(parent)
	{
		setupUi( this );
		productInfoWidget->setVisible( false );

		pageSizeIsoCheck->setChecked( model::Settings::searchIsoPaperSizes() );
		pageSizeUsCheck->setChecked( model::Settings::searchUsPaperSizes() );
		pageSizeOtherCheck->setChecked( model::Settings::searchOtherPaperSizes() );

		allCategoriesRadio->setChecked( model::Settings::searchAllCategories() );
		selectedCategoriesRadio->setChecked( !model::Settings::searchAllCategories() );

		categoriesCheckContainer->setEnabled( !model::Settings::searchAllCategories() );
		mCategoryIdList = model::Settings::searchCategoryList();
	
		QList<model::Category*> categories = model::Db::categories();
		foreach ( model::Category *category, categories )
		{
			QCheckBox* check = new QCheckBox( category->name() );
			check->setChecked( mCategoryIdList.contains( category->id() ) );
			categoriesLayout->addWidget( check );

			mCheckList.append( check );
			mCheckToCategoryMap[check] = category->id();

			connect( check, SIGNAL(clicked()), this, SLOT(onCategoryCheckClicked()) );
		}

		NotebookUtil::establishSize( modeNotebook );

		if ( templatePicker->mode() == QListView::IconMode )
		{
			viewModeButton->setIcon( QIcon::fromTheme( "glabels-view-list" ) );
			viewModeButton->setToolTip( tr( "List View" ) );
		}
		else
		{
			viewModeButton->setIcon( QIcon::fromTheme( "glabels-view-grid" ) );
			viewModeButton->setToolTip( tr( "Grid View" ) );
		}
		
		QList<model::Template*> tmplates = model::Db::templates();
		templatePicker->setTemplates( tmplates );

		if ( model::Settings::recentTemplateList().count() > 0 )
		{
			modeNotebook->setCurrentIndex(1);
		}

		onModeTabChanged();
	}

	
	///
	/// Get selected template
	///
	const model::Template* SelectProductDialog::tmplate() const
	{
		if ( mHasSelection )
		{
			return templatePicker->selectedTemplate();
		}
		else
		{
			return nullptr;
		}
	}


	///
	/// Mode Notebook Tab Changed Slot
	///
	void SelectProductDialog::onModeTabChanged()
	{
		switch (modeNotebook->currentIndex())
		{
		case 0:
			// Search Tab
			templatePicker->applyFilter( searchEntry->text(),
			                             pageSizeIsoCheck->isChecked(),
			                             pageSizeUsCheck->isChecked(),
			                             pageSizeOtherCheck->isChecked(),
			                             allCategoriesRadio->isChecked(),
			                             mCategoryIdList );
			break;
		case 1:
			// Recent Tab
			templatePicker->applyFilter( model::Settings::recentTemplateList() );
			break;
		default:
			qDebug() << "onModeTabChanged(): unknown tab!";
		}
	}


	///
	/// Search Entry Text Changed Slot
	///
	void SelectProductDialog::onSearchEntryTextChanged()
	{
		templatePicker->applyFilter( searchEntry->text(),
		                             pageSizeIsoCheck->isChecked(),
		                             pageSizeUsCheck->isChecked(),
		                             pageSizeOtherCheck->isChecked(),
		                             allCategoriesRadio->isChecked(),
		                             mCategoryIdList );
	}


	///
	/// Page Size Check Clicked Slot
	///
	void SelectProductDialog::onPageSizeCheckClicked()
	{
		model::Settings::setSearchIsoPaperSizes( pageSizeIsoCheck->isChecked() );
		model::Settings::setSearchUsPaperSizes( pageSizeUsCheck->isChecked() );
		model::Settings::setSearchOtherPaperSizes( pageSizeOtherCheck->isChecked() );

		templatePicker->applyFilter( searchEntry->text(),
		                             pageSizeIsoCheck->isChecked(),
		                             pageSizeUsCheck->isChecked(),
		                             pageSizeOtherCheck->isChecked(),
		                             allCategoriesRadio->isChecked(),
		                             mCategoryIdList );
	}


	///
	/// Category Radio Clicked Slot
	///
	void SelectProductDialog::onCategoryRadioClicked()
	{
		categoriesCheckContainer->setEnabled( selectedCategoriesRadio->isChecked() );
		loadCategoryList();

		templatePicker->applyFilter( searchEntry->text(),
		                             pageSizeIsoCheck->isChecked(),
		                             pageSizeUsCheck->isChecked(),
		                             pageSizeOtherCheck->isChecked(),
		                             allCategoriesRadio->isChecked(),
		                             mCategoryIdList );

		model::Settings::setSearchAllCategories( allCategoriesRadio->isChecked() );
	}


	///
	/// Category Check Clicked Slot
	///
	void SelectProductDialog::onCategoryCheckClicked()
	{
		loadCategoryList();

		templatePicker->applyFilter( searchEntry->text(),
		                             pageSizeIsoCheck->isChecked(),
		                             pageSizeUsCheck->isChecked(),
		                             pageSizeOtherCheck->isChecked(),
		                             allCategoriesRadio->isChecked(),
		                             mCategoryIdList );


		model::Settings::setSearchCategoryList( mCategoryIdList );
	}


	///
	/// View Mode Button Clicked Slot
	///
	void SelectProductDialog::onViewModeButtonClicked()
	{
		if ( templatePicker->mode() == QListView::IconMode )
		{
			templatePicker->setMode( QListView::ListMode );

			viewModeButton->setIcon( QIcon::fromTheme( "glabels-view-list" ) );
			viewModeButton->setToolTip( tr( "List View" ) );
		}
		else
		{
			templatePicker->setMode( QListView::IconMode );

			viewModeButton->setIcon( QIcon::fromTheme( "glabels-view-grid" ) );
			viewModeButton->setToolTip( tr( "Grid View" ) );
		}
	}


	///
	/// Template Picker Selection Changed Slot
	///
	void SelectProductDialog::onTemplatePickerSelectionChanged()
	{
		auto* tmplate   = templatePicker->selectedTemplate();
		if ( !tmplate )
		{
			productInfoWidget->setVisible( false );
			selectButton->setEnabled( false );
			return;
		}
		
		auto* frame     = tmplate->frames().first();

		preview->setTemplate( tmplate );

		const model::Vendor* vendor = model::Db::lookupVendorFromName( tmplate->brand() );
		if ( (vendor != nullptr) && (vendor->url() != nullptr) )
		{
			QString markup = QString( "<a href='%1'>%2</a>" ).arg( vendor->url(), vendor->name() );
			vendorLabel->setText( markup );
		}
		else
		{
			vendorLabel->setText( tmplate->brand() );
		}

		if ( tmplate->productUrl() != nullptr )
		{
			QString markup = QString( "<a href='%1'>%2</a>" ).arg( tmplate->productUrl(), tmplate->part() );
			partLabel->setText( markup );
		}
		else
		{
			partLabel->setText( tmplate->part() );
		}

		descriptionLabel->setText( tmplate->description() );
		pageSizeLabel->setText( tmplate->paperDescription( model::Settings::units() ) );
		labelSizeLabel->setText( frame->sizeDescription( model::Settings::units() ) );
		layoutLabel->setText( frame->layoutDescription() );

		productInfoWidget->setVisible( true );
		selectButton->setEnabled( true );
	}


	///
	/// Select Button Clicked Slot
	///
	void SelectProductDialog::onSelectButtonClicked()
	{
		mHasSelection = true;
		close();
	}


	///
	/// Cancel Button Clicked Slot
	///
	void SelectProductDialog::onCancelButtonClicked()
	{
		close();
	}


	///
	/// Load category list
	///
	void SelectProductDialog::loadCategoryList()
	{
		mCategoryIdList.clear();

		foreach( QCheckBox* check, mCheckList )
		{
			if ( check->isChecked() )
			{
				mCategoryIdList.append( mCheckToCategoryMap[check] );
			}
		}
	}

} // namespace glabels
