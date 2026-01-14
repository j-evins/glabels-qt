//  QrEncode.cpp
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

#if HAVE_QRENCODE


#include "QrEncode.hpp"

#include <qrencode.h>


namespace glabels::barcode::QrEncode
{

        /*
         * Static QrCode barcode creation method
         */
        glbarcode::Barcode* QrCode::create()
        {
                return new QrCode();
        }


        /*
         * QrCode data validation, implements glbarcode::Barcode2dBase::validate()
         */
        bool QrCode::validate( const std::string& rawData )
        {
                if ( rawData.size() == 0 )
                {
                        return false;
                }
                return true;
        }


        /*
         * QrCode data encoding, implements glbarcode::Barcode2dBase::encode()
         */
        bool QrCode::encode( const std::string& cookedData, glbarcode::Matrix<bool>& encodedData )
        {
                QRcode *qrcode = QRcode_encodeString( cookedData.c_str(), 0, QR_ECLEVEL_M, QR_MODE_8, 1 );
                if ( qrcode == nullptr )
                {
                        return false;
                }


                int w = qrcode->width;
                encodedData.resize( w, w );


                for ( int iy = 0; iy < w; iy++ )
                {
                        for ( int ix = 0; ix < w; ix++ )
                        {
                                encodedData[iy][ix] = qrcode->data[ iy*w + ix ] & 0x01;
                        }
                }


                QRcode_free( qrcode );

                return true;
        }

}


#endif // HAVE_QRENCODE
