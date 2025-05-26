/*  TemplatePicker.cpp
 *
 *  Copyright (C) 2013  Jaye Evins <evins@snaught.com>
 *
 *  This file is part of gLabels-qt.
 *
 *  gLabels-qt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  gLabels-qt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "TemplatePicker.h"

#include "TemplatePickerItem.h"

#include "model/Settings.h"

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
	// Note:  assumes that the text rectangle does not need to be resized, so does not reimplement sizeHint().
	//        This delegate does not work correctly in IconMode, and may not work correctly in other applications,
	//        for instance, when the height is not dominated by the icon.
	//
	class HtmlDelegate : public QStyledItemDelegate
	{
	protected:
		void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const override;
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

}


namespace glabels
{

	///
	/// Constructor
	///
	TemplatePicker::TemplatePicker( QWidget* parent ) : QListWidget(parent)
	{
		setResizeMode( QListView::Adjust );
		setUniformItemSizes( true );
		setWordWrap( true );
		setIconSize( QSize( TemplatePickerItem::SIZE, TemplatePickerItem::SIZE ) );

		setMode( model::Settings::templatePickerMode() );
	}


	///
	/// Set List of Templates to Pick From
	///
	void TemplatePicker::setTemplates( const QList<model::Template*>& tmplates )
	{
		auto mode = model::Settings::templatePickerMode();
		
		foreach (model::Template *tmplate, tmplates)
		{
			new TemplatePickerItem( tmplate, mode, this );
		}
	}


	///
	/// Set View Mode
	///
	void TemplatePicker::setMode( QListView::ViewMode mode )
	{
		model::Settings::setTemplatePickerMode( mode );
		
		for ( unsigned int i = 0; i < count(); i++ )
		{
			if (auto* tItem = dynamic_cast<TemplatePickerItem *>(item(i)))
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

		if ( auto* selected = selectedItem() )
		{
			scrollToItem( selected, QAbstractItemView::PositionAtCenter );
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
		for ( unsigned int i = 0; i < count(); i++ )
		{
			if (auto* tItem = dynamic_cast<TemplatePickerItem *>(item(i)))
			{
				bool nameMask = tItem->tmplate()->name().contains( searchString, Qt::CaseInsensitive );
		
				bool sizeMask =
					(isoMask   && tItem->tmplate()->isSizeIso())   ||
					(usMask    && tItem->tmplate()->isSizeUs())    ||
					(otherMask && tItem->tmplate()->isSizeOther());

				bool categoryMask;
				if ( anyCategory )
				{
					categoryMask = true;
				}
				else
				{
					categoryMask = false;
					foreach ( QString id, categoryIds )
					{
						categoryMask = categoryMask || tItem->tmplate()->hasCategory( id );
					}
				}
		

				if (  nameMask && sizeMask && categoryMask )
				{
					tItem->setHidden( false );
				}
				else
				{
					tItem->setHidden( true );
					tItem->setSelected( false );
				}
			}
		}

		if ( auto* selected = selectedItem() )
		{
			scrollToItem( selected, QAbstractItemView::PositionAtCenter );
		}
	}


	///
	/// Apply Filter to Narrow Template Choices by a list of names
	///
	void TemplatePicker::applyFilter( const QStringList& names )
	{
		for ( unsigned int i = 0; i < count(); i++ )
		{
			if (auto *tItem = dynamic_cast<TemplatePickerItem *>(item(i)))
			{
				bool match = false;
				foreach ( QString name, names )
				{
					if ( tItem->tmplate()->name() == name )
					{
						match = true;
						break;
					}
				}

				if (  match )
				{
					tItem->setHidden( false );
				}
				else
				{
					tItem->setHidden( true );
					tItem->setSelected( false );
				}
			}
		}

		if ( auto* selected = selectedItem() )
		{
			scrollToItem( selected, QAbstractItemView::PositionAtCenter );
		}
	}


	///
	/// Get Currently Selected Template
	///
	const model::Template* TemplatePicker::selectedTemplate() const
	{
		if ( auto* tItem = selectedItem() )
		{
			return tItem->tmplate();
		}
		
		return nullptr;
	}


	///
	/// Get Currently Selected Item
	///
	TemplatePickerItem* TemplatePicker::selectedItem() const
	{
		QList<QListWidgetItem*> items = selectedItems();
		if ( !items.isEmpty() )
		{
			return dynamic_cast<TemplatePickerItem*>( items.first() );
		}
		
		return nullptr;
	}


} // namespace glabels
