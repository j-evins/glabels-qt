//  BarcodeUpcBase.cpp
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


#include "BarcodeUpcBase.hpp"

#include "Constants.hpp"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>


using namespace glbarcode::Constants;


namespace
{
        /*
         * Symbology
         */
        const std::string symbols[10][2] = {
                /*          Odd     Even  */
                /*    Left: sBsB    sBsB  */
                /*   Right: BsBs    ----  */
                /*                        */
                /* 0 */  { "3211", "1123" },
                /* 1 */  { "2221", "1222" },
                /* 2 */  { "2122", "2212" },
                /* 3 */  { "1411", "1141" },
                /* 4 */  { "1132", "2311" },
                /* 5 */  { "1231", "1321" },
                /* 6 */  { "1114", "4111" },
                /* 7 */  { "1312", "2131" },
                /* 8 */  { "1213", "3121" },
                /* 9 */  { "3112", "2113" }
        };

        const std::string sSymbol = "111";   /* BsB */
        const std::string eSymbol = "111";   /* BsB */
        const std::string mSymbol = "11111"; /* sBsBs */


        /*
         * Parity selection
         */
        enum Parity { P_ODD, P_EVEN };

        const Parity parity[10][6] = {
                /*                Pos 1,  Pos 2,  Pos 3,  Pos 4,  Pos 5,  Pos 6 */
                /* 0 (UPC-A) */ { P_ODD,  P_ODD,  P_ODD,  P_ODD,  P_ODD,  P_ODD  },
                /* 1         */ { P_ODD,  P_ODD,  P_EVEN, P_ODD,  P_EVEN, P_EVEN },
                /* 2         */ { P_ODD,  P_ODD,  P_EVEN, P_EVEN, P_ODD,  P_EVEN },
                /* 3         */ { P_ODD,  P_ODD,  P_EVEN, P_EVEN, P_EVEN, P_ODD  },
                /* 4         */ { P_ODD,  P_EVEN, P_ODD,  P_ODD,  P_EVEN, P_EVEN },
                /* 5         */ { P_ODD,  P_EVEN, P_EVEN, P_ODD,  P_ODD,  P_EVEN },
                /* 6         */ { P_ODD,  P_EVEN, P_EVEN, P_EVEN, P_ODD,  P_ODD  },
                /* 7         */ { P_ODD,  P_EVEN, P_ODD,  P_EVEN, P_ODD,  P_EVEN },
                /* 8         */ { P_ODD,  P_EVEN, P_ODD,  P_EVEN, P_EVEN, P_ODD  },
                /* 9         */ { P_ODD,  P_EVEN, P_EVEN, P_ODD,  P_EVEN, P_ODD  }
        };


        /*
         * Constants
         */
        const int    QUIET_MODULES   = 9;

        const double BASE_MODULE_SIZE      = ( 0.01 *  PTS_PER_INCH );
        const double BASE_FONT_SIZE        = 7;
        const double BASE_TEXT_AREA_HEIGHT = 11;
}


namespace glbarcode
{

        /*
         * UPC data validation, implements Barcode1dBase::validate()
         */
        bool BarcodeUpcBase::validate( const std::string& rawData )
        {
                int nDigits = 0;

                for (char c : rawData)
                {
                        if ( isdigit( c ) )
                        {
                                nDigits++;
                        }
                        else if ( c != ' ')
                        {
                                /* Only allow digits and spaces -- ignoring spaces. */
                                return false;
                        }
                }

                /* validate nDigits (call implementation from concrete class) */
                return validateDigits( nDigits );
        }


        /*
         * UPC data encoding, implements Barcode1dBase::encode()
         */
        std::string BarcodeUpcBase::encode( const std::string& cookedData )
        {
                int sumOdd  = 0;
                int sumEven = mFirstDigitVal;

                std::string code;

                /* Left frame symbol */
                code += sSymbol;

                /* Left 6 digits */
                for ( int i = 0; i < 6; i++ )
                {
                        int cValue = cookedData[i] - '0';
                        code += symbols[ cValue ][ parity[ mFirstDigitVal ][ i ] ];

                        if ( (i & 1) == 0 )
                        {
                                sumOdd += cValue;
                        }
                        else
                        {
                                sumEven += cValue;
                        }
                }

                /* Middle frame symbol */
                code += mSymbol;

                /* Right 5 digits */
                for ( int i = 6; i < 11; i++ )
                {
                        int cValue = cookedData[i] - '0';
                        code += symbols[cValue][P_ODD];

                        if ( (i & 1) == 0 )
                        {
                                sumOdd += cValue;
                        }
                        else
                        {
                                sumEven += cValue;
                        }
                }

                /* Check digit */
                mCheckDigitVal = (3*sumOdd + sumEven) % 10;
                if ( mCheckDigitVal != 0 )
                {
                        mCheckDigitVal = 10 - mCheckDigitVal;
                }
                code += symbols[mCheckDigitVal][P_ODD];

                /* Right frame symbol */
                code += eSymbol;

                /* Append a final zero length space to make the length of the encoded string even. */
                /* This is because vectorize() handles bars and spaces in pairs. */
                code += "0";

                return code;
        }



        /*
         * UPC prepare text for display, implements Barcode1dBase::prepareText()
         */
        std::string BarcodeUpcBase::prepareText( const std::string& rawData )
        {
                std::string displayText;

                if ( showText() )
                {
                        for (char c : rawData)
                        {
                                if ( isdigit( c ) )
                                {
                                        displayText += c;
                                }
                        }

                        displayText += (mCheckDigitVal + '0');
                }

                return displayText;
        }


        /*
         * UPC vectorization, implements Barcode1dBase::vectorize()
         */
        void BarcodeUpcBase::vectorize( const std::string& codedData,
                                        const std::string& displayText,
                                        const std::string& cookedData,
                                        double&            w,
                                        double&            h )
        {
                /* determine width and establish horizontal scale */
                int nModules     = 7*int(cookedData.size()+1) + 11;

                double scale;
                if ( w == 0 )
                {
                        scale = 1.0;
                }
                else
                {
                        scale = w / ((nModules + 2*QUIET_MODULES) * BASE_MODULE_SIZE);

                        if ( scale < 1.0 )
                        {
                                scale = 1.0;
                        }
                }
                double mscale     = scale * BASE_MODULE_SIZE;

                double width      = mscale * (nModules + 2*QUIET_MODULES);
                double xQuiet     = mscale * QUIET_MODULES;

                /* determine bar height */
                double hTextArea   = showText() ? scale * BASE_TEXT_AREA_HEIGHT : 0;
                double hBar1       = std::max( (h - hTextArea), width/2 );
                double hBar2       = hBar1 + hTextArea/2;

                /* now traverse the code string and draw each bar */
                auto nBarsSpaces = int( codedData.size() - 1 ); /* coded data has dummy "0" on end. */

                double xModules = 0;
                for ( int i = 0; i < nBarsSpaces; i += 2 )
                {
                        double hBar;

                        if ( ( (xModules > mEndBarsModules) && (xModules < (nModules/2-1))               ) ||
                             ( (xModules > (nModules/2+1)) && (xModules < (nModules-mEndBarsModules)) ) )
                        {
                                hBar = hBar1;
                        }
                        else
                        {
                                hBar = hBar2;
                        }

                        /* Bar */
                        int wBar = codedData[i] - '0';
                        addLine( xQuiet + mscale*xModules, 0.0, mscale*wBar, hBar );
                        xModules += wBar;

                        /* Space */
                        int wSpace = codedData[i+1] - '0';
                        xModules += wSpace;
                }


                /* draw text */
                if ( showText() )
                {
                        /* determine text parameters */
                        double textSize1   = scale * BASE_FONT_SIZE;
                        double textSize2   = 0.75*textSize1;

                        double textX1Left  = xQuiet + mscale*(0.25*nModules + 0.5*mEndBarsModules - 0.75);
                        double textX1Right = xQuiet + mscale*(0.75*nModules - 0.5*mEndBarsModules + 0.75);
                        double textX2Left  = 0.5*xQuiet;
                        double textX2Right = 1.5*xQuiet + mscale*nModules;

                        double textY1      = hBar2 + textSize1/4;
                        double textY2      = hBar2 + textSize2/4;

                        /* draw text (call implementation from concrete class) */
                        vectorizeText( displayText,
                                       textSize1, textSize2,
                                       textX1Left, textX1Right, textY1,
                                       textX2Left, textX2Right, textY2 );
                }


                /* Overwrite requested size with actual size. */
                w = width;
                h = hBar1 + hTextArea;
        }

}
