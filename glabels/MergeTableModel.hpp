//  MergeTableModel.hpp
//
//  Copyright (C) 2025-2026  Jaye Evins <evins@snaught.com>
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

#ifndef MergeTableModel_hpp
#define MergeTableModel_hpp


#include "merge/Merge.hpp"

#include <QAbstractTableModel>


namespace glabels
{

        ///
        /// MergeTable proxy model
        ///
        class MergeTableModel : public QAbstractTableModel
        {
                Q_OBJECT

                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        public:
                MergeTableModel( merge::Merge* merge, QObject* parent = nullptr );


                /////////////////////////////////
                // Public methods
                /////////////////////////////////
        public:
                int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
                int columnCount( const QModelIndex &parent = QModelIndex() ) const override;

                QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;
                QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
                bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole ) override;
                Qt::ItemFlags flags( const QModelIndex& index ) const override;


                /////////////////////////////////
                // Private slots
                /////////////////////////////////
        private slots:
                void onSelectionChanged();


                /////////////////////////////////
                // Private Members
                /////////////////////////////////
        private:
                merge::Merge* mMerge;

                QStringList mDisplayKeys;

        };

}


#endif // MergeTableModel_hpp
