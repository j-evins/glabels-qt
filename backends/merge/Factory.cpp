//  Merge/Factory.cpp
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


#include "Factory.hpp"

#include "None.hpp"
#include "TextCsv.hpp"
#include "TextCsvKeys.hpp"
#include "TextTsv.hpp"
#include "TextTsvKeys.hpp"
#include "TextColon.hpp"
#include "TextColonKeys.hpp"
#include "TextSemicolon.hpp"
#include "TextSemicolonKeys.hpp"


namespace glabels::merge
{

        //
        // Static data
        //
        QMap<QString,Factory::BackendEntry> Factory::mBackendIdMap;
        QMap<QString,Factory::BackendEntry> Factory::mBackendNameMap;
        QStringList Factory::mNameList;


        ///
        /// Constructor
        ///
        Factory::Factory()
        {
                registerBackend( None::id(),
                                 tr("None"),
                                 NONE,
                                 &None::create );

                registerBackend( TextCsv::id(),
                                 tr("Text: Comma Separated Values (CSV)"),
                                 FILE,
                                 &TextCsv::create );

                registerBackend( TextCsvKeys::id(),
                                 tr("Text: Comma Separated Values (CSV), keys on line 1"),
                                 FILE,
                                 &TextCsvKeys::create );

                registerBackend( TextTsv::id(),
                                 tr("Text: Tab Separated Values (TSV)"),
                                 FILE,
                                 &TextTsv::create );

                registerBackend( TextTsvKeys::id(),
                                 tr("Text: Tab Separated Values (TSV), keys on line 1"),
                                 FILE,
                                 &TextTsvKeys::create );

                registerBackend( TextColon::id(),
                                 tr("Text: Colon Separated Values"),
                                 FILE,
                                 &TextColon::create );

                registerBackend( TextColonKeys::id(),
                                 tr("Text: Colon Separated Values, keys on line 1"),
                                 FILE,
                                 &TextColonKeys::create );

                registerBackend( TextSemicolon::id(),
                                 tr("Text: Semicolon Separated Values"),
                                 FILE,
                                 &TextSemicolon::create );

                registerBackend( TextSemicolonKeys::id(),
                                 tr("Text: Semicolon Separated Values, keys on line 1"),
                                 FILE,
                                 &TextSemicolonKeys::create );
        }


        ///
        /// Initialize
        ///
        void Factory::init()
        {
                static Factory* singletonInstance = nullptr;
                if ( !singletonInstance )
                {
                        singletonInstance = new Factory();
                }
        }


        ///
        /// Create Merge object
        ///
        Merge* Factory::createMerge( const QString& id )
        {
                QMap<QString,BackendEntry>::iterator iBackend = mBackendIdMap.find( id );
                if ( iBackend != mBackendIdMap.end() )
                {
                        return iBackend->create();
                }

                return None::create();
        }


        ///
        /// Get name list
        ///
        QStringList Factory::nameList()
        {
                return mNameList;
        }


        ///
        /// Convert ID to name
        ///
        QString Factory::idToName( const QString& id )
        {
                if ( mBackendIdMap.contains( id ) )
                {
                        return mBackendIdMap[id].name;
                }
                else
                {
                        return tr("None");
                }
        }


        ///
        /// Convert name to ID
        ///
        QString Factory::nameToId( const QString& name )
        {
                if ( mBackendNameMap.contains( name ) )
                {
                        return mBackendNameMap[name].id;
                }
                else
                {
                        return "None";
                }
        }


        ///
        /// Convert ID to type
        ///
        Factory::SourceType Factory::idToType( const QString& id )
        {
                if ( mBackendIdMap.contains( id ) )
                {
                        return mBackendIdMap[id].type;
                }
                else
                {
                        return NONE;
                }
        }


        ///
        /// Lookup ID from index
        ///
        QString Factory::indexToId( int index )
        {
                if ( (index > 0) && (index < mNameList.size()) )
                {
                        QString name = mNameList[index];

                        return mBackendNameMap[ name ].id;
                }

                return "None";
        }


        ///
        /// Register backend
        ///
        void Factory::registerBackend( const QString& id,
                                       const QString& name,
                                       SourceType     type,
                                       CreateFct      create )
        {
                BackendEntry backend;

                backend.id     = id;
                backend.name   = name;
                backend.type   = type;
                backend.create = create;

                mBackendIdMap[ id ]     = backend;
                mBackendNameMap[ name ] = backend;

                mNameList << name;
        }

}
