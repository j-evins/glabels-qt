//  BarcodePostnet.cpp
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


#include "BarcodePostnet.hpp"

#include "Constants.hpp"

#include <cctype>
#include <algorithm>


using namespace glbarcode::Constants;


namespace
{
        /*
         * Encoding symbology
         */
        const std::string symbols[] = {
                /* 0 */ "11000",
                /* 1 */ "00011",
                /* 2 */ "00101",
                /* 3 */ "00110",
                /* 4 */ "01001",
                /* 5 */ "01010",
                /* 6 */ "01100",
                /* 7 */ "10001",
                /* 8 */ "10010",
                /* 9 */ "10100"
        };

        const std::string frameSymbol = "1";


        /*
         * Constants
         */
        const double POSTNET_BAR_WIDTH      = ( 0.02    * PTS_PER_INCH );
        const double POSTNET_FULLBAR_HEIGHT = ( 0.125   * PTS_PER_INCH );
        const double POSTNET_HALFBAR_HEIGHT = ( 0.05    * PTS_PER_INCH );
        const double POSTNET_BAR_PITCH      = ( 0.04545 * PTS_PER_INCH );
        const double POSTNET_HORIZ_MARGIN   = ( 0.125   * PTS_PER_INCH );
        const double POSTNET_VERT_MARGIN    = ( 0.04    * PTS_PER_INCH );

}


namespace glbarcode
{

        /*
         * Static Postnet barcode creation method
         */
        Barcode* BarcodePostnet::create( )
        {
                return new BarcodePostnet();
        }


        /*
         * Postnet validate number of digits
         */
        bool BarcodePostnet::validateDigits( int nDigits )
        {
                /* Accept any valid USPS POSTNET length. */
                return (nDigits == 5) || (nDigits == 9) || (nDigits == 11);
        }


        /*
         * Postnet data validation, implements Barcode1dBase::validate()
         */
        bool BarcodePostnet::validate( const std::string& rawData )
        {
                int nDigits = 0;
                for (char c : rawData)
                {
                        if ( isdigit( c ) )
                        {
                                nDigits++;
                        }
                        else if ( (c != '-') && (c != ' ') )
                        {
                                /* Only allow digits, dashes, and spaces. */
                                return false;
                        }
                }

                return validateDigits( nDigits );
        }


        /*
         * Postnet data encoding, implements Barcode1dBase::encode()
         */
        std::string BarcodePostnet::encode( const std::string& cookedData )
        {
                std::string code;

                /* Left frame bar */
                code += frameSymbol;

                /* process each digit, adding appropriate symbol */
                int sum = 0;
                for (char c : cookedData)
                {
                        if ( isdigit( c ) )
                        {
                                /* Only translate the digits (0-9) */
                                int d = c - '0';
                                code += symbols[d];
                                sum += d;
                        }
                }

                /* Create mandatory correction character */
                code += symbols[ (10 - (sum % 10)) % 10 ];

                /* Right frame bar */
                code += frameSymbol;

                return code;
        }


        /*
         * Postnet vectorization, implements Barcode1dBase::vectorize()
         */
        void BarcodePostnet::vectorize( const std::string& codedData,
                                        const std::string& displayText,
                                        const std::string& cookedData,
                                        double&            w,
                                        double&            h )
        {
                double x = POSTNET_HORIZ_MARGIN;
                for (char c : codedData)
                {
                        double y = POSTNET_VERT_MARGIN;

                        double length = 0;
                        switch ( c )
                        {
                        case '0':
                                y += POSTNET_FULLBAR_HEIGHT - POSTNET_HALFBAR_HEIGHT;
                                length = POSTNET_HALFBAR_HEIGHT;
                                break;
                        case '1':
                                length = POSTNET_FULLBAR_HEIGHT;
                                break;
                        default:
                                // Not reached
                                break;
                        }

                        double width = POSTNET_BAR_WIDTH;

                        addLine( x, y, width, length );

                        x += POSTNET_BAR_PITCH;
                }

                /* Overwrite requested size with actual size. */
                w = x + POSTNET_HORIZ_MARGIN;
                h = POSTNET_FULLBAR_HEIGHT + 2 * POSTNET_VERT_MARGIN;
        }

}
