//  QrEncode.hpp
//
//  Copyright (C) 2017-2026  Jaye Evins <evins@snaught.com>
//
//  This file is part of gLabels-qt.
//
//  gLabels-qt is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  gLabels-qt is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef barcode_QrEncode_hpp
#define barcode_QrEncode_hpp

#if HAVE_QRENCODE


#include "glbarcode/Barcode2dBase.hpp"


namespace glabels::barcode::QrEncode
{

        ///
        /// QrEncode::QrCode QR Code Barcode
        ///
        /// Implements glbarcode::Barcode2dBase.
        ///
        class QrCode : public glbarcode::Barcode2dBase
        {
        public:
                static Barcode* create();

        private:
                bool validate( const std::string& rawData ) override;

                bool encode( const std::string&       cookedData,
                             glbarcode::Matrix<bool>& encodedData ) override;
        };

}


#endif // HAVE_QRENCODE

#endif // barcode_QrEncode_hpp
