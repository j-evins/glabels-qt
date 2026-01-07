//  AboutDialog.hpp
//
//  Copyright (C) 2016  Jaye Evins <evins@snaught.com>
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

#ifndef AboutDialog_hpp
#define AboutDialog_hpp


#include "ui_AboutDialog.h"


namespace glabels
{

        ///
        /// About Dialog Widget
        ///
        class AboutDialog : public QDialog, public Ui_AboutDialog
        {
                Q_OBJECT


                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        public:
                AboutDialog( QWidget *parent = nullptr );


                /////////////////////////////////
                // Slots
                /////////////////////////////////
        private slots:
                void onLicenseButtonClicked();
                void onWebsiteButtonClicked();

        };

}


#endif // AboutDialog_hpp
