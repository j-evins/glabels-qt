//  Merge/Text.hpp
//
//  Copyright (C) 2016  Jaye Evins <evins@snaught.com>
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

#ifndef merge_Text_hpp
#define merge_Text_hpp


#include "Merge.hpp"

#include <QFile>


namespace glabels::merge
{

        ///
        /// Text Merge Backend
        ///
        struct Text : public Merge
        {

                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        protected:
                Text( QChar delimiter, bool line1HasKeys );
                Text( const Text* merge );
                virtual ~Text() = default;


                /////////////////////////////////
                // Implementation of virtual methods
                /////////////////////////////////
        public:
                QStringList keys() const override;
                QString primaryKey() const override;
        protected:
                void open() override;
                void close() override;
                Record readNextRecord() override;


                /////////////////////////////////
                // Private methods
                /////////////////////////////////
                QString keyFromIndex( int iField ) const;
                QStringList parseLine();


                /////////////////////////////////
                // Private data
                /////////////////////////////////
        private:
                QChar mDelimeter;
                bool  mLine1HasKeys;

                QFile          mFile;
                QStringList    mKeys;
                int            mNFieldsMax;
        };

}


#endif // merge_Text_hpp
