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

#include <QIcon>


namespace glabels
{

	///
	/// Constructor
	///
	TemplatePicker::TemplatePicker( QWidget *parent ) : QListWidget(parent)
	{
		setResizeMode( QListView::Adjust );
		setUniformItemSizes( true );
		setWordWrap( true );

		setGridView();
	}


	///
	/// Set List of Templates to Pick From
	///
	void TemplatePicker::setTemplates( const QList <model::Template*> &tmplates )
	{
		foreach (model::Template *tmplate, tmplates)
		{
			new TemplatePickerItem( tmplate, this );
		}
	}


	///
	/// Set Grid View
	///
	void TemplatePicker::setGridView()
	{
		setViewMode( QListView::IconMode );
		setSpacing( 24 );
		setIconSize( QSize( TemplatePickerItem::SIZE, TemplatePickerItem::SIZE ) );
	}

	
	///
	/// Set List View
	///
	void TemplatePicker::setListView()
	{
		setViewMode( QListView::ListMode );
		setSpacing( 0 );
		setIconSize( QSize( 24, 24 ) );
	}

	
	///
	/// Apply Filter to Narrow Template Choices by search criteria
	///
	void TemplatePicker::applyFilter( const QString& searchString,
	                                  bool isoMask, bool usMask, bool otherMask,
	                                  bool anyCategory, const QStringList& categoryIds )
	{
		foreach ( QListWidgetItem *item, findItems( "*", Qt::MatchWildcard ) )
		{
			if (auto *tItem = dynamic_cast<TemplatePickerItem *>(item))
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
					item->setHidden( false );
				}
				else
				{
					item->setHidden( true );
					item->setSelected( false );
				}
			}
		}
	}


	///
	/// Apply Filter to Narrow Template Choices by a list of names
	///
	void TemplatePicker::applyFilter( const QStringList& names )
	{
		foreach ( QListWidgetItem *item, findItems( "*", Qt::MatchWildcard ) )
		{
			if (auto *tItem = dynamic_cast<TemplatePickerItem *>(item))
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
					item->setHidden( false );
				}
				else
				{
					item->setHidden( true );
					item->setSelected( false );
				}
			}
		}
	}


	///
	/// Get Currently Selected Template
	///
	const model::Template *TemplatePicker::selectedTemplate()
	{
		QList<QListWidgetItem *> items = selectedItems();
		if ( !items.isEmpty() )
		{
			if (auto *tItem = dynamic_cast<TemplatePickerItem*>(items.first()))
			{
				return tItem->tmplate();
			}
		}
		
		return nullptr;
	}

} // namespace glabels
