//  Printer/Protocol/PhomemoProtocol.hpp
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

#ifndef printer_protocol_PhomemoProtocol_hpp
#define printer_protocol_PhomemoProtocol_hpp


#include <QByteArray>
#include <QImage>
#include <QList>


namespace glabels::printer::protocol
{

        ///
        /// Phomemo ESC/POS Protocol (Picture Mode)
        ///
        class PhomemoProtocol
        {
                /////////////////////////////////
                // Constants
                /////////////////////////////////
        public:
                static constexpr int IMAGE_WIDTH = 96;            // Fixed width in pixels (thermal head)
                static constexpr double PRINTABLE_WIDTH_MM = 12.0; // Printable width in mm
                static constexpr int MAX_CHUNK_HEIGHT = 255;      // Max height per chunk


                /////////////////////////////////
                // Static Methods
                /////////////////////////////////
        public:
                // Generate initialization sequence (7 packets)
                static QByteArray initSequence();

                // Encode image to ESC/POS picture mode (handles chunking)
                static QList<QByteArray> encodeImage( const QImage& image );


                /////////////////////////////////
                // Private Helper Methods
                /////////////////////////////////
        private:
                // Generate image header with specific height
                static QByteArray imageHeader( int height );

                // Pack image bits (MSB-first, 12 bytes per line)
                static QByteArray packBits( const QImage& mono1bit );

                // Split image into chunks (max 255px height each)
                static QList<QImage> splitIntoChunks( const QImage& image );
        };

}


#endif // printer_protocol_PhomemoProtocol_hpp
