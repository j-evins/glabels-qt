/*  PreferencesDialog.cpp
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

#include "PreferencesDialog.h"

#include "model/Settings.h"


namespace glabels
{

	///
	/// Constructor
	///
	PreferencesDialog::PreferencesDialog( QWidget *parent )
		: QDialog(parent)
	{
		setupUi( this );

		
		auto units = model::Settings::units();
		
		switch ( units.toEnum() )
		{
		case model::Units::IN:
			unitsInchesRadio->setChecked( true );
			break;
		case model::Units::MM:
			unitsMillimetersRadio->setChecked( true );
			break;
		case model::Units::CM:
			unitsCentimetersRadio->setChecked( true );
			break;
		case model::Units::PC:
			unitsPicasRadio->setChecked( true );
			break;
		default:
			unitsPointsRadio->setChecked( true );
			break;
		}


		switch ( model::Settings::gridOrigin() )
		{
		case model::Settings::ORIGIN_CENTER:
			gridOriginCenterRadio->setChecked( true );
			break;
		case model::Settings::ORIGIN_TL:
			gridOriginTlRadio->setChecked( true );
			break;
		default:
			gridOriginTlRadio->setChecked( true );
			break;
		}


		auto gridSpacing = model::Settings::gridSpacing();
		
		gridSpacingSpin->setDecimals( units.resolutionDigits() );
		gridSpacingSpin->setSingleStep( units.resolution() );
		gridSpacingSpin->setMinimum( units.resolution() );
		gridSpacingSpin->setSuffix( " " + units.toIdString() );
		gridSpacingSpin->setValue( gridSpacing.inUnits( units ) );


		connect( model::Settings::instance(), SIGNAL(changed()),
		         this, SLOT(onSettingsChanged()) );
	}


	///
	/// Units Radios Changed
	///
	void PreferencesDialog::onUnitsRadiosChanged()
	{
		if ( unitsInchesRadio->isChecked() )
		{
			model::Settings::setUnits( model::Units::in() );
		}
		else if ( unitsMillimetersRadio->isChecked() )
		{
			model::Settings::setUnits( model::Units::mm() );
		}
		else if ( unitsCentimetersRadio->isChecked() )
		{
			model::Settings::setUnits( model::Units::cm() );
		}
		else if ( unitsPicasRadio->isChecked() )
		{
			model::Settings::setUnits( model::Units::pc() );
		}
		else
		{
			model::Settings::setUnits( model::Units::pt() );
		}
	}


	///
	/// Grid Origin Radios Changed
	///
	void PreferencesDialog::onGridOriginRadiosChanged()
	{
		if ( gridOriginTlRadio->isChecked() )
		{
			model::Settings::setGridOrigin( model::Settings::ORIGIN_TL );
		}
		else if ( gridOriginCenterRadio->isChecked() )
		{
			model::Settings::setGridOrigin( model::Settings::ORIGIN_CENTER );
		}
	}


	///
	/// Grid Spacing Spin Changed
	///
	void PreferencesDialog::onGridSpacingSpinChanged()
	{
		auto units = model::Settings::units();

		auto spacing = model::Distance( gridSpacingSpin->value(), units );

		model::Settings::setGridSpacing( spacing );
	}


	///
	/// Grid Spacing Reset Button Clicked
	///
	void PreferencesDialog::onGridSpacingResetButtonClicked()
	{
		model::Settings::resetGridSpacing();
	}


	///
	/// Settings Changed
	///
	void PreferencesDialog::onSettingsChanged()
	{
		auto units = model::Settings::units();
		auto gridSpacing = model::Settings::gridSpacing();

		gridSpacingSpin->blockSignals( true );
		gridSpacingSpin->setDecimals( units.resolutionDigits() );
		gridSpacingSpin->setSingleStep( units.resolution() );
		gridSpacingSpin->setMinimum( units.resolution() );
		gridSpacingSpin->setSuffix( " " + units.toIdString() );
		gridSpacingSpin->setValue( gridSpacing.inUnits( units ) );
		gridSpacingSpin->blockSignals( false );
	}


} // namespace glabels
