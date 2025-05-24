/*  TemplatePickerItem.h
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

#ifndef TemplatePickerItem_h
#define TemplatePickerItem_h


#include "model/Template.h"

#include <QLabel>
#include <QListWidget>


namespace glabels
{

	///
	/// Template Picker Item Widget
	///
	class TemplatePickerItem : public QListWidgetItem
	{
	public:
		static const int SIZE = 80;

		
		/////////////////////////////////
		// Life Cycle
		/////////////////////////////////
	public:
		TemplatePickerItem( model::Template*    tmplate,
		                    QListView::ViewMode mode,
		                    QListWidget*        parent = nullptr );


		/////////////////////////////////
		// Manipulate widget
		/////////////////////////////////
	public:
		void setMode( QListView::ViewMode mode );


		/////////////////////////////////
		// Properties
		/////////////////////////////////
	public:
		const model::Template *tmplate() const;


		/////////////////////////////////
		// Private Data
		/////////////////////////////////
	private:
		model::Template *mTmplate;

	};

}


#endif // TemplatePickerItem_h
