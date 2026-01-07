//  BarcodePostnet.hpp
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

#ifndef glbarcode_BarcodePostnet_hpp
#define glbarcode_BarcodePostnet_hpp


#include "Barcode1dBase.hpp"


namespace glbarcode
{

        /**
         * @class BarcodePostnet BarcodePostnet.h glbarcode/BarcodePostnet.h
         *
         * *POSTNET* barcode (All USPS sizes: ZIP, ZIP+4, ZIP+4+DC).
         *
         * @image html sample-postnet.svg "Sample USPS POSTNET Barcode"
         *
         *
         * ### Input Data Format ###
         *
         * The *POSTNET* specification defines 10 characters, consisting solely
         * of decimal digits (0-9).  The BarcodePostnet validator and encoder will
         * ignore spaces and dashes (-).  The validator will only accept input
         * data with 5 digits (ZIP), 9 digits (ZIP+4) or 11 digits (ZIP+4+DC).
         *
         *
         * ### Checksum Property ###
         *
         * The *checksum* property is ignored.  A mandatory check digit will
         * always be automatically generated and appended to the input data
         * prior to encoding.
         *
         *
         * ### Show Text Property ###
         *
         * The *Show Text* property is ignored.
         *
         *
         * ### References ###
         *
         * - http://en.wikipedia.org/wiki/POSTNET
         *
         */
        class BarcodePostnet : public Barcode1dBase
        {
        public:
                /**
                 * Static Postnet barcode creation method
                 *
                 * Used by glbarcode::BarcodeFactory
                 */
                static Barcode* create();


        protected:
                /**
                 * Validate number of digits
                 */
                virtual bool validateDigits( int nDigits );


        private:
                bool validate( const std::string& rawData ) override;

                std::string encode( const std::string& cookedData ) override;

                void vectorize( const std::string& codedData,
                                const std::string& displayText,
                                const std::string& cookedData,
                                double&            w,
                                double&            h ) override;

        };

}


#endif // glbarcode_BarcodePostnet_hpp
