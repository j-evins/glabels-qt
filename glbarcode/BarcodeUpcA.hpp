//  BarcodeUpcA.hpp
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

#ifndef glbarcode_BarcodeUpcA_hpp
#define glbarcode_BarcodeUpcA_hpp


#include "BarcodeUpcBase.hpp"


namespace glbarcode
{

        /**
         * @class BarcodeUpcA BarcodeUpcA.h glbarcode/BarcodeUpcA.h
         *
         * UPC-A barcode, implements BarcodeUpcBase
         *
         * @image html sample-upc-a.svg "Sample UPC-A Barcode"
         *
         */
        class BarcodeUpcA : public BarcodeUpcBase
        {
        public:
                /**
                 * Static UPC-A barcode creation method
                 *
                 * Used by glbarcode::BarcodeFactory
                 */
                static Barcode* create();


                /**
                 * Default constructor
                 */
                BarcodeUpcA();


        private:
                bool validateDigits( int nDigits ) override;

                std::string preprocess( const std::string& rawData ) override;

                void vectorizeText( const std::string& displayText,
                                    double             size1,
                                    double             size2,
                                    double             x1Left,
                                    double             x1Right,
                                    double             y1,
                                    double             x2Left,
                                    double             x2Right,
                                    double             y2 ) override;

        };

}


#endif // glbarcode_BarcodeUpcA_hpp
