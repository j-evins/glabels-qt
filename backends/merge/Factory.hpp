//  Merge/Factory.hpp
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

#ifndef merge_Factory_hpp
#define merge_Factory_hpp


#include <QCoreApplication>
#include <QStringList>
#include <QMap>


namespace glabels::merge
{

        // Forward references
        class Merge;


        ///
        /// Factory
        ///
        class Factory
        {
                Q_DECLARE_TR_FUNCTIONS(Factory)


                /////////////////////////////////
                // Source Type
                /////////////////////////////////
        public:
                enum SourceType { NONE, FIXED, FILE };


                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        protected:
                Factory();


                /////////////////////////////////
                // Static methods
                /////////////////////////////////
        public:
                static void init();

                static Merge* createMerge( const QString& id );

                static QStringList nameList();
                static QString idToName( const QString& id );
                static QString nameToId( const QString& name );
                static SourceType idToType( const QString& id );
                static QString indexToId( int index );


                /////////////////////////////////
                // private methods
                /////////////////////////////////
        private:
                using CreateFct = Merge* (*)();

                static void registerBackend( const QString& id,
                                             const QString& name,
                                             SourceType     type,
                                             CreateFct      create );


                /////////////////////////////////
                // private data
                /////////////////////////////////
                class BackendEntry
                {
                public:
                        QString    id;
                        QString    name;
                        SourceType type;
                        CreateFct  create;
                };

                static QMap<QString,BackendEntry> mBackendIdMap;
                static QMap<QString,BackendEntry> mBackendNameMap;

                static QStringList mNameList;
        };

}


#endif // merge_Factory_hpp
