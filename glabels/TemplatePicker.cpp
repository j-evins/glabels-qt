//  TemplatePicker.cpp
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


#include "TemplatePicker.hpp"

#include "MiniPreviewPixmap.hpp"
#include "TemplatePickerItem.hpp"

#include "model/Settings.hpp"

#include <QAbstractTextDocumentLayout>
#include <QApplication>
#include <QIcon>
#include <QPainter>
#include <QStyledItemDelegate>

#include <algorithm>


namespace
{

        //
        // Custom item delegate to render text as HTML in List View
        //
        // Based on solutions at
        // https://stackoverflow.com/questions/1956542/how-to-make-item-view-render-rich-html-text-in-qt/1956781#1956781
        //
        class HtmlDelegate : public QStyledItemDelegate
        {
        protected:
                void paint ( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const override;
                QSize sizeHint ( const QStyleOptionViewItem& option, const QModelIndex& index ) const override;
        };


        void HtmlDelegate::paint( QPainter*                   painter,
                                  const QStyleOptionViewItem& option,
                                  const QModelIndex&          index) const
        {
                auto opt = option;
                initStyleOption( &opt, index );

                QStyle *style = opt.widget? opt.widget->style() : QApplication::style();

                QTextDocument doc;
                doc.setHtml( opt.text );

                /// Painting everything other than text
                opt.text = QString();
                style->drawControl( QStyle::CE_ItemViewItem, &opt, painter );

                QAbstractTextDocumentLayout::PaintContext ctx;

                // Highlighting text if item is selected
                if ( opt.state & QStyle::State_Selected )
                {
                        ctx.palette.setColor( QPalette::Text, opt.palette.color( QPalette::Active, QPalette::HighlightedText ) );
                }
                else
                {
                        ctx.palette.setColor( QPalette::Text, opt.palette.color( QPalette::Active, QPalette::Text ) );
                }

                QRect textRect = style->subElementRect( QStyle::SE_ItemViewItemText, &opt );
                painter->save();
                painter->translate( textRect.topLeft() );
                painter->setClipRect( textRect.translated( -textRect.topLeft() ) );
                doc.documentLayout()->draw( painter, ctx );
                painter->restore();
        }


        QSize HtmlDelegate::sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const
        {
                auto opt = option;
                initStyleOption( &opt, index );

                QTextDocument doc;
                doc.setHtml( opt.text );
                doc.setTextWidth( opt.rect.width() );
                return QSize( doc.idealWidth(), doc.size().height() );
        }

}


namespace glabels
{

        ///
        /// Constructor
        ///
        TemplatePicker::TemplatePicker( QWidget* parent )
                : QListView(parent)
        {
                mModel = new QStandardItemModel( this );
                setModel( mModel );

                setResizeMode( QListView::Adjust );
                setUniformItemSizes( true );
                setWordWrap( true );
                setIconSize( QSize( TemplatePickerItem::SIZE, TemplatePickerItem::SIZE ) );
                setSpacing( 24 );

                setMode( model::Settings::templatePickerMode() );

                connect( selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),
                         this, SLOT(onSelectionChanged()) );
        }


        ///
        /// Set List of Templates to Pick From
        ///
        void TemplatePicker::setTemplates( const QList<model::Template>& tmplates )
        {
                auto mode = model::Settings::templatePickerMode();
                for ( auto& tmplate : tmplates )
                {
                        mModel->appendRow( new TemplatePickerItem( tmplate, mode ) );
                }
        }


        ///
        /// Set View Mode
        ///
        void TemplatePicker::setMode( QListView::ViewMode mode )
        {
                model::Settings::setTemplatePickerMode( mode );
                setViewMode( mode );

                for ( int i = 0; i < mModel->rowCount(); i++ )
                {
                        if ( auto* tItem = dynamic_cast<TemplatePickerItem *>( mModel->item( i, 0 ) ) )
                        {
                                tItem->setMode( mode );
                        }
                }

                switch ( mode )
                {

                case QListView::IconMode:
                        setItemDelegate( new QStyledItemDelegate() ); // Use default delegate
                        setViewMode( QListView::IconMode );
                        setSpacing( 24 );
                        break;

                case QListView::ListMode:
                        setItemDelegate( new HtmlDelegate() );
                        setViewMode( QListView::ListMode );
                        setSpacing( 8 );
                        break;

                default:
                        qWarning() << "TemplatePicker: unknown mode!";
                        break;

                }

                auto selection = selectedIndexes();
                if ( selection.size() )
                {
                        scrollTo( selection[0], QAbstractItemView::PositionAtCenter );
                }
        }


        ///
        /// Get current View Mode
        ///
        QListView::ViewMode TemplatePicker::mode() const
        {
                return model::Settings::templatePickerMode();
        }


        ///
        /// Apply Filter to Narrow Template Choices by search criteria
        ///
        void TemplatePicker::applyFilter( const QString& searchString,
                                          bool isoMask, bool usMask, bool otherMask,
                                          bool anyCategory, const QStringList& categoryIds )
        {
                for ( int i = 0; i < mModel->rowCount(); i++ )
                {
                        if ( auto* tItem = dynamic_cast<TemplatePickerItem *>( mModel->item( i, 0 ) ) )
                        {
                                bool nameMask = tItem->tmplate().name().contains( searchString, Qt::CaseInsensitive );

                                bool sizeMask =
                                        (isoMask   && tItem->tmplate().isSizeIso())   ||
                                        (usMask    && tItem->tmplate().isSizeUs())    ||
                                        (otherMask && tItem->tmplate().isSizeOther());

                                bool categoryMask;
                                if ( anyCategory )
                                {
                                        categoryMask = true;
                                }
                                else
                                {
                                        categoryMask = false;
                                        for ( QString id : categoryIds )
                                        {
                                                categoryMask = categoryMask || tItem->tmplate().hasCategory( id );
                                        }
                                }


                                if (  nameMask && sizeMask && categoryMask )
                                {
                                        setRowHidden( i, false );
                                }
                                else
                                {
                                        setRowHidden( i, true );
                                        selectionModel()->select( tItem->index(), QItemSelectionModel::Deselect );
                                }
                        }
                }

                auto selection = selectedIndexes();
                if ( selection.size() )
                {
                        scrollTo( selection[0], QAbstractItemView::PositionAtCenter );
                }
        }


        ///
        /// Apply Filter to Narrow Template Choices by a list of names
        ///
        void TemplatePicker::applyFilter( const QStringList& names )
        {
                for ( int i = 0; i < mModel->rowCount(); i++ )
                {
                        if ( auto* tItem = dynamic_cast<TemplatePickerItem *>( mModel->item( i, 0 ) ) )
                        {
                                bool match = false;
                                for ( QString name : names )
                                {
                                        if ( tItem->tmplate().name() == name )
                                        {
                                                match = true;
                                                break;
                                        }
                                }

                                if (  match )
                                {
                                        setRowHidden( i, false );
                                }
                                else
                                {
                                        setRowHidden( i, true );
                                        selectionModel()->select( tItem->index(), QItemSelectionModel::Deselect );
                                }
                        }
                }

                auto selection = selectedIndexes();
                if ( selection.size() )
                {
                        scrollTo( selection[0], QAbstractItemView::PositionAtCenter );
                }
        }


        ///
        /// Get Currently Selected Template
        ///
        model::Template TemplatePicker::selectedTemplate() const
        {
                auto selectionList = selectedIndexes();
                if ( selectionList.size() )
                {
                        int i = selectionList[0].row();
                        if ( auto* tItem = dynamic_cast<TemplatePickerItem *>( mModel->item( i, 0 ) ) )
                        {
                                return tItem->tmplate();
                        }
                }

                return model::Template();
        }


        ///
        /// Handle selection changed from selection model
        ///
        void TemplatePicker::onSelectionChanged()
        {
                emit selectionChanged();
        }


} // namespace glabels
