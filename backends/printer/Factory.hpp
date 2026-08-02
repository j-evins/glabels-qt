//  Printer/Factory.hpp
//
//  Copyright (C) 2026  gLabels-qt contributors
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

#ifndef printer_Factory_hpp
#define printer_Factory_hpp


#include <QCoreApplication>
#include <QString>
#include <QMap>


namespace glabels::printer
{

        // Forward references
        class Backend;


        ///
        /// Printer Backend Factory
        ///
        class Factory
        {
                Q_DECLARE_TR_FUNCTIONS(Factory)


                /////////////////////////////////
                // Backend Type
                /////////////////////////////////
        public:
                enum BackendType { QT_PRINTER, BLUETOOTH_ESCPOS };


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

                static Backend* createBackend( const QString& id );


                /////////////////////////////////
                // private methods
                /////////////////////////////////
        private:
                using CreateFct = Backend* (*)( const QString& id );

                static void registerBackend( const QString&   idPattern,
                                             const QString&   name,
                                             BackendType      type,
                                             CreateFct        create );


                /////////////////////////////////
                // private data
                /////////////////////////////////
                class BackendEntry
                {
                public:
                        QString     idPattern;
                        QString     name;
                        BackendType type;
                        CreateFct   create;
                };

                static QMap<QString,BackendEntry> mBackendMap;
        };

}


#endif // printer_Factory_hpp
