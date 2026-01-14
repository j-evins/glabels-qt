//  BarcodeCepnet.cpp
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


#include "BarcodeCepnet.hpp"


namespace glbarcode
{

        /*
         * Static CEPNET barcode creation method
         */
        Barcode* BarcodeCepnet::create( )
        {
                return new BarcodeCepnet();
        }


        /*
         * CEPNET validation of number of digits, overrides BarcodePostnet::validateDigits()
         */
        bool BarcodeCepnet::validateDigits( int nDigits )
        {
                return nDigits == 8;
        }

}
