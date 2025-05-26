/*  PropertiesView.cpp
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


#include "PropertiesView.h"

#include "SelectProductDialog.h"
#include "UndoRedoModel.h"

#include "model/Db.h"
#include "model/FrameContinuous.h"
#include "model/Settings.h"

#include <QStyledItemDelegate>
#include <QtDebug>


namespace glabels
{

	///
	/// Constructor
	///
	PropertiesView::PropertiesView( QWidget *parent )
		: QWidget(parent), mModel(nullptr), mUndoRedoModel(nullptr)
	{
		setupUi( this );

		titleLabel->setText( QString( "<span style='font-size:18pt;'>%1</span>" ).arg( tr("Properties") ) );

		// Hack to get orientationCombo item height to follow icon size plus padding 
		auto* itemDelegate = new QStyledItemDelegate();
		orientationCombo->setItemDelegate( itemDelegate );
		orientationCombo->setStyleSheet( "* QAbstractItemView::item { padding: 8px; }" );

		similarBrowser->setAttribute(Qt::WA_TranslucentBackground);
		similarBrowser->viewport()->setAutoFillBackground(false);

		connect( model::Settings::instance(), SIGNAL(changed()),
		         this, SLOT(onSettingsChanged()) );
		
		onSettingsChanged();
	}


	///
	/// Destructor
	///
	PropertiesView::~PropertiesView()
	{
		// empty
	}


	///
	/// Set Model
	///
	void PropertiesView::setModel( model::Model* model, UndoRedoModel* undoRedoModel )
	{
		mModel = model;
		mUndoRedoModel = undoRedoModel;

		connect( mModel, SIGNAL(sizeChanged()), this, SLOT(onLabelSizeChanged()) );

		onLabelSizeChanged();
	}


	///
	/// Settings changed handler
	///
	void PropertiesView::onSettingsChanged()
	{
		mUnits = model::Settings::units();
		if (mModel)
		{
			onLabelSizeChanged();
		}
	}


	///
	/// Label size changed handler
	///
	void PropertiesView::onLabelSizeChanged()
	{
		auto* tmplate   = mModel->tmplate();
		auto* frame     = tmplate->frames().first();
		bool  isRotated = mModel->rotate();

		preview->setTemplate( tmplate );
		preview->setShowArrow( true );
		preview->setRotate( isRotated );

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
		pageSizeLabel->setText( tmplate->paperDescription( mUnits ) );
		labelSizeLabel->setText( frame->sizeDescription( mUnits ) );
		layoutLabel->setText( frame->layoutDescription() );

		QStringList list = model::Db::getNameListOfSimilarTemplates( tmplate->name() );
		if ( list.isEmpty() )
		{
			similarProductsGroupBox->hide();
			similarProductsNullBox->show();
		}
		else
		{
			similarProductsGroupBox->show();
			similarProductsNullBox->hide();

			QString similarListString;
			foreach ( QString name, list )
			{
				similarListString += name + "\n";
			}
			similarBrowser->setText( similarListString );
		}

		orientationCombo->setEnabled( frame->w() != frame->h() );
		if ( frame->w() == frame->h() )
		{
			orientationCombo->setCurrentIndex(0);
		}
		else if ( frame->w() > frame->h() )
		{
			orientationCombo->setCurrentIndex( isRotated ? 1 : 0 );
		}
		else
		{
			orientationCombo->setCurrentIndex( isRotated ? 0 : 1 );
		}
		mOldOrientationIndex = orientationCombo->currentIndex();

		if ( auto* continuousFrame = dynamic_cast<const model::FrameContinuous*>( frame ) )
		{
			if (!mInLengthSpinChanged)
			{
				const QSignalBlocker blocker( lengthSpin );

				lengthSpin->setSuffix( " " + mUnits.toTrName() );
				lengthSpin->setDecimals( mUnits.resolutionDigits() );
				lengthSpin->setSingleStep( mUnits.resolution() );
				lengthSpin->setMinimum( continuousFrame->hMin().inUnits( mUnits ) );
				lengthSpin->setMaximum( continuousFrame->hMax().inUnits( mUnits ) );
				lengthSpin->setValue( continuousFrame->h().inUnits( mUnits ) );
			}

			parametersGroupBox->setVisible( true );
		}
		else
		{
			parametersGroupBox->setVisible( false );
		}
	}


	///
	/// Length spin changed handler
	///
	void PropertiesView::onLengthSpinChanged()
	{
		mInLengthSpinChanged = true;
		mModel->setH( model::Distance( lengthSpin->value(), mUnits ) );
		mInLengthSpinChanged = false;
	}


	///
	/// Orientation combo box changed handler
	///
	void PropertiesView::onOrientationActivated()
	{
		const model::Template* tmplate = mModel->tmplate();
		const model::Frame*    frame   = tmplate->frames().first();

		// Make sure index actually changed.
		int index = orientationCombo->currentIndex();
		if ( index != mOldOrientationIndex )
		{
			mOldOrientationIndex = index;

			mUndoRedoModel->checkpoint( tr("Product Rotate") );

			if ( frame->w() == frame->h() )
			{
				mModel->setRotate( false );
			}
			else if ( frame->w() > frame->h() )
			{
				mModel->setRotate( index == 1 );
			}
			else
			{
				mModel->setRotate( index == 0 );
			}
		}
	}


	///
	/// Change Product Button Clicked handler
	///
	void PropertiesView::onChangeProductButtonClicked()
	{
		SelectProductDialog selectProductDialog( this );
		selectProductDialog.exec();

		const model::Template* tmplate = selectProductDialog.tmplate();
		if ( tmplate )
		{
			mUndoRedoModel->checkpoint( tr("Change Product") );

			mModel->setTmplate( tmplate );

			// Don't rotate circular or round labels
			const model::Frame *frame = tmplate->frames().first();
			if ( frame->w() == frame->h() )
			{
				mModel->setRotate( false );
			}
		}
	}

} // namespace glabels
