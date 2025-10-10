/*  ColorPaletteDialog.h
 *
 *  Copyright (C) 2014-2016  Jaye Evins <evins@snaught.com>
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

#ifndef ColorPaletteDialog_h
#define ColorPaletteDialog_h


#include "ColorHistory.h"
#include "ColorPaletteItem.h"
#include "FieldButton.h"

#include "model/ColorNode.h"

#include <QDialog>


namespace glabels
{

	///
	/// Color Palette Dialog
	///
	class ColorPaletteDialog : public QDialog
	{
		Q_OBJECT


		/////////////////////////////////
		// Life Cycle
		/////////////////////////////////
	public:
		ColorPaletteDialog( const QString& defaultLabel,
		                    const QColor&  defaultColor,
		                    const QColor&  color,
		                    bool           showUseFieldButton = true,
		                    QWidget*       parent = nullptr );


		/////////////////////////////////
		// Signals
		/////////////////////////////////
	signals:
		void colorChanged( model::ColorNode colorNode, bool isDefault );


		/////////////////////////////////
		// Public Methods
		/////////////////////////////////
	public:
		void setColorNode( const model::ColorNode& colorNode );
		
		void setKeys( const merge::Merge*     merge,
		              const model::Variables& variables );


		/////////////////////////////////
		// Slots
		/////////////////////////////////
	private slots:
		void onPaletteItemActivated( int id );
		void onHistoryItemActivated( int id );
		void onDefaultButtonClicked();
		void onCustomColorButtonClicked();
		void onKeySelected( QString key );
		void onColorHistoryChanged();


		/////////////////////////////////
		// Private Methods
		/////////////////////////////////
	private:
		void loadCustomColorHistory();


		/////////////////////////////////
		// Private Members
		/////////////////////////////////
	private:
		QColor           mDefaultColor;
		model::ColorNode mColorNode;

		static const int PALETTE_COLS = ColorHistory::MAX_COLORS;
		static const int PALETTE_ROWS = 4;

		struct ColorTableEntry {
			QString colorSpec;
			const char* trname;
		};

		static ColorTableEntry mColorTable[];

		ColorHistory* mColorHistory;
		ColorPaletteItem* mHistoryItem[PALETTE_COLS];

		FieldButton* mFieldButton;

	};

}


#endif // ColorPaletteDialog_h
