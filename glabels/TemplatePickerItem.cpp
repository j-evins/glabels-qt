/*  TemplatePickerItem.cpp
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


#include "TemplatePickerItem.h"

#include "MiniPreviewPixmap.h"

#include "model/Settings.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QListWidgetItem>


namespace glabels
{

	///
	/// Constructor
	///
	TemplatePickerItem::TemplatePickerItem( model::Template*    tmplate,
	                                        QListView::ViewMode mode,
	                                        QListWidget*        parent )
		: QListWidgetItem( parent )
	{
		mTmplate = tmplate;

		setIcon( QIcon( MiniPreviewPixmap( tmplate, SIZE, SIZE ) ) );
		setMode( mode );
		
		setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
	}


	///
	/// Configure for given View Mode
	///
	void TemplatePickerItem::setMode( QListView::ViewMode mode )
	{
		auto* frame = mTmplate->frames().first();

		switch ( mode )
		{
			
		case QListView::IconMode:
			setText( mTmplate->name() );
			break;

		case QListView::ListMode:
			setText( "<b>" + mTmplate->name() + "</b><br/>" +
			         mTmplate->description() + "<br/>" +
			         frame->sizeDescription( model::Settings::units() ) + "<br/>" +
			         frame->layoutDescription() );
			break;
			
		default:
			qWarning() << "TemplatePickerItem: unknown mode!";
			break;

		}
	}


	///
	/// Template Property Getter
	///
	const model::Template *TemplatePickerItem::tmplate() const
	{
		return mTmplate;
	}

} // namespace glabels
