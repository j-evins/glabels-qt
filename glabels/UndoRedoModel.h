/*  UndoRedoModel.h
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

#ifndef UndoRedoModel_h
#define UndoRedoModel_h


#include "model/Model.h"

#include <QList>
#include <QObject>
#include <QString>


namespace glabels
{

	///
	/// UndoRedoModel
	///
	class UndoRedoModel : public QObject
	{
		Q_OBJECT


		/////////////////////////////////
		// Life Cycle
		/////////////////////////////////
	public:
		UndoRedoModel( model::Model* model );
		virtual ~UndoRedoModel() = default;


		/////////////////////////////////
		// Public Methods
		/////////////////////////////////
	public:
		void checkpoint( const QString& description );
		void undo();
		void redo();
		bool canUndo() const;
		bool canRedo() const;
		QString undoDescription() const;
		QString redoDescription() const;


		/////////////////////////////////
		// Slots
		/////////////////////////////////
	private slots:
		void onSelectionChanged();
		

		/////////////////////////////////
		// Signals
		/////////////////////////////////
	signals:
		void changed();
		

		/////////////////////////////////
		// Private types
		/////////////////////////////////
	private:
		class State
		{
		public:
			State( model::Model* model, const QString& description );
			~State();

			model::Model* model;
			QString       description;
		};

		class Stack
		{
		public:
			Stack();
			~Stack();

			void push( State* state );
			State* pop();
			const State* topState() const;
			bool isEmpty() const;
			void clear();

		private:
			QList<State*> list;
		};
	

		/////////////////////////////////
		// Private data
		/////////////////////////////////
	private:
		model::Model*  mModel;

		Stack          mUndoStack;
		Stack          mRedoStack;
	
		bool           mNewSelection;
		QString        mLastDescription;

	};

}


#endif // UndoRedoModel_h
