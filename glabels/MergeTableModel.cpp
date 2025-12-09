/*  MergeTableModel.cpp
 *
 *  Copyright (C) 2025  Jaye Evins <evins@snaught.com>
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


#include "MergeTableModel.h"

#include <QDebug>


namespace glabels
{

	///
	/// Constructor
	///
	MergeTableModel::MergeTableModel( merge::Merge* merge, QObject* parent )
		: QAbstractTableModel( parent ),
		  mMerge( merge )
	{
		// Copy keys, make sure primary key is first
		mDisplayKeys.push_back( mMerge->primaryKey() );
		for ( auto& key : mMerge->keys() )
		{
			if ( key != mMerge->primaryKey() )
			{
				mDisplayKeys.push_back( key );
			}
		}

		connect( mMerge, SIGNAL(selectionChanged()),
		         this, SLOT(onSelectionChanged()) );
	}


	///
	/// Row count
	///
	int MergeTableModel::rowCount( const QModelIndex& parent ) const
	{
		return mMerge->recordList().size();
	}


	///
	/// Column count
	///
	int MergeTableModel::columnCount( const QModelIndex& parent ) const
	{
		return mDisplayKeys.size() + 1;
	}


	///
	/// Header data
	///
	QVariant MergeTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
	{
		if ( orientation == Qt::Vertical )
		{
			return QAbstractTableModel::headerData( section, orientation, role );
		}

		if ( (role != Qt::DisplayRole) || section >= mDisplayKeys.size() )
		{
			return QVariant();
		}

		return mDisplayKeys[ section ];
	}


	///
	/// Data
	///
	QVariant MergeTableModel::data( const QModelIndex& index, int role ) const
	{
		if ( !index.isValid() )
		{
			return QVariant();
		}

		if ( (index.row() >= mMerge->recordList().size()) ||
		     (index.column() >= mDisplayKeys.size()) )
		{
			return QVariant();
		}


		if ( (role == Qt::CheckStateRole) && (index.column() == 0) )
		{
			auto record = mMerge->recordList()[ index.row() ];
			return record.isSelected() ? Qt::Checked : Qt::Unchecked;
		}

		if ( role == Qt::DisplayRole )
		{
			auto record = mMerge->recordList()[ index.row() ];
			auto key = mDisplayKeys[ index.column() ];

			if ( record.contains( key ) )
			{
				return record[ key ];
			}
		}

		return QVariant();
	}


	///
	/// Set data
	///
	bool MergeTableModel::setData( const QModelIndex& index, const QVariant& value, int role )
	{
		if ( !index.isValid() || (index.column() != 0) || (role != Qt::CheckStateRole) )
		{
			return false;
		}

		bool isChecked = static_cast<Qt::CheckState>(value.toInt()) != Qt::Unchecked;
		
		mMerge->blockSignals( true );
		mMerge->setSelected( index.row(), isChecked );
		mMerge->blockSignals( false );
		return true;
	}


	///
	/// Flags
	///
	Qt::ItemFlags MergeTableModel::flags( const QModelIndex& index ) const
	{
		if ( !index.isValid() )
		{
			return Qt::NoItemFlags;
		}

		if ( index.column() == 0 )
		{
			return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
		}

		return Qt::ItemIsEnabled;
	}


	///
	/// Selection changed handler
	///
	void MergeTableModel::onSelectionChanged()
	{
		for ( int iRow = 0; iRow < mMerge->recordList().size(); iRow++ )
		{
			auto index = createIndex( iRow, 0 );
			emit dataChanged( index, index, {Qt::CheckStateRole} );
		}
	}


} // namespace glabels
