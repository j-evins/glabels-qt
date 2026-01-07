//  Style.hpp
//
//  Copyright (C) 2013  Jaye Evins <evins@snaught.com>
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

#ifndef barcode_Style_hpp
#define barcode_Style_hpp


#include <QString>


namespace glabels::barcode
{

        ///
        ///  Style Type
        ///
        class Style
        {

                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        public:
                Style ();

                Style ( const QString& id,
                        const QString& backendId,
                        const QString& name,
                        bool           canText,
                        bool           textOptional,
                        bool           canChecksum,
                        bool           checksumOptional,
                        const QString& defaultDigits,
                        bool           canFreeform,
                        int            preferedN );


                /////////////////////////////////
                // Properties
                /////////////////////////////////
                const QString& id() const;

                QString fullId() const;

                const QString& backendId() const;

                const QString& name() const;

                QString fullName() const;

                bool canText() const;

                bool textOptional() const;

                bool canChecksum() const;

                bool checksumOptional() const;

                const QString& defaultDigits() const;

                bool canFreeform() const;

                int preferedN() const;


                /////////////////////////////////
                // Methods
                /////////////////////////////////
        public:
                QString exampleDigits( int n ) const;


                /////////////////////////////////
                // Operators
                /////////////////////////////////
        public:
                bool operator!=( const Style& other ) const;


                /////////////////////////////////
                // Private Data
                /////////////////////////////////
        private:
                QString mId;
                QString mBackendId;
                QString mName;
                bool    mCanText;
                bool    mTextOptional;
                bool    mCanChecksum;
                bool    mChecksumOptional;
                QString mDefaultDigits;
                bool    mCanFreeform;
                int     mPreferedN;

        };

}


#endif // barcode_Style_hpp
