//  BarcodeCode39.hpp
//
//  Copyright (C) 2013-2026  Jaye Evins <evins@snaught.com>
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

#ifndef glbarcode_BarcodeCode39_hpp
#define glbarcode_BarcodeCode39_hpp


#include "Barcode1dBase.hpp"


namespace glbarcode
{

        /**
         * @class BarcodeCode39 BarcodeCode39.h glbarcode/BarcodeCode39.h
         *
         * *Code 39* 1D barcode symbology.
         *
         * @image html sample-code39.svg "Sample Code 39 Barcode"
         *
         *
         * ### Input Data Format ###
         *
         * The *Code 39* specification defines 43 characters, consisting of upper
         * case letters (A-Z), decimal digits (0-9), space, and several special
         * characters (-.$/+%). The BarcodeCode39 validator will also accept
         * lower case letters (a-z).  The encoder will automatically upshift
         * any lower case letters prior to encoding.
         *
         * For full ASCII support see BarcodeCode39Ext.
         *
         *
         * ### Checksum Property ###
         *
         * If the *checksum* property is *true*, a modulo 43 check digit will be
         * automatically generated and appended to input data before encoding.
         * By default, the check digit will not be generated.
         *
         * See setChecksum().
         *
         *
         * ### Show Text Property ###
         *
         * If the *Show Text* property is *true*, the input data will be printed
         * below the barcode.  By default, the data will not be printed.
         *
         * See setShowText().
         *
         *
         * ### References ###
         *
         * - http://en.wikipedia.org/wiki/Code_39
         *
         */
        class BarcodeCode39 : public Barcode1dBase
        {
        public:
                /**
                 * Static Code39 barcode creation method
                 *
                 * Used by glbarcode::BarcodeFactory
                 */
                static Barcode* create();


        private:
                bool validate( const std::string& rawData ) override;

                std::string encode( const std::string& cookedData ) override;

                std::string prepareText( const std::string& rawData ) override;

                void vectorize( const std::string& codedData,
                                const std::string& displayText,
                                const std::string& cookedData,
                                double&            w,
                                double&            h ) override;
        };

}


#endif // glbarcode_BarcodeCode39_hpp
