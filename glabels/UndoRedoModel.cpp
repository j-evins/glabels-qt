/*  UndoRedoModel.cpp
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

#include "UndoRedoModel.h"

#include "model/Model.h"


namespace glabels
{

	///
	/// Constructor
	///
	UndoRedoModel::UndoRedoModel( model::Model* model )
	{
		mModel = model;
		mNewSelection = true;

		connect( model, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()) );
	}


	///
	/// Checkpoint
	///
	void UndoRedoModel::checkpoint( const QString& description )
	{
		//
		// Do not perform consecutive checkpoints that are identical.
		// E.g. moving an object by dragging, would produce a large number
		// of incremental checkpoints -- what we really want is a single
		// checkpoint so that we can undo the entire dragging effort with
		// one "undo"
		//
		if ( mNewSelection || (description != mLastDescription) )
		{

			/* Sever old redo "thread" */
			mRedoStack.clear();

			/* Save state onto undo stack. */
			auto* stateNow = new State( mModel, description );
			mUndoStack.push( stateNow );

			/* Track consecutive checkpoints. */
			mNewSelection = false;
			mLastDescription = description;

			emit changed();
		}
	}


	///
	/// Undo
	///
	void UndoRedoModel::undo()
	{
		State* oldState = mUndoStack.pop();
		auto* stateNow = new State( mModel, oldState->description );

		mRedoStack.push( stateNow );

		mModel->restore( oldState->model );
		delete oldState;
	
		mNewSelection = true;

		emit changed();
	}
	

	///
	/// Redo
	///
	void UndoRedoModel::redo()
	{
		State* oldState = mRedoStack.pop();
		auto* stateNow = new State( mModel, oldState->description );

		mUndoStack.push( stateNow );

		mModel->restore( oldState->model );
		delete oldState;
	
		mNewSelection = true;

		emit changed();
	}
	

	///
	/// Can we undo?
	///
	bool UndoRedoModel::canUndo() const
	{
		return !mUndoStack.isEmpty();
	}

	
	///
	/// Can we redo?
	///
	bool UndoRedoModel::canRedo() const
	{
		return !mRedoStack.isEmpty();
	}

	
	///
	/// Undo description
	///
	QString UndoRedoModel::undoDescription() const
	{
		if ( canUndo() )
		{
			return mUndoStack.topState()->description;
		}
		else
		{
			return "";
		}
	}

	
	///
	/// Redo description
	///
	QString UndoRedoModel::redoDescription() const
	{
		if ( canRedo() )
		{
			return mRedoStack.topState()->description;
		}
		else
		{
			return "";
		}
	}

	
	///
	/// Selection changed handler
	///
	void UndoRedoModel::onSelectionChanged()
	{
		mNewSelection = true;
	}


	///
	/// State constructor
	///
	UndoRedoModel::State::State( model::Model* model, const QString& description )
	{
		this->model = model->save();
		this->description = description;
	}


	///
	/// State destructor
	///
	UndoRedoModel::State::~State()
	{
		delete model;
	}


	///
	/// Stack constructor
	///
	UndoRedoModel::Stack::Stack()
	{
		// empty
	}


	///
	/// Stack destructor
	///
	UndoRedoModel::Stack::~Stack()
	{
		clear();
	}


	///
	/// Push state onto stack
	///
	void UndoRedoModel::Stack::push( UndoRedoModel::State* state )
	{
		list.push_front( state );
	}
	

	///
	/// Pop state from stack
	///
	UndoRedoModel::State* UndoRedoModel::Stack::pop()
	{
		return list.takeFirst();
	}


	///
	/// Peek at state at top of stack
	///
	const UndoRedoModel::State* UndoRedoModel::Stack::topState() const
	{
		return list.first();
	}


	///
	/// Is stack empty?
	///
	bool UndoRedoModel::Stack::isEmpty() const
	{
		return list.isEmpty();
	}


	///
	/// Clear stack
	///
	void UndoRedoModel::Stack::clear()
	{
		while ( !isEmpty() )
		{
			delete pop();
		}
	}

} // namespace glabels
