//  PageRenderer.cpp
//
//  Copyright (C) 2013-2026  Jaye Evins <evins@snaught.com>
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


#include "PageRenderer.hpp"

#include "Model.hpp"

#include "merge/Merge.hpp"
#include "merge/None.hpp"
#include "merge/Record.hpp"

#include <QDebug>
#include <QImage>
#include <QPainter>


//
// Private
//
namespace
{
        const QColor labelOutlineColor( 0, 0, 0 );
        const double labelOutlineWidth = 0.25;
        const double tickOffset = 2.25;
        const double tickLength = 18;
}


namespace glabels::model
{

        PageRenderer::PageRenderer( const Model* model )
        {
                if ( model )
                {
                        setModel( model );
                }
        }


        void PageRenderer::setModel( const Model* model )
        {
                mModel = model;

                connect( mModel, SIGNAL(changed()), this, SLOT(onModelChanged()) );

                onModelChanged();
        }


        const Model* PageRenderer::model() const
        {
                return mModel;
        }


        void PageRenderer::onModelChanged()
        {
                mMerge = mModel->merge();
                mOrigins = mModel->frame()->getOrigins();
                mNItemsPerPage = mModel->frame()->nLabels();
                mIsMerge = ( dynamic_cast<const merge::None*>(mMerge) == nullptr );
                updateNPages();

                emit changed();
        }


        void PageRenderer::setNCopies( int nCopies )
        {
                mNCopies = nCopies;
                updateNPages();

                emit changed();
        }


        void PageRenderer::setStartItem( int startItem )
        {
                mStartItem = startItem;
                updateNPages();

                emit changed();
        }


        void PageRenderer::setIsCollated( bool isCollated )
        {
                mIsCollated = isCollated;
                updateNPages();

                emit changed();
        }


        void PageRenderer::setAreGroupsContiguous( bool areGroupsContiguous )
        {
                mAreGroupsContiguous = areGroupsContiguous;
                updateNPages();

                emit changed();
        }


        void PageRenderer::setPrintOutlines( bool printOutlinesFlag )
        {
                mPrintOutlines = printOutlinesFlag;

                emit changed();
        }


        void PageRenderer::setPrintCropMarks( bool printCropMarksFlag )
        {
                mPrintCropMarks = printCropMarksFlag;

                emit changed();
        }


        void PageRenderer::setPrintReverse( bool printReverseFlag )
        {
                mPrintReverse = printReverseFlag;

                emit changed();
        }


        void PageRenderer::setIPage( int iPage )
        {
                mIPage = iPage;

                emit changed();
        }


        int PageRenderer::nItems() const
        {
                return mNItems;
        }


        int PageRenderer::nPages() const
        {
                return mNPages;
        }


        QRectF PageRenderer::pageRect() const
        {
                if ( mModel )
                {
                        return QRectF( 0, 0, mModel->tmplate().pageWidth().pt(), mModel->tmplate().pageHeight().pt() );
                }
                else
                {
                        return QRectF( 0, 0, 0, 0 );
                }
        }


        void PageRenderer::updateNPages()
        {
                if ( mModel )
                {
                        if ( !mIsMerge )
                        {
                                // Simple project
                                mLastItem = mStartItem + mNCopies - 1;
                                mNItems = mNCopies;
                                mNGroups = 1;
                                mNItemsPerGroup = mNItems;
                        }
                        else
                        {
                                // Merge project
                                if ( mIsCollated )
                                {
                                        // Collated Merge project
                                        mNItemsPerGroup = mMerge->nSelectedRecords();
                                        mNGroups = mNCopies;
                                }
                                else
                                {
                                        // Un-Collated Merge project
                                        mNItemsPerGroup = mNCopies;
                                        mNGroups = mMerge->nSelectedRecords();
                                }
                                if ( mAreGroupsContiguous )
                                {
                                        // Merge groups are contiguous
                                        mLastItem = mStartItem + mNGroups*mNItemsPerGroup - 1;
                                }
                                else
                                {
                                        // Merge groups start on new page
                                        mNPagesPerGroup = (mStartItem + mNItemsPerGroup - 1)/mNItemsPerPage + 1;
                                        mLastItem = (mNGroups-1)*mNPagesPerGroup*mNItemsPerPage + mStartItem + mNItemsPerGroup - 1;
                                }
                                mNItems = mNGroups*mNItemsPerGroup;
                        }
                }
                else
                {
                        mNPages = 0;
                }

                mNPages = mNItems ? mLastItem/mNItemsPerPage + 1 : 0;
        }


        ///
        /// Print
        ///
        void PageRenderer::print( QPrinter* printer ) const
        {
                QSizeF pageSize( mModel->tmplate().pageWidth().pt(), mModel->tmplate().pageHeight().pt() );
                printer->setPageSize( QPageSize(pageSize, QPageSize::Point) );
                printer->setFullPage( true );
                printer->setPageMargins( QMarginsF(0, 0, 0, 0), QPageLayout::Point );

                QPainter painter( printer );

                QRectF rectPx  = printer->paperRect( QPrinter::DevicePixel );
                QRectF rectPts = printer->paperRect( QPrinter::Point );
                painter.scale( rectPx.width()/rectPts.width(), rectPx.height()/rectPts.height() );


                for ( int iPage = 0; iPage < mNPages; iPage++ )
                {
                        if ( iPage )
                        {
                                printer->newPage();
                        }

                        printPage( &painter, iPage );
                }
        }


        ///
        /// Print page using persistent page number
        ///
        void PageRenderer::printPage( QPainter* painter ) const
        {
                printPage( painter, mIPage );
        }


        ///
        /// Print page
        ///
        void PageRenderer::printPage( QPainter* painter, int iPage ) const
        {
                if ( mModel )
                {
                        if ( !mIsMerge )
                        {
                                printSimplePage( painter, iPage );
                        }
                        else
                        {
                                if ( mIsCollated )
                                {
                                        printCollatedMergePage( painter, iPage );
                                }
                                else
                                {
                                        printUnCollatedMergePage( painter, iPage );
                                }
                        }
                }
        }


        void PageRenderer::printSimplePage( QPainter* painter, int iPage ) const
        {
                printCropMarks( painter );

                int iCopy = 0;
                int iItem = mStartItem;
                int iCurrentPage = 0;

                Variables variables( mModel->constVariables() );

                while ( (iCopy < mNCopies) && (iCurrentPage <= iPage) )
                {
                        if ( iCurrentPage == iPage )
                        {
                                int i = iItem % mNItemsPerPage;

                                painter->save();

                                painter->translate( mOrigins[i].x().pt(), mOrigins[i].y().pt() );

                                painter->save();

                                clipLabel( painter );
                                printLabel( painter, merge::NullRecord(), variables );

                                painter->restore();  // From before clip

                                printOutline( painter );

                                painter->restore();  // From before translation
                        }

                        // Next copy
                        iCopy++;
                        iItem++;
                        iCurrentPage = iItem / mNItemsPerPage;

                        // User variable book keeping
                        variables.incrementVariablesOnItem();
                        variables.incrementVariablesOnCopy();
                        if ( (iItem % mNItemsPerPage) == 0 /* starting a new page */ )
                        {
                                variables.incrementVariablesOnPage();
                        }
                }
        }


        void PageRenderer::printCollatedMergePage( QPainter* painter, int iPage ) const
        {
                printCropMarks( painter );

                int iCopy = 0;
                int iItem = mStartItem;
                int iCurrentPage = 0;

                auto records = mMerge->selectedRecords();
                int iRecord = 0;
                int nRecords = records.size();

                if ( nRecords == 0 )
                {
                        return;
                }

                Variables variables( mModel->constVariables() );

                while ( (iCopy < mNCopies) && (iCurrentPage <= iPage) )
                {
                        if ( iCurrentPage == iPage )
                        {
                                int i = iItem % mNItemsPerPage;

                                painter->save();

                                painter->translate( mOrigins[i].x().pt(), mOrigins[i].y().pt() );

                                painter->save();

                                clipLabel( painter );
                                printLabel( painter, records[iRecord], variables );

                                painter->restore();  // From before clip

                                printOutline( painter );

                                painter->restore();  // From before translation
                        }

                        // Next record
                        iRecord = (iRecord + 1) % nRecords;
                        if ( iRecord == 0 )
                        {
                                iCopy++;
                                if ( mAreGroupsContiguous )
                                {
                                        iItem++;
                                }
                                else
                                {
                                        iItem = iCopy*mNPagesPerGroup*mNItemsPerPage + mStartItem;
                                }
                        }
                        else
                        {
                                iItem++;
                        }
                        iCurrentPage = iItem / mNItemsPerPage;

                        // User variable book keeping
                        variables.incrementVariablesOnItem();
                        if ( iRecord == 0 )
                        {
                                variables.incrementVariablesOnCopy();
                        }
                        if ( (iItem % mNItemsPerPage) == 0 /* starting a new page */ )
                        {
                                variables.incrementVariablesOnPage();
                        }
                }
        }


        void PageRenderer::printUnCollatedMergePage( QPainter* painter, int iPage ) const
        {
                printCropMarks( painter );

                int iCopy = 0;
                int iItem = mStartItem;
                int iCurrentPage = 0;

                auto records = mMerge->selectedRecords();
                int iRecord = 0;
                int nRecords = records.size();

                if ( nRecords == 0 )
                {
                        return;
                }

                Variables variables( mModel->constVariables() );

                while ( (iRecord < nRecords) && (iCurrentPage <= iPage) )
                {
                        if ( iCurrentPage == iPage )
                        {
                                int i = iItem % mNItemsPerPage;

                                painter->save();

                                painter->translate( mOrigins[i].x().pt(), mOrigins[i].y().pt() );

                                painter->save();

                                clipLabel( painter );
                                printLabel( painter, records[iRecord], variables );

                                painter->restore();  // From before clip

                                printOutline( painter );

                                painter->restore();  // From before translation
                        }

                        // Next copy
                        iCopy = (iCopy + 1) % mNCopies;
                        if ( iCopy == 0 )
                        {
                                iRecord++;
                                if ( mAreGroupsContiguous )
                                {
                                        iItem++;
                                }
                                else
                                {
                                        iItem = iRecord*mNPagesPerGroup*mNItemsPerPage + mStartItem;
                                }
                        }
                        else
                        {
                                iItem++;
                        }
                        iCurrentPage = iItem / mNItemsPerPage;

                        // User variable book keeping
                        variables.incrementVariablesOnItem();
                        variables.incrementVariablesOnCopy();
                        if ( iCopy == 0 )
                        {
                                variables.resetOnCopyVariables();
                        }
                        if ( (iItem % mNItemsPerPage) == 0 /* starting a new page */ )
                        {
                                variables.incrementVariablesOnPage();
                        }
                }
        }


        void PageRenderer::printCropMarks( QPainter* painter ) const
        {
                if ( mPrintCropMarks )
                {
                        painter->save();

                        painter->setBrush( QBrush( Qt::NoBrush ) );
                        painter->setPen( QPen( labelOutlineColor, labelOutlineWidth ) );

                        Distance w = mModel->frame()->w();
                        Distance h = mModel->frame()->h();

                        for ( auto& layout : mModel->frame()->layouts() )
                        {
                                Distance xMin = layout.x0();
                                Distance yMin = layout.y0();
                                Distance xMax = layout.x0() + layout.dx()*(layout.nx()-1) + w;
                                Distance yMax = layout.y0() + layout.dy()*(layout.ny()-1) + h;

                                for ( int ix = 0; ix < layout.nx(); ix++ )
                                {
                                        Distance x1 = xMin + ix*layout.dx();
                                        Distance x2 = x1 + w;

                                        Distance y1 = max( yMin-tickOffset, Distance::pt(0) );
                                        Distance y2 = max( y1-tickLength, Distance::pt(0) );

                                        Distance y3 = min( yMax+tickOffset, mModel->tmplate().pageHeight() );
                                        Distance y4 = min( y3+tickLength, mModel->tmplate().pageHeight() );

                                        painter->drawLine( x1.pt(), y1.pt(), x1.pt(), y2.pt() );
                                        painter->drawLine( x2.pt(), y1.pt(), x2.pt(), y2.pt() );
                                        painter->drawLine( x1.pt(), y3.pt(), x1.pt(), y4.pt() );
                                        painter->drawLine( x2.pt(), y3.pt(), x2.pt(), y4.pt() );
                                }

                                for ( int iy = 0; iy < layout.ny(); iy++ )
                                {
                                        Distance y1 = yMin + iy*layout.dy();
                                        Distance y2 = y1 + h;

                                        Distance x1 = max( xMin-tickOffset, Distance::pt(0) );
                                        Distance x2 = max( x1-tickLength, Distance::pt(0) );

                                        Distance x3 = min( xMax+tickOffset, mModel->tmplate().pageWidth() );
                                        Distance x4 = min( x3+tickLength, mModel->tmplate().pageWidth() );

                                        painter->drawLine( x1.pt(), y1.pt(), x2.pt(), y1.pt() );
                                        painter->drawLine( x1.pt(), y2.pt(), x2.pt(), y2.pt() );
                                        painter->drawLine( x3.pt(), y1.pt(), x4.pt(), y1.pt() );
                                        painter->drawLine( x3.pt(), y2.pt(), x4.pt(), y2.pt() );
                                }
                        }

                        painter->restore();
                }
        }


        void PageRenderer::printOutline( QPainter* painter ) const
        {
                if ( mPrintOutlines )
                {
                        painter->save();

                        painter->setBrush( QBrush( Qt::NoBrush ) );
                        painter->setPen( QPen( labelOutlineColor, labelOutlineWidth ) );

                        painter->drawPath( mModel->frame()->path() );

                        painter->restore();
                }
        }


        void PageRenderer::clipLabel( QPainter* painter ) const
        {
                painter->setClipPath( mModel->frame()->clipPath() );
        }


        void PageRenderer::printLabel( QPainter*            painter,
                                       const merge::Record& record,
                                       Variables&           variables ) const
        {
                painter->save();

                if ( mModel->rotate() )
                {
                        painter->rotate( -90.0 );
                        painter->translate( -mModel->w().pt(), 0 );
                }

                if ( mPrintReverse )
                {
                        painter->translate( mModel->w().pt(), 0 );
                        painter->scale( -1, 1 );
                }

                mModel->draw( painter, false, record, variables );

                painter->restore();
        }


        ///
        /// Enumerate all label items as rendered QImages (for tape/direct printers).
        ///
        void PageRenderer::enumerateLabels( double                                    dpi,
                                            const std::function<bool(const QImage&)>& callback ) const
        {
                if ( !mModel )  return;

                const double scale = dpi / 72.0;            // PostScript points → device pixels
                const int    wPx   = qRound( mModel->w().pt() * scale );
                const int    hPx   = qRound( mModel->h().pt() * scale );

                // Render one label into a QImage and invoke the callback.
                // Returns false if the callback requests abort.
                // For P-Touch tape printing the image must be oriented as:
                //   image width  = tape feed direction  (the LONG label dimension)
                //   image height = tape print direction (the SHORT label dimension)
                //
                // "Vertical orientation" labels (portrait, h > w):
                //   Long dim = h → image width,  Short dim = w → image height
                //   Requires axis-swap: model-x → image-y, model-y → image-x
                //
                // "Horizontal orientation" labels (landscape, w >= h):
                //   Long dim = w → image width,  Short dim = h → image height
                //   Plain scale: no axis swap needed
                //
                // QTransform(m11, m12, m21, m22, dx, dy):
                //   x' = m11*x + m21*y + dx
                //   y' = m12*x + m22*y + dy

                const bool swapAxes = (hPx > wPx);
                const int  imgW     = swapAxes ? hPx : wPx;
                const int  imgH     = swapAxes ? wPx : hPx;

                QTransform baseTransform;
                if ( swapAxes )
                {
                        // Axis-swap (vertical/portrait label)
                        // Normal:  model-x→img-y, model-y→img-x
                        // Reverse: mirror tape-feed (model y-axis)
                        baseTransform = mPrintReverse
                                ? QTransform( 0.0, scale, -scale, 0.0, static_cast<double>(hPx), 0.0 )
                                : QTransform( 0.0, scale,  scale, 0.0, 0.0,                      0.0 );
                }
                else
                {
                        // Plain scale (horizontal/landscape label)
                        // Normal:  model-x→img-x, model-y→img-y
                        // Reverse: mirror tape-feed (model x-axis)
                        baseTransform = mPrintReverse
                                ? QTransform( -scale, 0.0, 0.0, scale, static_cast<double>(wPx), 0.0 )
                                : QTransform(  scale, 0.0, 0.0, scale, 0.0,                      0.0 );
                }

                auto renderOne = [&]( const merge::Record& record,
                                      Variables&           variables ) -> bool
                {
                        QImage img( imgW, imgH, QImage::Format_RGB32 );
                        img.fill( Qt::white );

                        QPainter painter( &img );
                        painter.setTransform( baseTransform );

                        mModel->draw( &painter, false, record, variables );
                        painter.end();

                        return callback( img );
                };

                if ( !mIsMerge )
                {
                        // ── Simple (no merge) ──────────────────────────────────────────
                        Variables variables( mModel->constVariables() );

                        for ( int i = 0; i < mNCopies; ++i )
                        {
                                if ( !renderOne( merge::NullRecord(), variables ) )  return;
                                variables.incrementVariablesOnItem();
                                variables.incrementVariablesOnCopy();
                        }
                }
                else if ( mIsCollated )
                {
                        // ── Collated merge: all records × copies ───────────────────────
                        auto records = mMerge->selectedRecords();
                        Variables variables( mModel->constVariables() );

                        for ( int copy = 0; copy < mNCopies; ++copy )
                        {
                                for ( const auto& record : records )
                                {
                                        if ( !renderOne( record, variables ) )  return;
                                        variables.incrementVariablesOnItem();
                                }
                                variables.incrementVariablesOnCopy();
                        }
                }
                else
                {
                        // ── Un-collated merge: each record × copies ────────────────────
                        auto records = mMerge->selectedRecords();
                        Variables variables( mModel->constVariables() );

                        for ( const auto& record : records )
                        {
                                for ( int copy = 0; copy < mNCopies; ++copy )
                                {
                                        if ( !renderOne( record, variables ) )  return;
                                        variables.incrementVariablesOnItem();
                                        variables.incrementVariablesOnCopy();
                                }
                                variables.resetOnCopyVariables();
                        }
                }
        }

}
