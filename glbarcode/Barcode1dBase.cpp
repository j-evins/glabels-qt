//  Barcode1dBase.cpp
//
//  Copyright (C) 2013  Jaye Evins <evins@snaught.com>
//
//  This file is part of glbarcode++.
//
//  glbarcode++ is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  glbarcode++ is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with glbarcode++.  If not, see <http://www.gnu.org/licenses/>.
//


#include "Barcode1dBase.hpp"

#include "DrawingPrimitives.hpp"

#include <list>


namespace glbarcode
{

        /*
         * Barcode1dBase private data
         */
        struct Barcode1dBase::PrivateData {
                int dummy;
        };


        Barcode1dBase::Barcode1dBase()
        {
                d = new Barcode1dBase::PrivateData;
        }


        Barcode1dBase::~Barcode1dBase()
        {
                delete d;
        }


        Barcode& Barcode1dBase::build( const std::string& rawData,
                                       double             w,
                                       double             h )
        {
                std::string cookedData;     /* Preprocessed data */
                std::string displayText;    /* Text data to be displayed */
                std::string codedData;      /* Encoded data */

                clear();

                if ( rawData.empty() )
                {
                        setIsEmpty( true );
                        setIsDataValid( false );

                        setWidth( 0 );
                        setHeight( 0 );
                }
                else
                {
                        setIsEmpty( false );

                        if ( !validate( rawData ) )
                        {
                                setIsDataValid( false );

                                setWidth( 0 );
                                setHeight( 0 );
                        }
                        else
                        {
                                setIsDataValid( true );

                                cookedData  = preprocess( rawData );
                                codedData   = encode( cookedData );
                                displayText = prepareText( rawData );

                                vectorize( codedData, displayText, cookedData, w, h );

                                setWidth( w );
                                setHeight( h );
                        }
                }

                return *this;
        }


        /*
         * Default preprocess method
         */
        std::string Barcode1dBase::preprocess( const std::string& rawData )
        {
                return rawData;
        }


        /*
         * Default prepareText method
         */
        std::string Barcode1dBase::prepareText( const std::string& rawData )
        {
                return rawData;
        }


}
