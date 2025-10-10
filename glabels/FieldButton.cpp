/*  FieldButton.cpp
 *
 *  Copyright (C) 2019  Jaye Evins <evins@snaught.com>
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

#include "FieldButton.h"

#include <QLineEdit>
#include <QStandardItemModel>


namespace glabels
{

	///
	/// Constructor
	///
	FieldButton::FieldButton( QWidget* parent ) : QPushButton(parent)
	{
		setEnabled( false );
		setMenu( &mMenu );
		
		connect( &mMenu, SIGNAL(triggered(QAction*)),
		         this, SLOT(onMenuActionTriggered(QAction*)) );
	}


	///
	/// Set Keys
	///
	void FieldButton::setKeys( const merge::Merge*     merge,
	                           const model::Variables& variables )
	{
		// Clear old keys
		mMenu.clear();
		
		// Add merge keys, if any
		mMenu.addSection( tr("Merge fields") );
		for ( auto& key : merge->keys() )
		{
			auto* action = mMenu.addAction( QString( "${%1}" ).arg( key ) );
			action->setData( key );
		}
		if ( merge->keys().empty() )
		{
			auto* action = mMenu.addAction( "None" );
			action->setEnabled( false );
		}

		// Add variable keys, if any
		mMenu.addSection( tr("Variables") );
		for ( auto& key : variables.keys() )
		{
			auto* action = mMenu.addAction( QString( "${%1}" ).arg( key ) );
			action->setData( key );
		}
		if ( variables.keys().empty() )
		{
			auto* action = mMenu.addAction( "None" );
			action->setEnabled( false );
		}

		setEnabled( !merge->keys().empty() || !variables.keys().empty() );
	}


	///
	/// onMenuActionTriggered slot
	///
	void FieldButton::onMenuActionTriggered( QAction* action )
	{
		emit keySelected( action->data().toString() );
	}

} // namespace glabels
