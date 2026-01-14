//  BarcodeQrcode.cpp
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

#if HAVE_QRENCODE


#include "BarcodeQrcode.hpp"

#include "qrencode.hpp"


namespace glbarcode
{

        /*
         * Static Qrcode barcode creation method
         */
        Barcode* BarcodeQrcode::create( void )
        {
                return new BarcodeQrcode();
        }


        /*
         * Qrcode data validation, implements Barcode2dBase::validate()
         */
        bool BarcodeQrcode::validate( const std::string& rawData )
        {
                if ( rawData.size() == 0 )
                {
                        return false;
                }
                return true;
        }


        /*
         * Qrcode data encoding, implements Barcode2dBase::encode()
         */
        bool BarcodeQrcode::encode( const std::string& cookedData, Matrix<bool>& encodedData )
        {
                QRcode *qrcode = QRcode_encodeString( cookedData.c_str(), 0, QR_ECLEVEL_M, QR_MODE_8, 1 );
                if ( qrcode == NULL )
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
                QRcode_clearCache();

                return true;
        }


}

#endif // HAVE_QRENCODE
