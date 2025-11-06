/*  VariablesView.h
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

#ifndef VariablesView_h
#define VariablesView_h


#include "ui_VariablesView.h"

#include "model/Model.h"


namespace glabels
{

	// Forward references
	class UndoRedoModel;
	

	///
	/// Variables Property Editor Widget
	///
	class VariablesView : public QWidget, public Ui_VariablesView
	{
		Q_OBJECT


		/////////////////////////////////
		// Life Cycle
		/////////////////////////////////
	public:
		VariablesView( QWidget *parent = nullptr );
		virtual ~VariablesView() = default;


		/////////////////////////////////
		// Public methods
		/////////////////////////////////
		void setModel( model::Model* model, UndoRedoModel* undoRedoModel );


		/////////////////////////////////
		// Slots
		/////////////////////////////////
	private slots:
		void onTableSelectionChanged();
		void onAddButtonClicked();
		void onEditButtonClicked();
		void onDeleteButtonClicked();
		void onVariablesChanged();


		/////////////////////////////////
		// Private methods
		/////////////////////////////////
	private:
		void updateControls();
		void loadTable();
		void selectVariable( const QString& name );


		/////////////////////////////////
		// Private Data
		/////////////////////////////////
	private:
		model::Model*  mModel;
		UndoRedoModel* mUndoRedoModel;

	};

}


#endif // VariablesView_h
