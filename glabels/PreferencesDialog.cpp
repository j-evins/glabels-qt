//  PreferencesDialog.cpp
//
//  Copyright (C) 2016-2026  Jaye Evins <evins@snaught.com>
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


#include "PreferencesDialog.hpp"

#include "AppearanceModel.hpp"

#include "model/Settings.hpp"

#include <QFileDialog>
#include <QStandardPaths>


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


                appearanceSystemRadio->setVisible( AppearanceModel::supportsSystemColorScheme() );


                switch ( model::Settings::colorScheme() )
                {
                case model::Settings::LIGHT_COLOR_SCHEME:
                        appearanceLightRadio->setChecked( true );
                        break;
                case model::Settings::DARK_COLOR_SCHEME:
                        appearanceDarkRadio->setChecked( true );
                        break;
#if QT_VERSION >= QT_VERSION_CHECK(6,8,0)
                case model::Settings::SYSTEM_COLOR_SCHEME:
                        appearanceSystemRadio->setChecked( true );
                        break;
#endif
                default:
                        appearanceLightRadio->setChecked( true );
                        break;
                }


                auto path = model::Settings::startupPath();
                if ( path.isEmpty() )
                {
                        startupParentRadio->setChecked( true );
                }
                else if ( path == QStandardPaths::writableLocation( QStandardPaths::HomeLocation ) )
                {
                        startupHomeRadio->setChecked( true );
                }
                else if ( path == QStandardPaths::writableLocation( QStandardPaths::DocumentsLocation ) )
                {
                        startupDocumentsRadio->setChecked( true );
                }
                else
                {
                        startupOtherRadio->setChecked( true );
                }

                if ( path.isEmpty() )
                {
                        // Start with ${HOME} when browsing for user-defined startup location
                        startupOtherLineEdit->setText( QStandardPaths::writableLocation( QStandardPaths::HomeLocation ) );
                }
                else
                {
                        // Otherwise, start from previously set startup path
                        startupOtherLineEdit->setText( path );
                }


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
        /// Appearance Mode Radios Changed
        ///
        void PreferencesDialog::onAppearanceModeRadiosChanged()
        {
                if ( appearanceLightRadio->isChecked() )
                {
                        model::Settings::setColorScheme( model::Settings::LIGHT_COLOR_SCHEME );
                }
                else if ( appearanceDarkRadio->isChecked() )
                {
                        model::Settings::setColorScheme( model::Settings::DARK_COLOR_SCHEME );
                }
#if QT_VERSION >= QT_VERSION_CHECK(6,8,0)
                else if ( appearanceSystemRadio->isChecked() )
                {
                        model::Settings::setColorScheme( model::Settings::SYSTEM_COLOR_SCHEME );
                }
#endif
        }


        ///
        /// Startup Location Radios Changed
        ///
        void PreferencesDialog::onStartupLocationRadiosChanged()
        {
                QString path;

                if ( startupParentRadio->isChecked() )
                {
                        path = "";
                }
                else if ( startupHomeRadio->isChecked() )
                {
                        path = QStandardPaths::writableLocation( QStandardPaths::HomeLocation );
                }
                else if ( startupDocumentsRadio->isChecked() )
                {
                        path = QStandardPaths::writableLocation( QStandardPaths::DocumentsLocation );
                }
                else if ( startupOtherRadio->isChecked() )
                {
                        path = startupOtherLineEdit->text();
                }
                else
                {
                        qWarning() << "No startup location selected.  Should not happen!";
                        return;
                }

                model::Settings::setStartupPath( path );
                if ( !path.isEmpty() )
                {
                        QDir::setCurrent( path );
                }
        }


        ///
        /// Startup Location Radios Changed
        ///
        void PreferencesDialog::onStartupOtherBrowseButtonClicked()
        {
                auto path = QFileDialog::getExistingDirectory( this,
                                                               tr("Select startup directory"),
                                                               startupOtherLineEdit->text(),
                                                               QFileDialog::ShowDirsOnly );
                if ( !path.isEmpty() )
                {
                        startupOtherLineEdit->setText( path );
                        model::Settings::setStartupPath( path );
                        QDir::setCurrent( path );
                }
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
