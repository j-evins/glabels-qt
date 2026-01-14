//  StartupView.cpp
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


#include "StartupView.hpp"

#include "File.hpp"
#include "MainWindow.hpp"

#include "model/Settings.hpp"

#include <QAction>
#include <QDebug>
#include <QFileInfo>
#include <QMenu>


namespace glabels
{

        ///
        /// Constructor
        ///
        StartupView::StartupView( MainWindow* window )
                : QWidget(window), mWindow(window)
        {
                setupUi( this );

                QString titleImage = ":images/glabels-label-designer.png";
                titleLabel->setPixmap( QPixmap( titleImage ) );

                recentProjectButton->setEnabled( model::Settings::recentFileList().size() > 0 );

                loadRecentsMenu();

                connect( model::Settings::instance(), SIGNAL(changed()), this, SLOT(onSettingsChanged()) );
        }


        ///
        /// "New Project" Button Clicked Slot
        ///
        void StartupView::onNewProjectButtonClicked()
        {
                File::newLabel( mWindow );
        }


        ///
        /// "Open Project" Button Clicked Slot
        ///
        void StartupView::onOpenProjectButtonClicked()
        {
                File::open( mWindow );
        }


        ///
        /// "Open Recent" Action Activated Slot
        ///
        void StartupView::onOpenRecentAction()
        {
                QAction* action = qobject_cast<QAction*>( sender() );
                if ( action )
                {
                        File::open( action->data().toString(), mWindow );
                }
        }


        ///
        /// Settings changed Slot
        ///
        void StartupView::onSettingsChanged()
        {
                // reload recents menu
                loadRecentsMenu();
        }


        ///
        /// Create recents menu
        ///
        void StartupView::loadRecentsMenu()
        {
                auto fileList = model::Settings::recentFileList();

                auto* recentMenu = new QMenu();

                for ( auto& filename : fileList )
                {
                        QString basename = QFileInfo( filename ).completeBaseName();
                        auto* action = new QAction( basename, this );
                        action->setIcon( QIcon::fromTheme( "glabels" ) );
                        action->setData( filename );
                        connect( action, SIGNAL(triggered()), this, SLOT(onOpenRecentAction()) );
                        recentMenu->addAction( action );
                }
                recentMenu->setMinimumWidth( recentProjectButton->minimumWidth() );

                recentProjectButton->setMenu( recentMenu );
                recentProjectButton->setEnabled( fileList.size() != 0 );
        }


} // namespace glabels
