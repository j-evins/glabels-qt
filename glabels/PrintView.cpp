//  PrintView.cpp
//
//  Copyright (C) 2013-2016  Jaye Evins <evins@snaught.com>
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


#include "PrintView.hpp"

#include "PrinterMonitor.hpp"

#include "model/Settings.hpp"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintDialog>


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

                auto* printerMonitor = PrinterMonitor::instance();
                loadDestinations( printerMonitor->availablePrinters() );
                connect( printerMonitor, SIGNAL(availablePrintersChanged(QStringList)),
                         this, SLOT(onAvailablePrintersChanged(QStringList)) );

                setDestination( model::Settings::recentPrinter() );

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
        /// Available printers changed handler
        ///
        void PrintView::onAvailablePrintersChanged( QStringList printers )
        {
                auto savedSelection = destinationCombo->currentText();
                loadDestinations( printers );
                setDestination( savedSelection );
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
                auto printerName = destinationCombo->currentText();
                auto printerInfo = QPrinterInfo::printerInfo( printerName );
                bool isPrinter = !printerInfo.isNull();

                QPrinter printer( QPrinter::HighResolution );
                printer.setColorMode( QPrinter::Color );

                if ( isPrinter )
                {
                        printer.setPrinterName( printerName );
                        mRenderer.print( &printer );
                        model::Settings::setRecentPrinter( printerName );
                }
                else
                {
                        QString fileName =
                                QFileDialog::getSaveFileName( this,
                                                              tr("Print to file (PDF)"),
                                                              defaultPdf(),
                                                              tr("PDF files (*.pdf);;All files (*)"),
                                                              nullptr,
                                                              QFileDialog::DontConfirmOverwrite        );
                        if ( !fileName.isEmpty() )
                        {
                                if ( QFileInfo::exists(fileName) )
                                {
                                        QMessageBox msgBox( this );
                                        msgBox.setWindowTitle( tr("Print to file (PDF)") );
                                        msgBox.setIcon( QMessageBox::Warning );
                                        msgBox.setText( tr("%1 already exists.").arg(fileName) );
                                        msgBox.setInformativeText( tr("Do you want to overwrite it?") );
                                        msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
                                        msgBox.setDefaultButton( QMessageBox::No );

                                        if ( msgBox.exec() == QMessageBox::No )
                                        {
                                                return;
                                        }
                                }

                                printer.setOutputFileName( fileName );
                                printer.setOutputFormat( QPrinter::PdfFormat );
                                mRenderer.print( &printer );
                        }
                }
        }


        ///
        /// System Dialog Button Clicked handler
        ///
        void PrintView::onSystemDialogButtonClicked()
        {
                QPrinter printer( QPrinter::HighResolution );
                printer.setColorMode( QPrinter::Color );
                printer.setPrinterName( destinationCombo->currentText() );

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

                        if ( !printer.printerName().isEmpty() )
                        {
                                model::Settings::setRecentPrinter( printer.printerName() );
                        }
                }
        }


        ///
        /// Load available printers
        ///
        void PrintView::loadDestinations( const QStringList& printers )
        {
                destinationCombo->blockSignals( true );

                destinationCombo->clear();
                for ( auto& printerName : printers )
                {
                        destinationCombo->addItem( QIcon::fromTheme( "glabels-print" ), printerName  );
                }

                if ( destinationCombo->count() )
                {
                        destinationCombo->insertSeparator( destinationCombo->count() );
                }
                destinationCombo->addItem( QIcon::fromTheme( "glabels-file-new" ), tr( "Print to file (PDF)" ) );

                destinationCombo->blockSignals( false );
        }


        ///
        /// Generate default PDF filename
        ///
        QString PrintView::defaultPdf()
        {
                if ( !mModel )
                {
                        return "output.pdf";
                }
                return mModel->dirPath() + "/" + mModel->shortName() + ".pdf";
        }


        ///
        /// Set destination to printerName if valid, or to a fallback value
        ///
        void PrintView::setDestination( const QString& printerName )
        {
                destinationCombo->blockSignals( true );

                auto printerInfo = QPrinterInfo::printerInfo( printerName );

                if ( !printerInfo.isNull() )
                {
                        // printerName is a valid printer
                        destinationCombo->setCurrentText( printerName );
                }
                else
                {
                        auto defaultPrinterName = QPrinterInfo::defaultPrinterName();
                        auto defaultPrinterInfo = QPrinterInfo::printerInfo( defaultPrinterName );
                        if ( !defaultPrinterInfo.isNull() )
                        {
                                // defaultPinterName is a valid printer
                                destinationCombo->setCurrentText( defaultPrinterName );
                        }
                        else
                        {
                                // No default printer available, set to first item in combo (probably "print to file")
                                destinationCombo->setCurrentIndex( 0 );
                        }
                }

                destinationCombo->blockSignals( false );
        }


} // namespace glabels
