//  Barcode2dBase.cpp
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


#include "Barcode2dBase.hpp"

#include "Constants.hpp"
#include "DrawingPrimitives.hpp"

#include <list>
#include <algorithm>


using namespace glbarcode::Constants;


namespace
{

        const double MIN_CELL_SIZE  = ( 1.0/64.0 * PTS_PER_INCH );

}


namespace glbarcode
{

        /*
         * Barcode2dBase private data
         */
        struct Barcode2dBase::PrivateData {
                int dummy;
        };


        Barcode2dBase::Barcode2dBase()
        {
                d = new Barcode2dBase::PrivateData;
        }


        Barcode2dBase::~Barcode2dBase()
        {
                delete d;
        }


        Barcode& Barcode2dBase::build( const std::string& rawData,
                                       double             w,
                                       double             h )
        {
                std::string  cookedData;   /* Preprocessed data */
                Matrix<bool> encodedData;  /* Encoded data matrix */

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
                                encode( cookedData, encodedData );

                                vectorize( encodedData, w, h );

                                setWidth( w );
                                setHeight( h );
                        }
                }

                return *this;
        }


        /*
         * Default preprocess method
         */
        std::string Barcode2dBase::preprocess( const std::string& rawData )
        {
                return rawData;
        }


        /*
         * Default 2D vectorization method
         */
        void Barcode2dBase::vectorize( const Matrix<bool>& encodedData,
                                       double&             w,
                                       double&             h )
        {

                /* determine size and establish scale */
                double scale;
                double minW = MIN_CELL_SIZE*encodedData.nx() + 2*MIN_CELL_SIZE;
                double minH = MIN_CELL_SIZE*encodedData.ny() + 2*MIN_CELL_SIZE;

                if ( (w <= minW) || (h <= minH) )
                {
                        scale = 1;
                        w     = minW;
                        h     = minH;
                }
                else
                {
                        scale = std::min( w / minW, h / minH );
                        w     = scale * minW;
                        h     = scale * minH;
                }
                double cellSize  = scale * MIN_CELL_SIZE;
                double quietSize = scale * MIN_CELL_SIZE;


                for ( int iy = 0; iy < encodedData.ny(); iy++ )
                {
                        for ( int ix = 0; ix < encodedData.nx(); ix++ )
                        {
                                if ( encodedData[iy][ix] )
                                {
                                        addBox( quietSize + ix*cellSize,
                                                quietSize + iy*cellSize,
                                                cellSize,
                                                cellSize );
                                }
                        }
                }

        }


}
