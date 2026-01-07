//  BarcodeCode39.cpp
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


#include "BarcodeCode39.hpp"

#include "Constants.hpp"

#include <cctype>
#include <algorithm>


using namespace glbarcode::Constants;


namespace
{
        /* Code 39 alphabet. Position indicates value. */
        const std::string alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-. $/+%";

        /* Code 39 symbols. Position must match position in alphabet. */
        const std::string symbols[] = {
                /*        BsBsBsBsB */
                /* 0 */  "NnNwWnWnN",
                /* 1 */  "WnNwNnNnW",
                /* 2 */  "NnWwNnNnW",
                /* 3 */  "WnWwNnNnN",
                /* 4 */  "NnNwWnNnW",
                /* 5 */  "WnNwWnNnN",
                /* 6 */  "NnWwWnNnN",
                /* 7 */  "NnNwNnWnW",
                /* 8 */  "WnNwNnWnN",
                /* 9 */  "NnWwNnWnN",
                /* A */  "WnNnNwNnW",
                /* B */  "NnWnNwNnW",
                /* C */  "WnWnNwNnN",
                /* D */  "NnNnWwNnW",
                /* E */  "WnNnWwNnN",
                /* F */  "NnWnWwNnN",
                /* G */  "NnNnNwWnW",
                /* H */  "WnNnNwWnN",
                /* I */  "NnWnNwWnN",
                /* J */  "NnNnWwWnN",
                /* K */  "WnNnNnNwW",
                /* L */  "NnWnNnNwW",
                /* M */  "WnWnNnNwN",
                /* N */  "NnNnWnNwW",
                /* O */  "WnNnWnNwN",
                /* P */  "NnWnWnNwN",
                /* Q */  "NnNnNnWwW",
                /* R */  "WnNnNnWwN",
                /* S */  "NnWnNnWwN",
                /* T */  "NnNnWnWwN",
                /* U */  "WwNnNnNnW",
                /* V */  "NwWnNnNnW",
                /* W */  "WwWnNnNnN",
                /* X */  "NwNnWnNnW",
                /* Y */  "WwNnWnNnN",
                /* Z */  "NwWnWnNnN",
                /* - */  "NwNnNnWnW",
                /* . */  "WwNnNnWnN",
                /*   */  "NwWnNnWnN",
                /* $ */  "NwNwNwNnN",
                /* / */  "NwNwNnNwN",
                /* + */  "NwNnNwNwN",
                /* % */  "NnNwNwNwN"
        };

        const std::string frameSymbol = "NwNnWnWnN";

        /* Vectorization constants */
        const double MIN_X       = ( 0.0075 *  PTS_PER_INCH );
        const double N           = 2.5;
        const double MIN_I       = MIN_X;
        const double MIN_HEIGHT  = ( 0.19685 *  PTS_PER_INCH );
        const double MIN_QUIET   = ( 10 * MIN_X );

        const double MIN_TEXT_AREA_HEIGHT = 12.0;
        const double MIN_TEXT_SIZE        = 8.0;

}


namespace glbarcode
{

        /*
         * Static Code39 barcode creation method
         */
        Barcode* BarcodeCode39::create( )
        {
                return new BarcodeCode39();
        }


        /*
         * Code39 data validation, implements Barcode1dBase::validate()
         */
        bool BarcodeCode39::validate( const std::string& rawData )
        {
                for (char r : rawData)
                {
                        char c = toupper( r );

                        if ( alphabet.find(c) == std::string::npos )
                        {
                                return false;
                        }
                }

                return true;
        }


        /*
         * Code39 data encoding, implements Barcode1dBase::encode()
         */
        std::string BarcodeCode39::encode( const std::string& cookedData )
        {
                std::string code;

                /* Left frame symbol */
                code += frameSymbol;
                code += "i";

                int sum = 0;
                for (char c : cookedData)
                {
                        size_t cValue = alphabet.find( toupper( c ) );

                        code += symbols[cValue];
                        code += "i";

                        sum += int(cValue);
                }

                if ( checksum() )
                {
                        code += symbols[sum % 43];
                        code += "i";
                }

                /* Right frame bar */
                code += frameSymbol;

                return code;
        }


        /*
         * Code39 prepare text for display, implements Barcode1dBase::prepareText()
         */
        std::string BarcodeCode39::prepareText( const std::string& rawData )
        {
                std::string displayText;

                for (char c : rawData)
                {
                        displayText += toupper( c );
                }

                return displayText;
        }


        /*
         * Code39 vectorization, implements Barcode1dBase::vectorize()
         */
        void BarcodeCode39::vectorize( const std::string& codedData,
                                       const std::string& displayText,
                                       const std::string& cookedData,
                                       double&            w,
                                       double&            h )
        {

                /* determine width and establish horizontal scale, based on original cooked data */
                auto dataSize = double( cookedData.size() );
                double minL;
                if ( !checksum() )
                {
                        minL = (dataSize + 2)*(3*N + 6)*MIN_X + (dataSize + 1)*MIN_I;
                }
                else
                {
                        minL = (dataSize + 3)*(3*N + 6)*MIN_X + (dataSize + 2)*MIN_I;
                }

                double scale;
                if ( w == 0 )
                {
                        scale = 1.0;
                }
                else
                {
                        scale = w / (minL + 2*MIN_QUIET);

                        if ( scale < 1.0 )
                        {
                                scale = 1.0;
                        }
                }
                double width = minL * scale;

                /* determine text parameters */
                double hTextArea = scale * MIN_TEXT_AREA_HEIGHT;
                double textSize   = scale * MIN_TEXT_SIZE;

                /* determine height of barcode */
                double height = showText() ? h - hTextArea : h;
                height = std::max( height, std::max( 0.15*width, MIN_HEIGHT ) );

                /* determine horizontal quiet zone */
                double xQuiet = std::max( (10 * scale * MIN_X), MIN_QUIET );

                /* Now traverse the code string and draw each bar */
                double x1 = xQuiet;
                for (char c : codedData)
                {
                        double lwidth;

                        switch ( c )
                        {

                        case 'i':
                                /* Inter-character gap */
                                x1 += scale * MIN_I;
                                break;

                        case 'N':
                                /* Narrow bar */
                                lwidth = scale*MIN_X;
                                addLine( x1, 0.0, lwidth, height );
                                x1 += scale * MIN_X;
                                break;

                        case 'W':
                                /* Wide bar */
                                lwidth = scale*N*MIN_X;
                                addLine( x1, 0.0, lwidth, height );
                                x1 += scale * N * MIN_X;
                                break;

                        case 'n':
                                /* Narrow space */
                                x1 += scale * MIN_X;
                                break;

                        case 'w':
                                /* Wide space */
                                x1 += scale * N * MIN_X;
                                break;

                        default:
                                // NOT REACHED
                                break;
                        }
                }

                if ( showText() )
                {
                        std::string starredText = "*" + displayText + "*";
                        addText( xQuiet + width/2, height + (hTextArea+0.7*textSize)/2, textSize, starredText );
                }

                /* Overwrite requested size with actual size. */
                w = width + 2*xQuiet;
                h = showText() ? height + hTextArea : height;

        }

}
