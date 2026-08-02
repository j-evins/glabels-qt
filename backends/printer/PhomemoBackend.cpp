//  Printer/PhomemoBackend.cpp
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


#include "PhomemoBackend.hpp"

#include "protocol/PhomemoProtocol.hpp"
#include "transport/BluetoothSerial.hpp"
#include "model/Model.hpp"
#include "model/PageRenderer.hpp"

#include <QPainter>
#include <QTransform>


namespace glabels::printer
{

        ///
        /// Constructor
        ///
        PhomemoBackend::PhomemoBackend( const QString& deviceName,
                                        const QBluetoothAddress& address )
                : mDeviceName( deviceName )
                , mAddress( address )
        {
        }


        ///
        /// Get backend ID (format: "BT:DeviceName:MAC")
        ///
        QString PhomemoBackend::id() const
        {
                return QString("BT:%1:%2").arg(mDeviceName).arg(mAddress.toString());
        }


        ///
        /// Get backend name (display name)
        ///
        QString PhomemoBackend::name() const
        {
                return mDeviceName;
        }


        ///
        /// Print to Phomemo device
        ///
        bool PhomemoBackend::print( model::PageRenderer* renderer )
        {
                // 1. Connect to device
                if ( !connect() )
                {
                        return false;
                }

                // 2. Send initialization sequence
                QByteArray initSeq = protocol::PhomemoProtocol::initSequence();
                if ( !mTransport->write( initSeq ) )
                {
                        mLastError = QString("Failed to send initialization: %1")
                                     .arg( mTransport->lastError() );
                        disconnect();
                        return false;
                }
                mTransport->flush();

                // 3. Render PageRenderer to QImage
                QImage rawImage = renderToImage( renderer );
                if ( rawImage.isNull() )
                {
                        mLastError = "Failed to render label to image";
                        disconnect();
                        return false;
                }

                // 4. Get label width from model to calculate margins
                double labelWidthMm = 12.0;  // Default to 12mm
                const model::Model* model = renderer->model();
                if ( model )
                {
                        const model::Frame* frame = model->tmplate().frame();
                        if ( frame )
                        {
                                labelWidthMm = frame->w().mm();
                        }
                }

                // 5. Add print margins if label is wider than printable area
                QImage marginedImage = addPrintMargins( rawImage, labelWidthMm );
                if ( marginedImage.isNull() )
                {
                        mLastError = "Failed to add print margins";
                        disconnect();
                        return false;
                }

                // 6. Preprocess image for Phomemo (resize, invert, monochrome)
                QImage processedImage = preprocessImage( marginedImage );
                if ( processedImage.isNull() )
                {
                        mLastError = "Failed to preprocess image";
                        disconnect();
                        return false;
                }

                // 5. Encode image to ESC/POS protocol (handles chunking)
                QList<QByteArray> packets = protocol::PhomemoProtocol::encodeImage( processedImage );

                // 6. Send each chunk
                for ( const auto& packet : packets )
                {
                        if ( !mTransport->write( packet ) )
                        {
                                mLastError = QString("Failed to send image data: %1")
                                             .arg( mTransport->lastError() );
                                disconnect();
                                return false;
                        }
                        mTransport->flush();
                }

                // 7. Disconnect
                disconnect();

                mLastError.clear();
                return true;
        }


        ///
        /// Check if device is available (Bluetooth device is paired)
        ///
        bool PhomemoBackend::isAvailable() const
        {
                // For now, assume device is available if we have a valid address
                // Could be improved by checking Bluetooth device list
                return !mAddress.isNull();
        }


        ///
        /// Get last error message
        ///
        QString PhomemoBackend::lastError() const
        {
                return mLastError;
        }


        ///
        /// Connect to Bluetooth device
        ///
        bool PhomemoBackend::connect()
        {
                if ( !mTransport )
                {
                        mTransport.reset( new transport::BluetoothSerial( mAddress ) );
                }

                if ( !mTransport->open() )
                {
                        mLastError = QString("Failed to connect to %1: %2")
                                     .arg(mDeviceName)
                                     .arg(mTransport->lastError());
                        return false;
                }

                return true;
        }


        ///
        /// Disconnect from Bluetooth device
        ///
        void PhomemoBackend::disconnect()
        {
                if ( mTransport )
                {
                        mTransport->close();
                }
        }


        ///
        /// Render PageRenderer output to QImage
        ///
        QImage PhomemoBackend::renderToImage( model::PageRenderer* renderer )
        {
                // Check if renderer has a model
                const model::Model* model = renderer->model();
                if ( !model )
                {
                        qWarning() << "Phomemo: PageRenderer has no model!";
                        return QImage();
                }

                qDebug() << "Phomemo: Renderer has model, checking template...";

                // Get page dimensions from renderer
                QRectF pageRect = renderer->pageRect();

                qDebug() << "Phomemo: Page dimensions:" << pageRect.width() << "x" << pageRect.height() << "points";

                // For continuous-feed printers like Phomemo, if page size is 0,
                // use the label size instead (each label is a "page")
                if ( pageRect.width() <= 0 || pageRect.height() <= 0 )
                {
                        qDebug() << "Phomemo: Page size is 0, using label dimensions instead";
                        qDebug() << "Phomemo: Template - brand:" << model->tmplate().brand()
                                 << "part:" << model->tmplate().part()
                                 << "description:" << model->tmplate().description();

                        // Get label dimensions from the template's frame
                        const model::Frame* frame = model->tmplate().frame();
                        if ( !frame )
                        {
                                qWarning() << "Phomemo: Template has no frame/label defined";
                                return QImage();
                        }

                        pageRect = QRectF( 0, 0, frame->w().pt(), frame->h().pt() );

                        qDebug() << "Phomemo: Using label dimensions:" << pageRect.width() << "x" << pageRect.height() << "points";
                }

                // Validate dimensions
                if ( pageRect.width() <= 0 || pageRect.height() <= 0 )
                {
                        qWarning() << "Phomemo: Still have invalid dimensions after checking label size";
                        return QImage();
                }

                // Create image with sufficient resolution (300 DPI)
                // Convert points to pixels at 300 DPI (1 point = 300/72 pixels)
                double scale = 300.0 / 72.0;
                int imageWidth = qRound( pageRect.width() * scale );
                int imageHeight = qRound( pageRect.height() * scale );

                qDebug() << "Phomemo: Image size:" << imageWidth << "x" << imageHeight << "pixels";

                // Validate calculated dimensions
                if ( imageWidth <= 0 || imageHeight <= 0 )
                {
                        qWarning() << "Phomemo: Calculated image dimensions are invalid:" << imageWidth << "x" << imageHeight;
                        return QImage();
                }

                // Create canvas with ARGB32_Premultiplied format (supports QPainter)
                QImage image( imageWidth, imageHeight, QImage::Format_ARGB32_Premultiplied );

                if ( image.isNull() )
                {
                        qWarning() << "Phomemo: Failed to create QImage";
                        return QImage();
                }

                image.fill( Qt::white );

                qDebug() << "Phomemo: Image created successfully, initializing painter";

                // Render to image
                QPainter painter( &image );
                if ( !painter.isActive() )
                {
                        qWarning() << "Phomemo: Failed to create QPainter for image rendering";
                        qWarning() << "Phomemo: Image format:" << image.format();
                        qWarning() << "Phomemo: Image size:" << image.size();
                        qWarning() << "Phomemo: Image depth:" << image.depth();
                        return QImage();
                }

                qDebug() << "Phomemo: Painter active, scaling and rendering";
                painter.scale( scale, scale );

                // Render the current page
                renderer->printPage( &painter );

                painter.end();

                qDebug() << "Phomemo: Rendering complete, converting format";

                // Convert to RGB32 (remove alpha channel)
                return image.convertToFormat( QImage::Format_RGB32 );
        }


        ///
        /// Crop to printable area for labels wider than 12mm
        /// Phomemo D30/D35 have 96-pixel (12mm) printable width regardless of label size
        /// For 14mm/15mm labels, we crop to the center 12mm portion
        /// The remaining width becomes physical margins on the label
        ///
        QImage PhomemoBackend::addPrintMargins( const QImage& source, double labelWidthMm )
        {
                const double printableWidthMm = protocol::PhomemoProtocol::PRINTABLE_WIDTH_MM;

                // If label width <= printable width, no cropping needed
                if ( labelWidthMm <= printableWidthMm )
                {
                        qDebug() << "Phomemo: Label width" << labelWidthMm << "mm <= printable width"
                                 << printableWidthMm << "mm, no cropping needed";
                        return source;
                }

                qDebug() << "Phomemo: Label width" << labelWidthMm << "mm > printable width"
                         << printableWidthMm << "mm, cropping to center" << printableWidthMm << "mm";

                // Calculate printable width in pixels
                // Scale factor to convert mm to pixels (based on source image)
                double mmToPixels = source.width() / labelWidthMm;
                int printableWidthPx = qRound( printableWidthMm * mmToPixels );

                // Calculate left edge of printable area (centered)
                int leftEdgePx = (source.width() - printableWidthPx) / 2;

                qDebug() << "Phomemo: Cropping - source width:" << source.width() << "px"
                         << "printable width:" << printableWidthPx << "px"
                         << "left edge:" << leftEdgePx << "px";

                // Crop to center printable area
                QImage cropped = source.copy( leftEdgePx, 0, printableWidthPx, source.height() );

                qDebug() << "Phomemo: Cropped image size:" << cropped.width() << "x" << cropped.height();

                return cropped;
        }


        ///
        /// Preprocess image for Phomemo printer
        /// 1. Resize to 96px width, maintain aspect ratio
        /// 2. Invert colors (thermal printer)
        /// 3. Convert to 1-bit monochrome
        ///
        /// Note: No rotation needed - our rendered image is already in portrait orientation (96px wide)
        ///
        QImage PhomemoBackend::preprocessImage( const QImage& source )
        {
                // Step 1: Resize to 96px width, maintaining aspect ratio
                // Calculate height to preserve aspect ratio
                int targetHeight = source.height() * protocol::PhomemoProtocol::IMAGE_WIDTH / source.width();

                // Use IgnoreAspectRatio since we've already calculated the correct height
                // This ensures we get exactly 96 pixels width (not 95)
                QImage resized = source.scaled( protocol::PhomemoProtocol::IMAGE_WIDTH,
                                                targetHeight,
                                                Qt::IgnoreAspectRatio,
                                                Qt::SmoothTransformation );

                qDebug() << "Phomemo: After resize:" << resized.width() << "x" << resized.height();

                // Step 2: Invert colors (thermal printer burns black areas, creating black on paper)
                // After inversion: text becomes white, background becomes black
                // Bit packing will set bit=1 for white pixels, causing thermal head to burn those areas black
                resized.invertPixels();

                // Step 3: Convert to 1-bit monochrome with dithering
                QImage mono = resized.convertToFormat( QImage::Format_Mono,
                                                       Qt::MonoOnly | Qt::ThresholdDither );

                qDebug() << "Phomemo: Final preprocessed image:" << mono.width() << "x" << mono.height();

                return mono;
        }


        ///
        /// Factory create method
        /// Parse ID format: "BT:DeviceName:MAC"
        ///
        Backend* PhomemoBackend::create( const QString& id )
        {
                QStringList parts = id.split( ':' );

                if ( parts.size() < 3 || parts[0] != "BT" )
                {
                        return nullptr;
                }

                QString deviceName = parts[1];
                QString macAddress = parts[2];

                // Handle MAC addresses that may contain colons
                // Format could be: "BT:Phomemo D30:AA:BB:CC:DD:EE:FF"
                if ( parts.size() > 3 )
                {
                        // Reconstruct MAC address from remaining parts
                        macAddress = parts.mid(2).join(':');
                }

                QBluetoothAddress address( macAddress );
                if ( address.isNull() )
                {
                        return nullptr;
                }

                return new PhomemoBackend( deviceName, address );
        }

}
