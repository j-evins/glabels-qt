/*  StartupView.cpp
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

#include "StartupView.h"

#include "File.h"
#include "MainWindow.h"

#include "model/Settings.h"

#include <QAction>
#include <QFileInfo>
#include <QMenu>
#include <QtDebug>


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

		auto* recentMenu = new QMenu();
		for ( auto& filename : model::Settings::recentFileList() )
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

} // namespace glabels
