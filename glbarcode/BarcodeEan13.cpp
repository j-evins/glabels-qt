//  BarcodeEan13.cpp
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


#include "BarcodeEan13.hpp"

#include <cctype>
#include <string>


namespace glbarcode
{

        /*
         * Static EAN-13 barcode creation method
         */
        Barcode* BarcodeEan13::create( )
        {
                return new BarcodeEan13();
        }


        /*
         * EAN-13 barcode constructor
         */
        BarcodeEan13::BarcodeEan13()
        {
                mEndBarsModules = 3;
        }


        /*
         * EAN-13 validate number of digits, implements BarcodeUpcBase::validateDigits()
         */
        bool BarcodeEan13::validateDigits( int nDigits )
        {
                return (nDigits == 12);
        }


        /*
         * EAN-13 Pre-process data before encoding, implements Barcode1dBase::preprocess()
         */
        std::string BarcodeEan13::preprocess( const std::string& rawData )
        {
                std::string cookedData;

                for (char c : rawData)
                {
                        if ( isdigit( c ) )
                        {
                                cookedData += c;
                        }
                }

                mFirstDigitVal = (cookedData[0] - '0');
                return cookedData.substr( 1, cookedData.size()-1 );
        }


        /*
         * EAN-13 vectorize text, implements BarcodeUpcBase::vectorizeText()
         */
        void BarcodeEan13::vectorizeText( const std::string& displayText,
                                          double             size1,
                                          double             size2,
                                          double             x1Left,
                                          double             x1Right,
                                          double             y1,
                                          double             x2Left,
                                          double             x2Right,
                                          double             y2 )
        {
                addText( x2Left,  y2, size2, displayText.substr( 0, 1 ) );
                addText( x1Left,  y1, size1, displayText.substr( 1, 6 ) );
                addText( x1Right, y1, size1, displayText.substr( 7, 6 ) );
        }

}
