//  BarcodeCode39Ext.hpp
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

#ifndef glbarcode_BarcodeCode39Ext_hpp
#define glbarcode_BarcodeCode39Ext_hpp


#include "BarcodeCode39.hpp"


namespace glbarcode
{

        /**
         * @class BarcodeCode39Ext BarcodeCode39Ext.h glbarcode/BarcodeCode39Ext.h
         *
         * Extended *Code 39* 1D barcode symbology.
         *
         * @image html sample-code39ext.svg "Sample Extended Code 39 Barcode"
         *
         *
         * ### Input Data Format ###
         *
         * The extended *Code 39* barcode type supports the full 7-bit ASCII
         * character set (0-127).  This support is accomplished by encoding
         * lower case letters, constrol characters, and special characters
         * other than "-" and "." as two character sequences as illustrated
         * in the following table:
         *
         * | Dec | Char | Enc || Dec | Char | Enc || Dec | Char | Enc || Dec | Char | Enc |
         * |----:|:----:|:---:||----:|:----:|:---:||----:|:----:|:---:||----:|:----:|:---:|
         * |   0 |  NUL | %%U ||  32 | \" \"|\" \"||  64 |   @  | %%V ||  96 |   `  | %%W |
         * |   1 |  SOH |  $A ||  33 |   !  |  /A ||  65 |   A  |  A  ||  97 |   a  |  +A |
         * |   2 |  STX |  $B ||  34 |  \"  |  /B ||  66 |   B  |  B  ||  98 |   b  |  +B |
         * |   3 |  ETX |  $C ||  35 |   #  |  /C ||  67 |   C  |  C  ||  99 |   c  |  +C |
         * |   4 |  EOT |  $D ||  36 |   $  |  /D ||  68 |   D  |  D  || 100 |   d  |  +D |
         * |   5 |  ENQ |  $E ||  37 |   %  |  /E ||  69 |   E  |  E  || 101 |   e  |  +E |
         * |   6 |  ACK |  $F ||  38 |   &  |  /F ||  70 |   F  |  F  || 102 |   f  |  +F |
         * |   7 |  BEL |  $G ||  39 |   '  |  /G ||  71 |   G  |  G  || 103 |   g  |  +G |
         * |   8 |  BS  |  $H ||  40 |   (  |  /H ||  72 |   H  |  H  || 104 |   h  |  +H |
         * |   9 |  HT  |  $I ||  41 |   )  |  /I ||  73 |   I  |  I  || 105 |   i  |  +I |
         * |  10 |  LF  |  $J ||  42 |   *  |  /J ||  74 |   J  |  J  || 106 |   j  |  +J |
         * |  11 |  VT  |  $K ||  43 |   +  |  /K ||  75 |   K  |  K  || 107 |   k  |  +K |
         * |  12 |  FF  |  $L ||  44 |   ,  |  /L ||  76 |   L  |  L  || 108 |   l  |  +L |
         * |  13 |  CR  |  $M ||  45 |   -  |  -  ||  77 |   M  |  M  || 109 |   m  |  +M |
         * |  14 |  SO  |  $N ||  46 |   .  |  .  ||  78 |   N  |  N  || 110 |   n  |  +N |
         * |  15 |  SI  |  $O ||  47 |   /  |  /O ||  79 |   O  |  O  || 111 |   o  |  +O |
         * |  16 |  DLE |  $P ||  48 |   0  |  0  ||  80 |   P  |  P  || 112 |   p  |  +P |
         * |  17 |  DC1 |  $Q ||  49 |   1  |  1  ||  81 |   Q  |  Q  || 113 |   q  |  +Q |
         * |  18 |  DC2 |  $R ||  50 |   2  |  2  ||  82 |   R  |  R  || 114 |   r  |  +R |
         * |  19 |  DC3 |  $S ||  51 |   3  |  3  ||  83 |   S  |  S  || 115 |   s  |  +S |
         * |  20 |  DC4 |  $T ||  52 |   4  |  4  ||  84 |   T  |  T  || 116 |   t  |  +T |
         * |  21 |  NAK |  $U ||  53 |   5  |  5  ||  85 |   U  |  U  || 117 |   u  |  +U |
         * |  22 |  SYN |  $V ||  54 |   6  |  6  ||  86 |   V  |  V  || 118 |   v  |  +V |
         * |  23 |  ETB |  $W ||  55 |   7  |  7  ||  87 |   W  |  W  || 119 |   w  |  +W |
         * |  24 |  CAN |  $X ||  56 |   8  |  8  ||  88 |   X  |  X  || 120 |   x  |  +X |
         * |  25 |  EM  |  $Y ||  57 |   9  |  9  ||  89 |   Y  |  Y  || 121 |   y  |  +Y |
         * |  26 |  SUB |  $Z ||  58 |   :  |  /Z ||  90 |   Z  |  Z  || 122 |   z  |  +Z |
         * |  27 |  ESC | %%A ||  59 |   ;  | %%F ||  91 |   [  | %%K || 123 |   {  | %%P |
         * |  28 |  FS  | %%B ||  60 |   <  | %%G ||  92 |   \\ | %%L || 124 |   \| | %%Q |
         * |  29 |  GS  | %%C ||  61 |   =  | %%H ||  93 |   ]  | %%M || 125 |   }  | %%R |
         * |  30 |  RS  | %%D ||  62 |   >  | %%I ||  94 |   ^  | %%N || 126 |   ~  | %%S |
         * |  31 |  US  | %%E ||  63 |   ?  | %%J ||  95 |   _  | %%O || 127 |  DEL | %%T |
         *
         * This pre-encoded data is now encoded using the standard *Code 39* symbology.
         *
         *
         * ### Checksum Property ###
         *
         * If the *checksum* property is *true*, a modulo 43 check digit will be
         * automatically generated and appended to the 1st pass encoded data
         * prior to the 2nd pass *Code 39* encoding. By default, the check digit
         * will not be generated.
         *
         * See setChecksum().
         *
         *
         * ### Show Text Property ###
         *
         * If the *Show Text* property is *true*, the original ASCII input data will
         * be printed below the barcode.  By default, the data will not be printed.
         *
         * See setShowText().
         *
         *
         * ### References ###
         *
         * - http://en.wikipedia.org/wiki/Code_39
         *
         *
         */
        class BarcodeCode39Ext : public BarcodeCode39
        {
        public:
                /**
                 * Static Extended Code39 barcode creation method
                 *
                 * Used by glbarcode::BarcodeFactory
                 */
                static Barcode* create();


        private:
                bool validate( const std::string& rawData ) override;

                std::string preprocess( const std::string& rawData ) override;

                std::string prepareText( const std::string& rawData ) override;

        };

}


#endif // glbarcode_BarcodeCode39Ext_hpp
