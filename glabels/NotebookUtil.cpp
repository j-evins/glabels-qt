//  NotebookUtil.cpp
//
//  Copyright (C) 2015  Jaye Evins <evins@snaught.com>
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


#include "NotebookUtil.hpp"

#include <QLayout>


namespace glabels
{

        ///
        /// Establish notebook size based on largest page
        ///
        void NotebookUtil::establishSize( QTabWidget* notebook )
        {
                // Establish size of notebook size based on its largest page.
                // This may not be established until runtime, due to varying
                // lengths of translated strings or may even be influenced by
                // the current theme and fonts.  Without doing this, the
                // notebook may change size as tabs are selected for the first
                // time, which can be very annoying.

                // This hack shows the top-level window containing the notebook,
                // then shows each page and invalidates the window's layout.
                // Finally it re-hides the window without ever having
                // relinquishing control to the Qt event loop.

                QWidget* window = notebook->window();
                int iTabSaved = notebook->currentIndex();
                int nTabs = notebook->count();

                window->show();
                for ( int iTab = 0; iTab < nTabs; iTab++ )
                {
                        notebook->setCurrentIndex( iTab );
                        window->layout()->invalidate();
                }
                window->hide();

                notebook->setCurrentIndex( iTabSaved );
        }

}
