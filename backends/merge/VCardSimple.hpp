//  Merge/VCardSimple.hpp
//
//  Copyright (C) 2016-2026  Jaye Evins <evins@snaught.com>
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

#ifndef merge_VCardSimple_hpp
#define merge_VCardSimple_hpp


#include "Merge.hpp"

#include <QFile>


namespace glabels::merge
{

        ///
        /// VCardSimple Merge Backend
        ///
        struct VCardSimple : public Merge
        {

                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        protected:
                VCardSimple();
                VCardSimple( const VCardSimple* merge );
                virtual ~VCardSimple() = default;


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
                // Private data
                /////////////////////////////////
        private:
                QFile          mFile;
                QStringList    mKeys;
                qsizetype      mNFieldsMax{0};
        };

}


#endif // merge_VCardSimple_hpp
