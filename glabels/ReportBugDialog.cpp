//  ReportBugDialog.cpp
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


#include "ReportBugDialog.hpp"

#include "model/Version.hpp"

#include <QDebug>
#include <QDesktopServices>
#include <QUrl>


namespace glabels
{

        ///
        /// Constructor
        ///
        ReportBugDialog::ReportBugDialog( QWidget *parent )
                : QDialog(parent)
        {
                setupUi( this );

                QString title = tr("How to Report a Bug");
                titleLabel->setText( QString( "<span style='font-size:14pt;font-weight:bold;'>%1</span>" ).arg( title ) );

                QString directionsP1 =
                        tr( "To submit a bug report, click on the button below.  This will open a "
                            "web browser to the gLabels github issue tracking page." );

                p1Label->setText( QString( "<p>%1</p>" ).arg( directionsP1 ) );

                QString directionsP2 =
                        tr( "Before submitting a report, look through the existing issues for similar "
                            "or related bugs.  If the issue has already been reported, please consider "
                            "contributing to its report instead.  Otherwise, create a new issue report. "
                            "Please paste the following information into the issue description." );

                p2Label->setText( QString( "<p>%1</p>" ).arg( directionsP2 ) );

                infoText->append( model::Version::details() );

                QString directionsP3 =
                        tr( "Be sure to include a detailed description of the problem and how to "
                            "recreate it.  Attach any screenshots and/or example glabels project "
                            "files that may illustrate the problem." );

                p3Label->setText( QString( "<p>%1</p>" ).arg( directionsP3 ) );

        }


        ///
        /// "Copy" Button Clicked Slot
        ///
        void ReportBugDialog::onCopyButtonClicked()
        {
                infoText->selectAll();
                infoText->copy();
        }


        ///
        /// "Website" Button Clicked Slot
        ///
        void ReportBugDialog::onWebsiteButtonClicked()
        {
                QDesktopServices::openUrl( QUrl(model::Version::BUG_WEBSITE) );
        }

} // namespace glabels
