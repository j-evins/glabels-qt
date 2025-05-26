/*  Settings.cpp
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


#include "Settings.h"

#include <QLocale>
#include <QString>
#include <QtDebug>


namespace glabels
{
	namespace model
	{

		//
		// Static data
		//
		Settings* Settings::mInstance = nullptr;


		Settings::Settings()
		{
			// empty
		}


		void Settings::init()
		{
			if ( mInstance == nullptr )
			{
				mInstance = new Settings();
			}
		}


		Settings* Settings::instance()
		{
			init();

			return mInstance;
		}


		Units Settings::units()
		{
			// Guess at a suitable default
			QString defaultIdString;
			if ( QLocale::system().measurementSystem() == QLocale::ImperialSystem )
			{
				defaultIdString = Units(Units::IN).toIdString();
			}
			else
			{
				defaultIdString = Units(Units::MM).toIdString();
			}
	
			mInstance->beginGroup( "Locale" );
			QString idString = mInstance->value( "units", defaultIdString ).toString();
			mInstance->endGroup();

			return Units( idString );
		}


		void Settings::setUnits( const Units& units )
		{
			QString idString = units.toIdString();

			mInstance->beginGroup( "Locale" );
			mInstance->setValue( "units", idString );
			mInstance->endGroup();

			emit mInstance->changed();
		}


		Settings::PageSizeFamily Settings::preferedPageSizeFamily()
		{
			// Guess at a suitable default
			QString defaultFamily;
			switch (QLocale::system().territory())
			{
			case QLocale::UnitedStates:
			case QLocale::Canada:
				defaultFamily = "us";
				break;

			default:
				defaultFamily = "iso";
				break;
			}
	
			mInstance->beginGroup( "Locale" );
			QString value = mInstance->value( "preferedPageSizeFamily", defaultFamily ).toString();
			mInstance->endGroup();

			return (value == "iso") ? ISO : US;
		}


		void Settings::setPreferedPageSizeFamily( PageSizeFamily preferedPageSizeFamily )
		{
			mInstance->beginGroup( "Locale" );
			mInstance->setValue( "preferedPageSizeFamily", preferedPageSizeFamily == ISO ? "iso" : "us" );
			mInstance->endGroup();

			emit mInstance->changed();
		}


		bool Settings::searchIsoPaperSizes()
		{
			// Guess at a suitable default
			bool defaultValue;
			switch (QLocale::system().country())
			{
			case QLocale::UnitedStates:
			case QLocale::Canada:
				defaultValue = false;
				break;

			default:
				defaultValue = true;
				break;
			}
	
			mInstance->beginGroup( "Search" );
			bool returnValue = mInstance->value( "isoPaperSizes", defaultValue ).toBool();
			mInstance->endGroup();

			return returnValue;
		}


		void Settings::setSearchIsoPaperSizes( bool searchIsoPaperSizes )
		{
			mInstance->beginGroup( "Search" );
			mInstance->setValue( "isoPaperSizes", searchIsoPaperSizes );
			mInstance->endGroup();

			emit mInstance->changed();
		}


		bool Settings::searchUsPaperSizes()
		{
			// Guess at a suitable default
			bool defaultValue;
			switch (QLocale::system().country())
			{
			case QLocale::UnitedStates:
			case QLocale::Canada:
				defaultValue = true;
				break;

			default:
				defaultValue = false;
				break;
			}
	
			mInstance->beginGroup( "Search" );
			bool returnValue = mInstance->value( "usPaperSizes", defaultValue ).toBool();
			mInstance->endGroup();

			return returnValue;
		}


		void Settings::setSearchUsPaperSizes( bool searchUsPaperSizes )
		{
			mInstance->beginGroup( "Search" );
			mInstance->setValue( "usPaperSizes", searchUsPaperSizes );
			mInstance->endGroup();

			emit mInstance->changed();
		}


		bool Settings::searchOtherPaperSizes()
		{
			// Guess at a suitable default
			bool defaultValue = true;
	
			mInstance->beginGroup( "Search" );
			bool returnValue = mInstance->value( "otherPaperSizes", defaultValue ).toBool();
			mInstance->endGroup();

			return returnValue;
		}


		void Settings::setSearchOtherPaperSizes( bool searchOtherPaperSizes )
		{
			mInstance->beginGroup( "Search" );
			mInstance->setValue( "otherPaperSizes", searchOtherPaperSizes );
			mInstance->endGroup();

			emit mInstance->changed();
		}


		bool Settings::searchAllCategories()
		{
			// Guess at a suitable default
			bool defaultValue = true;
	
			mInstance->beginGroup( "Search" );
			bool returnValue = mInstance->value( "allCategories", defaultValue ).toBool();
			mInstance->endGroup();

			return returnValue;
		}


		void Settings::setSearchAllCategories( bool searchAllCategories )
		{
			mInstance->beginGroup( "Search" );
			mInstance->setValue( "allCategories", searchAllCategories );
			mInstance->endGroup();

			emit mInstance->changed();
		}


		QStringList Settings::searchCategoryList()
		{
			QStringList defaultList;
	
			mInstance->beginGroup( "Search" );
			QStringList returnList = mInstance->value( "categoryList", defaultList ).toStringList();
			mInstance->endGroup();

			return returnList;
		}


		void Settings::setSearchCategoryList( const QStringList& searchCategoryList )
		{
			mInstance->beginGroup( "Search" );
			mInstance->setValue( "categoryList", searchCategoryList );
			mInstance->endGroup();

			emit mInstance->changed();
		}


		QListView::ViewMode Settings::templatePickerMode()
		{
			QString defaultMode = "icon";
	
			mInstance->beginGroup( "TemplatePicker" );
			QString returnMode = mInstance->value( "viewMode", defaultMode ).toString();
			mInstance->endGroup();

			return returnMode == "icon" ? QListView::IconMode : QListView::ListMode;
		}


		void Settings::setTemplatePickerMode( QListView::ViewMode viewMode )
		{
			mInstance->beginGroup( "TemplatePicker" );
			mInstance->setValue( "viewMode", viewMode == QListView::IconMode ? "icon" : "list" );
			mInstance->endGroup();

			emit mInstance->changed();
		}


		QStringList Settings::recentTemplateList()
		{
			QStringList defaultList;
	
			mInstance->beginGroup( "Recent" );
			QStringList returnList = mInstance->value( "templates", defaultList ).toStringList();
			mInstance->endGroup();

			return returnList;
		}


		void Settings::addToRecentTemplateList( const QString& name )
		{
			mInstance->beginGroup( "Recent" );

			QStringList list = mInstance->value( "templates" ).toStringList();

			list.removeAll( name );
			list.prepend( name );
			while ( list.count() > 10 )
			{
				list.removeLast();
			}

			mInstance->setValue( "templates", list );

			mInstance->endGroup();

			emit mInstance->changed();
		}


		int Settings::maxRecentFiles()
		{
			return mMaxRecentFiles;
		}


		QStringList Settings::recentFileList()
		{
			QStringList defaultList;
	
			mInstance->beginGroup( "Recent" );
			QStringList returnList = mInstance->value( "files", defaultList ).toStringList();
			mInstance->endGroup();

			return returnList;
		}


		void Settings::addToRecentFileList( const QString& filePath )
		{
			mInstance->beginGroup( "Recent" );

			QStringList list = mInstance->value( "files" ).toStringList();

			list.removeAll( filePath );
			list.prepend( filePath );
			while ( list.count() > mMaxRecentFiles )
			{
				list.removeLast();
			}

			mInstance->setValue( "files", list );

			mInstance->endGroup();

			emit mInstance->changed();
		}

	}
}
