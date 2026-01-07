//  StartupView.hpp
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

#ifndef StartupView_hpp
#define StartupView_hpp


#include "ui_StartupView.h"


namespace glabels
{

        // Forward references
        class MainWindow;


        ///
        /// Startup View Widget
        ///
        class StartupView : public QWidget, public Ui_StartupView
        {
                Q_OBJECT


                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        public:
                StartupView( MainWindow* window );


                /////////////////////////////////
                // Slots
                /////////////////////////////////
        private slots:
                void onNewProjectButtonClicked();
                void onOpenProjectButtonClicked();
                void onOpenRecentAction();
                void onSettingsChanged();


                /////////////////////////////////
                // Private methods
                /////////////////////////////////
        private:
                void loadRecentsMenu();


                /////////////////////////////////
                // Private data
                /////////////////////////////////
        private:
                MainWindow* mWindow;

        };

}


#endif // StartupView_hpp
