//  BarcodeUpcBase.hpp
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

#ifndef glbarcode_BarcodeUpcBase_hpp
#define glbarcode_BarcodeUpcBase_hpp


#include "Barcode1dBase.hpp"


namespace glbarcode
{

        /**
         * @class BarcodeUpcBase BarcodeUpcBase.h glbarcode/BarcodeUpcBase.h
         *
         * UpcBase barcode, base class for UPC-A and EAN-13 barcode types, implements Barcode1dBase
         */
        class BarcodeUpcBase : public Barcode1dBase
        {
        protected:
                virtual bool validateDigits( int nDigits ) = 0;

                virtual void vectorizeText( const std::string& displayText,
                                            double             size1,
                                            double             size2,
                                            double             x1Left,
                                            double             x1Right,
                                            double             y1,
                                            double             x2Left,
                                            double             x2Right,
                                            double             y2 ) = 0;

        private:
                bool validate( const std::string& rawData ) override;

                std::string encode( const std::string& cookedData ) override;

                std::string prepareText( const std::string& rawData ) override;

                void vectorize( const std::string& codedData,
                                const std::string& displayText,
                                const std::string& cookedData,
                                double&            w,
                                double&            h ) override;


        protected:
                int mEndBarsModules;
                int mFirstDigitVal;


        private:
                int mCheckDigitVal;

        };

}


#endif // glbarcode_BarcodeUpcBase_hpp
