/*  FieldButton.h
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

#ifndef FieldButton_h
#define FieldButton_h


#include "model/Variables.h"
#include "merge/Merge.h"

#include <QAction>
#include <QPushButton>
#include <QMenu>
#include <QStringList>


namespace glabels
{

	///
	/// Field Button
	///
	class FieldButton : public QPushButton
	{
		Q_OBJECT

		/////////////////////////////////
		// Life Cycle
		/////////////////////////////////
	public:
		FieldButton( QWidget* parent = nullptr );


		/////////////////////////////////
		// Signals
		/////////////////////////////////
	signals:
		void keySelected( QString key );


		/////////////////////////////////
		// Public Methods
		/////////////////////////////////
	public:
		void setKeys( const merge::Merge*     merge,
		              const model::Variables& variables );


		/////////////////////////////////
		// Slots
		/////////////////////////////////
	private slots:
		void onMenuActionTriggered( QAction* action );


		/////////////////////////////////
		// Private Data
		/////////////////////////////////
	private:
		QMenu mMenu;

	};

}


#endif // FieldButton_h
