//  EditVariableDialog.cpp
//
//  Copyright (C) 2019-2026  Jaye Evins <evins@snaught.com>
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


#include "EditVariableDialog.hpp"

#include "model/Settings.hpp"

#include <QPushButton>


namespace
{
        // All variable types. (must be in sorted order)
        const QVector<glabels::model::Variable::Type> allTypes = {
                glabels::model::Variable::Type::STRING,
                glabels::model::Variable::Type::INTEGER,
                glabels::model::Variable::Type::FLOATING_POINT,
                glabels::model::Variable::Type::COLOR
        };

        // All variable increments. (must be in sorted order)
        const QVector<glabels::model::Variable::Increment> allIncrements = {
                glabels::model::Variable::Increment::NEVER,
                glabels::model::Variable::Increment::PER_ITEM,
                glabels::model::Variable::Increment::PER_COPY,
                glabels::model::Variable::Increment::PER_PAGE
        };
}


namespace glabels
{

        ///
        /// Constructor
        ///
        EditVariableDialog::EditVariableDialog( QWidget *parent )
                : QDialog(parent)
        {
                setupUi( this );

                QRegularExpression reIdentifier( "[a-zA-Z_][a-zA-Z_0-9]*" );
                nameEdit->setValidator( new QRegularExpressionValidator( reIdentifier ) );

                colorValueButton->init( tr("Default"),
                                        QColor(0,0,0,255),
                                        QColor(0,0,0,255),
                                        false );

                for ( auto type : allTypes )
                {
                        typeCombo->addItem( model::Variable::typeToI18nString( type ) );
                }

                for ( auto type : allIncrements )
                {
                        incrementCombo->addItem( model::Variable::incrementToI18nString( type ) );
                }

                stepSizeEdit->setText( "1" );
        }


        ///
        /// Set variable
        ///
        void EditVariableDialog::setVariable( const model::Variable& variable )
        {
                typeCombo->setCurrentIndex( static_cast<int>(variable.type()) );
                nameEdit->setText( variable.name() );
                valueEdit->setText( variable.initialValue() );
                colorValueButton->setColor( QColor( variable.initialValue() ) );
                incrementCombo->setCurrentIndex( static_cast<int>(variable.increment()) );
                stepSizeEdit->setText( variable.stepSize() );

                updateControls();
        }


        ///
        /// Get variable
        ///
        model::Variable EditVariableDialog::variable() const
        {
                return model::Variable( static_cast<model::Variable::Type>(typeCombo->currentIndex()),
                                        nameEdit->text(),
                                        valueEdit->text(),
                                        static_cast<model::Variable::Increment>(incrementCombo->currentIndex()),
                                        stepSizeEdit->text() );
        }


        ///
        /// nameEdit Changed
        ///
        void EditVariableDialog::onNameEditChanged()
        {
                validateCurrentInputs();
        }


        ///
        /// typeCombo Changed
        ///
        void EditVariableDialog::onTypeComboChanged()
        {
                updateControls();
        }


        ///
        /// valueEdit Changed
        ///
        void EditVariableDialog::onValueEditChanged()
        {
                validateCurrentInputs();
        }


        ///
        /// colorValueButton Changed
        ///
        void EditVariableDialog::onColorValueButtonChanged()
        {
                valueEdit->setText( colorValueButton->colorNode().color().name() );
                validateCurrentInputs();
        }


        ///
        /// incrementCombo Changed
        ///
        void EditVariableDialog::onIncrementComboChanged()
        {
                updateControls();
        }


        ///
        /// stepSizeEdit Changed
        ///
        void EditVariableDialog::onStepSizeEditChanged()
        {
                validateCurrentInputs();
        }


        ///
        /// update controls
        ///
        void EditVariableDialog::updateControls()
        {
                auto type      = static_cast<model::Variable::Type>(typeCombo->currentIndex());
                auto increment = static_cast<model::Variable::Increment>(incrementCombo->currentIndex());

                switch (type)
                {

                case model::Variable::Type::INTEGER:
                        valueEdit->setValidator( new QIntValidator() );
                        stepSizeEdit->setValidator( new QIntValidator() );
                        break;

                case model::Variable::Type::FLOATING_POINT:
                        valueEdit->setValidator( new QDoubleValidator() );
                        stepSizeEdit->setValidator( new QDoubleValidator() );
                        break;

                default:
                        valueEdit->setValidator( nullptr );
                        stepSizeEdit->setValidator( nullptr );
                        break;

                }

                colorValueButton->setVisible( type == model::Variable::Type::COLOR );

                bool isNumeric = ( type == model::Variable::Type::INTEGER ) ||
                                 ( type == model::Variable::Type::FLOATING_POINT );

                incrementGroup->setVisible( isNumeric );
                stepSizeLabel->setEnabled( isNumeric && (increment != model::Variable::Increment::NEVER) );
                stepSizeEdit->setEnabled( isNumeric && (increment != model::Variable::Increment::NEVER) );

                validateCurrentInputs();
        }


        ///
        /// validate current inputs
        ///
        void EditVariableDialog::validateCurrentInputs()
        {
                bool hasValidIdentifier = nameEdit->hasAcceptableInput();
                bool hasValidValue      = valueEdit->hasAcceptableInput();
                bool hasValidStepSize   = stepSizeEdit->hasAcceptableInput();

                bool isValid = hasValidIdentifier && hasValidValue && hasValidStepSize;
                buttonBox->button(QDialogButtonBox::Ok)->setEnabled( isValid );
        }


} // namespace glabels
