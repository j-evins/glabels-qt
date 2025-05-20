/*  Settings.h
 *
 *  Copyright (C) 2016  Jaye Evins <evins@snaught.com>
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

#ifndef model_Settings_h
#define model_Settings_h


#include "Distance.h"

#include <QListView>
#include <QSettings>
#include <QStringList>


namespace glabels
{
	namespace model
	{

		///
		/// Settings Singleton Class
		///
		class Settings : public QSettings
		{
			Q_OBJECT
	
		public:
			enum PageSizeFamily { ISO, US, };


			/////////////////////////////////
			// Life Cycle
			/////////////////////////////////
		private:
			Settings();

		public:
			static void init();
			static Settings* instance();

	
			/////////////////////////////////
			// Signals
			/////////////////////////////////
		signals:
			void changed();
	

			/////////////////////////////////
			// Accessors
			/////////////////////////////////
		public:
			static Units units();
			static void setUnits( const Units& units );

			static PageSizeFamily preferedPageSizeFamily();
			static void setPreferedPageSizeFamily( PageSizeFamily preferedPageSizeFamily );

			static bool searchIsoPaperSizes();
			static void setSearchIsoPaperSizes( bool searchIsoPaperSizes );

			static bool searchUsPaperSizes();
			static void setSearchUsPaperSizes( bool searchUsPaperSizes );

			static bool searchOtherPaperSizes();
			static void setSearchOtherPaperSizes( bool searchOtherPaperSizes );

			static bool searchAllCategories();
			static void setSearchAllCategories( bool searchAllCategories );

			static QStringList searchCategoryList();
			static void setSearchCategoryList( const QStringList& searchCategoryList );

			static QListView::ViewMode templatePickerMode();
			static void setTemplatePickerMode( QListView::ViewMode viewMode );

			static QStringList recentTemplateList();
			static void addToRecentTemplateList( const QString& name );

			static int maxRecentFiles();
			static QStringList recentFileList();
			static void addToRecentFileList( const QString& filePath );

			
		private:
			static Settings* mInstance;
			static const int mMaxRecentFiles{5};

		};

	}
}


#endif // model_Settings_h
