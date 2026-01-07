//  BarcodePostnet9.hpp
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

#ifndef glbarcode_BarcodePostnet9_hpp
#define glbarcode_BarcodePostnet9_hpp


#include "BarcodePostnet.hpp"


namespace glbarcode
{

        /**
         * @class BarcodePostnet9 BarcodePostnet9.h glbarcode/BarcodePostnet9.h
         *
         * *POSTNET-9* barcode (ZIP+4 only), extends BarcodePostnet
         *
         * @image html sample-postnet-9.svg "Sample 9 digit USPS POSTNET Barcode"
         *
         *
         * ### Input Data Format ###
         *
         * Input data requirements are identical to BarcodePostnet, except the
         * validator only accepts 9 digits (ZIP+4) of input.
         *
         * See BarcodePostnet.
         *
         */
        class BarcodePostnet9 : public BarcodePostnet
        {
        public:
                /**
                 * Static POSTNET-9 barcode creation method
                 *
                 * Used by glbarcode::BarcodeFactory
                 */
                static Barcode* create();


        private:
                bool validateDigits( int nDigits ) override;

        };

}


#endif // glbarcode_BarcodePostnet9_hpp
