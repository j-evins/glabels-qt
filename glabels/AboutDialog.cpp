//  AboutDialog.cpp
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


#include "AboutDialog.hpp"

#include "model/Version.hpp"

#include <QDebug>
#include <QDesktopServices>
#include <QUrl>


namespace glabels
{

        ///
        /// Constructor
        ///
        AboutDialog::AboutDialog( QWidget *parent )
                : QDialog(parent)
        {
                setupUi( this );

                QString version = tr("Version") + " " + model::Version::LONG_STRING;

                QString description = tr("A program to create labels and business cards.");

                QString copyright = "Copyright &copy; 2016-2026 Jaye Evins <evins@snaught.com>";

                QString licenseParagraph1 =
                        tr( "gLabels is free software: you can redistribute it and/or modify "
                            "it under the terms of the GNU General Public License as published by "
                            "the Free Software Foundation, either version 3 of the License, or "
                            "(at your option) any later version." );

                QString licenseParagraph2 =
                        tr( "gLabels is distributed in the hope that it will be useful, "
                            "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                            "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
                            "GNU General Public License for more details." );

                QString markup =
                        "<p align='center'>" + version + "</p>" +
                        "<p align='center'>" + description + "</p>" +
                        "<p align='center'>" + copyright + "</p>" +
                        "<p align='left'>"   + licenseParagraph1 + "</p>" +
                        "<p align='left'>"   + licenseParagraph2 + "</p>";

                aboutLabel->setText( markup );
        }


        ///
        /// "License" Button Clicked Slot
        ///
        void AboutDialog::onLicenseButtonClicked()
        {
                QDesktopServices::openUrl( QUrl("http://www.gnu.org/licenses/gpl-3.0.txt") );
        }


        ///
        /// "Website" Button Clicked Slot
        ///
        void AboutDialog::onWebsiteButtonClicked()
        {
                QDesktopServices::openUrl( QUrl(model::Version::WEBSITE) );
        }

} // namespace glabels
