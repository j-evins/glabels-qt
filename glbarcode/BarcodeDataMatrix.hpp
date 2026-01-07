//  BarcodeDataMatrix.hpp
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

#ifndef glbarcode_BarcodeDataMatrix_hpp
#define glbarcode_BarcodeDataMatrix_hpp


#include "Barcode2dBase.hpp"


namespace glbarcode
{

        /**
         * @class BarcodeDataMatrix BarcodeDataMatrix.h glbarcode/BarcodeDataMatrix.h
         *
         * DataMatrix barcode, implements Barcode2dBase
         *
         * @image html sample-datamatrix.svg "Sample Data Matrix Barcode"
         *
         */
        class BarcodeDataMatrix : public Barcode2dBase
        {
        public:
                /**
                 * Static DataMatrix barcode creation method
                 *
                 * Used by glbarcode::BarcodeFactory
                 */
                static Barcode* create();


        private:
                bool validate( const std::string& rawData ) override;

                bool encode( const std::string& cookedData,
                             Matrix<bool>&      encodedData ) override;

        };

}


#endif // glbarcode_BarcodeDataMatrix_hpp
