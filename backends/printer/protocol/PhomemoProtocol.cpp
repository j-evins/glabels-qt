//  Printer/Protocol/PhomemoProtocol.cpp
//
//  Copyright (C) 2026  gLabels-qt contributors
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


#include "PhomemoProtocol.hpp"


namespace glabels::printer::protocol
{

        ///
        /// Generate initialization sequence
        /// 7-packet initialization for Phomemo printers
        ///
        QByteArray PhomemoProtocol::initSequence()
        {
                return QByteArray::fromHex("1f1138") +
                       QByteArray::fromHex("1f11121f1113") +
                       QByteArray::fromHex("1f1109") +
                       QByteArray::fromHex("1f1111") +
                       QByteArray::fromHex("1f1119") +
                       QByteArray::fromHex("1f1107") +
                       QByteArray::fromHex("1f110a1f110202");
        }


        ///
        /// Generate image header with specific height
        /// Format: 1f1124001b401d763000WWWWHHHH
        /// Where WWWW is width in bytes (little-endian), HHHH is height in pixels (little-endian)
        ///
        QByteArray PhomemoProtocol::imageHeader( int height )
        {
                // Base header: 1f1124001b401d763000 (10 bytes)
                // ESC/POS GS v 0 command for raster bit image
                QByteArray header = QByteArray::fromHex("1f1124001b401d763000");

                // Width: 0x000C (12 bytes = 96 pixels) - little endian
                header.append( char(0x0C) );
                header.append( char(0x00) );

                // Height: little endian (variable based on chunk)
                header.append( char(height & 0xFF) );
                header.append( char((height >> 8) & 0xFF) );

                return header;
        }


        ///
        /// Pack image bits (MSB-first, 12 bytes per line)
        ///
        QByteArray PhomemoProtocol::packBits( const QImage& mono1bit )
        {
                QByteArray packed;
                int height = mono1bit.height();
                int width = mono1bit.width();

                for ( int y = 0; y < height; y++ )
                {
                        // 96 pixels = 12 bytes per scanline
                        for ( int byteX = 0; byteX < 12; byteX++ )
                        {
                                quint8 byte = 0;

                                for ( int bit = 0; bit < 8; bit++ )
                                {
                                        int x = byteX * 8 + bit;

                                        if ( x < width )
                                        {
                                                // Get pixel value (monochrome image)
                                                QRgb pixel = mono1bit.pixel( x, y );
                                                int gray = qGray( pixel );

                                                // White pixel (>= 128) sets bit to 1
                                                // After inversion, white pixels are the text we want to print
                                                if ( gray >= 128 )
                                                {
                                                        byte |= (1 << (7 - bit));  // MSB first
                                                }
                                        }
                                }

                                packed.append( byte );
                        }
                }

                return packed;
        }


        ///
        /// Split image into chunks (max 255px height each)
        ///
        QList<QImage> PhomemoProtocol::splitIntoChunks( const QImage& image )
        {
                QList<QImage> chunks;
                int height = image.height();
                int numChunks = (height + MAX_CHUNK_HEIGHT - 1) / MAX_CHUNK_HEIGHT;

                for ( int i = 0; i < numChunks; i++ )
                {
                        int yStart = i * MAX_CHUNK_HEIGHT;
                        int chunkHeight = qMin( MAX_CHUNK_HEIGHT, height - yStart );

                        QImage chunk = image.copy( 0, yStart, image.width(), chunkHeight );
                        chunks.append( chunk );
                }

                return chunks;
        }


        ///
        /// Encode image to ESC/POS picture mode
        /// Returns single packet with header (total height) + all data
        ///
        /// IMPORTANT: Send ONE header with total height, then all image data.
        /// Don't send separate headers per chunk!
        ///
        QList<QByteArray> PhomemoProtocol::encodeImage( const QImage& image )
        {
                QList<QByteArray> packets;

                // Convert to monochrome if not already
                QImage mono = image;
                if ( mono.format() != QImage::Format_Mono )
                {
                        mono = mono.convertToFormat( QImage::Format_Mono,
                                                     Qt::MonoOnly | Qt::ThresholdDither );
                }

                // Create single packet with header for TOTAL height + all image data
                QByteArray packet;

                // Add image header with TOTAL height (not chunked)
                packet.append( imageHeader( mono.height() ) );

                // Pack and append ALL image data at once
                packet.append( packBits( mono ) );

                packets.append( packet );

                return packets;
        }

}
