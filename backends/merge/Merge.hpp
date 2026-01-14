//  Merge/Merge.hpp
//
//  Copyright (C) 2015-2026  Jaye Evins <evins@snaught.com>
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

#ifndef merge_Merge_hpp
#define merge_Merge_hpp


#include "Record.hpp"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>


namespace glabels::merge
{

        // Forward references
        class Record;


        ///
        /// Merge Object
        ///
        class Merge : public QObject
        {
                Q_OBJECT


                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        protected:
                Merge() = default;
                Merge( const Merge* merge );
        public:
                virtual ~Merge() = default;


                /////////////////////////////////
                // Object duplication
                /////////////////////////////////
                virtual Merge* clone() const = 0;


                /////////////////////////////////
                // Properties
                /////////////////////////////////
        public:
                QString id() const;
                QString source() const;
                void setSource( const QString& source );
                void reloadSource();

                const QList<Record>& recordList( ) const;


                /////////////////////////////////
                // Selection methods
                /////////////////////////////////
        public:
                void setSelected( int i, bool state = true );
                void selectAll();
                void unselectAll();

                int nSelectedRecords() const;
                const QList<Record> selectedRecords() const;


                /////////////////////////////////
                // Virtual methods
                /////////////////////////////////
        public:
                virtual QStringList keys() const = 0;
                virtual QString primaryKey() const = 0;
        protected:
                virtual void open() = 0;
                virtual void close() = 0;
                virtual Record readNextRecord() = 0;


                /////////////////////////////////
                // Signals
                /////////////////////////////////
        signals:
                void sourceChanged();
                void selectionChanged();


                /////////////////////////////////
                // Private data
                /////////////////////////////////
        protected:
                QString       mId;
        private:
                QString       mSource;
                QList<Record> mRecordList;
        };

}


#endif // merge_Merge_hpp
