//  BarcodePostnet5.hpp
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

#ifndef glbarcode_BarcodePostnet5_hpp
#define glbarcode_BarcodePostnet5_hpp


#include "BarcodePostnet.hpp"


namespace glbarcode
{

        /**
         * @class BarcodePostnet5 BarcodePostnet5.h glbarcode/BarcodePostnet5.h
         *
         * *POSTNET-5* barcode (ZIP only), extends BarcodePostnet
         *
         * @image html sample-postnet-5.svg "Sample 5 digit USPS POSTNET Barcode"
         *
         *
         * ### Input Data Format ###
         *
         * Input data requirements are identical to BarcodePostnet, except the
         * validator only accepts 5 digits (ZIP) of input.
         *
         * See BarcodePostnet.
         *
         */
        class BarcodePostnet5 : public BarcodePostnet
        {
        public:
                /**
                 * Static POSTNET-5 barcode creation method
                 *
                 * Used by glbarcode::BarcodeFactory
                 */
                static Barcode* create();


        private:
                bool validateDigits( int nDigits ) override;

        };

}


#endif // glbarcode_BarcodePostnet5_hpp
