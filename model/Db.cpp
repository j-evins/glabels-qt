/*  Db.cpp
 *
 *  Copyright (C) 2013-2016  Jaye Evins <evins@snaught.com>
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

#include "Db.h"

#include "Config.h"
#include "StrUtil.h"
#include "FileUtil.h"
#include "Settings.h"
#include "XmlCategoryParser.h"
#include "XmlPaperParser.h"
#include "XmlTemplateParser.h"
#include "XmlTemplateCreator.h"
#include "XmlVendorParser.h"

#include <QDebug>
#include <QtGlobal>

#include <algorithm>


namespace glabels
{
	namespace model
	{
	
		//
		// Private
		//
		namespace
		{
			const QString    empty = "";
	
			bool partNameLessThan( const Template& a, const Template& b )
			{
				return StrUtil::comparePartNames( a.name(), b.name() ) < 0;
			}
		}


		//
		// Static data
		//
		QList<Paper>           Db::mPapers;
		QMap<QString,Paper>    Db::mPapersNameMap;
		QMap<QString,Paper>    Db::mPapersIdMap;
		QStringList            Db::mPaperIds;
		QStringList            Db::mPaperNames;

		QList<Category>        Db::mCategories;
		QMap<QString,Category> Db::mCategoriesNameMap;
		QMap<QString,Category> Db::mCategoriesIdMap;
		QStringList            Db::mCategoryIds;
		QStringList            Db::mCategoryNames;

		QList<Vendor>          Db::mVendors;
		QMap<QString,Vendor>   Db::mVendorsNameMap;
		QStringList            Db::mVendorNames;

		QList<Template>        Db::mTemplates;
		QMap<QString,Template> Db::mTemplatesNameMap;

	
		Db::Db()
		{
			readPapers();
			readCategories();
			readVendors();
			readTemplates();
		}


		void Db::init()
		{
			instance();
		}

	
		Db* Db::instance()
		{
			static auto* db = new Db();
			return db;
		}


		const QList<Paper>& Db::papers()
		{
			return mPapers;
		}


		const QStringList& Db::paperIds()
		{
			return mPaperIds;
		}


		const QStringList& Db::paperNames()
		{
			return mPaperNames;
		}


		const QList<Category>& Db::categories()
		{
			return mCategories;
		}


		const QStringList& Db::categoryIds()
		{
			return mCategoryIds;
		}


		const QStringList& Db::categoryNames()
		{
			return mCategoryNames;
		}


		const QList<Vendor>& Db::vendors()
		{
			return mVendors;
		}


		const QStringList& Db::vendorNames()
		{
			return mVendorNames;
		}


		const QList<Template>& Db::templates()
		{
			return mTemplates;
		}


		const Paper Db::lookupPaperFromName( const QString& name )
		{
			if ( name.isEmpty() )
			{
				qWarning() << "NULL paper name.";
				return Paper();
			}

			auto it = mPapersNameMap.find( name );
			if ( it != mPapersNameMap.end() )
			{
				return *it;
			}

			qWarning() << "Unknown paper name: " << name;
			return Paper();
		}


		const Paper Db::lookupPaperFromId( const QString& id )
		{
			if ( id.isEmpty() )
			{
				qWarning() << "NULL paper id.";
				return Paper();
			}

			auto it = mPapersIdMap.find( id );
			if ( it != mPapersIdMap.end() )
			{
				return *it;
			}

			qWarning() << "Unknown paper ID: " << id;
			return Paper();
		}


		QString Db::lookupPaperIdFromName( const QString& name )
		{
			return lookupPaperFromName( name ).id();
		}


		QString Db::lookupPaperNameFromId( const QString& id )
		{
			return lookupPaperFromId( id ).name();
		}


		bool Db::isPaperIdKnown( const QString& id )
		{
			return mPapersIdMap.contains( id );
		}


		const Category Db::lookupCategoryFromName( const QString& name )
		{
			if ( name.isEmpty() )
			{
				qWarning() << "NULL category name.";
				return Category();
			}

			auto it = mCategoriesNameMap.find( name );
			if ( it != mCategoriesNameMap.end() )
			{
				return *it;
			}

			qWarning() << "Unknown category name: \"%s\"." << name;
			return Category();
		}


		const Category Db::lookupCategoryFromId( const QString& id )
		{
			if ( id.isEmpty() )
			{
				qWarning() << "NULL category id.";
				return Category();
			}

			auto it = mCategoriesIdMap.find( id );
			if ( it != mCategoriesIdMap.end() )
			{
				return *it;
			}

			qWarning() << "Unknown category ID: \"%s\"." << id;
			return Category();
		}


		QString Db::lookupCategoryIdFromName( const QString& name )
		{
			return lookupCategoryFromName( name ).id();
		}


		QString Db::lookupCategoryNameFromId( const QString& id )
		{
			return lookupCategoryFromId( id ).name();
		}


		bool Db::isCategoryIdKnown( const QString& id )
		{
			return mCategoriesIdMap.contains( id );
		}


		const Vendor Db::lookupVendorFromName( const QString& name )
		{
			if ( name.isEmpty() )
			{
				qWarning() << "NULL vendor name.";
				return Vendor();
			}

			auto it = mVendorsNameMap.find( name );
			if ( it != mVendorsNameMap.end() )
			{
				return *it;
			}

			qWarning() << "Unknown vendor name: " << name;
			return Vendor();
		}


		QString Db::lookupVendorUrlFromName( const QString& name )
		{
			return lookupVendorFromName( name ).url();
		}


		bool Db::isVendorNameKnown( const QString& name )
		{
			return mVendorsNameMap.contains( name );
		}


		void Db::registerTemplate( const Template& tmplate )
		{
			if ( !isTemplateKnown( tmplate.brand(), tmplate.part() ) )
			{
				mTemplates.push_back( tmplate );
				mTemplatesNameMap[ tmplate.name() ] = tmplate;
			}
			else
			{
				qWarning() << "Duplicate template name: " << tmplate.name();
			}
		}


		const Template Db::lookupTemplateFromName( const QString& name )
		{
			if ( name.isEmpty() )
			{
				qWarning() << "NULL template name.";
				return Template();
			}

			auto it = mTemplatesNameMap.find( name );
			if ( it != mTemplatesNameMap.end() )
			{
				return *it;
			}

			qWarning() << "Unknown template name: " << name;
			return Template();
		}


		const Template Db::lookupTemplateFromBrandPart( const QString& brand, const QString& part )
		{
			if ( brand.isEmpty() || part.isEmpty() )
			{
				qWarning() << "NULL template brand and/or part.";
				return Template();
			}

			auto it = mTemplatesNameMap.find( Template::brandPartToName( brand, part ) );
			if ( it != mTemplatesNameMap.end() )
			{
				return *it;
			}

			qWarning() << "Unknown template brand, part: " << brand << ", " << part;
			return Template();
		}


		bool Db::isTemplateKnown( const QString& brand, const QString& part )
		{
			return mTemplatesNameMap.contains( Template::brandPartToName( brand, part ) );
		}


		bool Db::isSystemTemplateKnown( const QString& brand, const QString& part )
		{
			auto tmplate = Db::lookupTemplateFromBrandPart( brand, part );
			if ( (tmplate.brand() == brand) &&
			     (tmplate.part() == part)   &&
			     !tmplate.isUserDefined() )
			{
				return true;
			}

			return false;
		}


		QStringList Db::getNameListOfSimilarTemplates( const QString& name )
		{
			QStringList list;

			auto tmplate1 = lookupTemplateFromName( name );
			if ( tmplate1.isNull() )
			{
				qWarning() << "Unknown template name: " << name;
				return list;
			}

			for ( auto& tmplate2 : mTemplates )
			{
				if ( tmplate1.name() != tmplate2.name() )
				{
					if ( tmplate1.isSimilarTo( tmplate2 ) )
					{
						list << tmplate2.name();
					}
				}
			}

			return list;
		}


		QString Db::userTemplateFilename( const QString& brand, const QString& part )
		{
			QString filename = brand + "_" + part + ".template";
			return FileUtil::userTemplatesDir().filePath( filename );
		}


		void Db::registerUserTemplate( const Template& tmplate )
		{
			QString filename = userTemplateFilename( tmplate.brand(), tmplate.part() );

			// Write file
			if ( XmlTemplateCreator().writeTemplate( tmplate, filename ) )
			{
				// Add template to list of registered templates
				registerTemplate( tmplate );
				Settings::addToRecentTemplateList( tmplate.name() );
			}
			else
			{
				qWarning() << "Problem writing user template" << filename;
			}
		}


		void Db::deleteUserTemplateByBrandPart( const QString& brand, const QString& part )
		{
			auto tmplate = lookupTemplateFromBrandPart( brand, part );
			if ( !tmplate.isNull() )
			{
				mTemplates.removeOne( tmplate );
				mTemplatesNameMap.remove( Template::brandPartToName( brand, part ) );

				QString filename = userTemplateFilename( brand, part );
				QFile( filename ).remove();
			}
		}


		void Db::printKnownPapers()
		{
			qDebug() << "KNOWN PAPERS:";

			for ( auto& paper : mPapers )
			{
				qDebug() << "paper "
				         << "id="       << paper.id()          << ", "
				         << "name="     << paper.name()        << ", "
				         << "width="    << paper.width().pt()  << "pts, "
				         << "height="   << paper.height().pt() << "pts, "
				         << "pwg_size=" << paper.pwgSize();
			}

			qDebug();
		}


		void Db::printKnownCategories()
		{
			qDebug() << "KNOWN CATEGORIES:";

			for ( auto& category : mCategories )
			{
				qDebug() << "category "
				         << "id="    << category.id()   << ", "
				         << "name="  << category.name();
			}

			qDebug();
		}


		void Db::printKnownVendors()
		{
			qDebug() << "KNOWN VENDORS:";

			for ( auto& vendor : mVendors )
			{
				qDebug() << "vendor "
				         << "name='" << vendor.name() << ", "
				         << "url='"  << vendor.url();
			}

			qDebug();
		}


		void Db::printKnownTemplates()
		{
			qDebug() << "KNOWN TEMPLATES:";

			for ( auto& tmplate : mTemplates )
			{
				qDebug() << "template "
				         << "brand="       << tmplate.brand()       << ", "
				         << "part="        << tmplate.part()        << ", "
				         << "description=" << tmplate.description();
			}

			qDebug();
		}


		void Db::readPapers()
		{
			readPapersFromDir( FileUtil::systemTemplatesDir() );
		}


		void Db::readPapersFromDir( const QDir& dir )
		{
			XmlPaperParser parser;

			for ( auto fileName : dir.entryList( QDir::Files ) )
			{
				if ( fileName == "paper-sizes.xml" )
				{
					auto list = parser.readFile( dir.absoluteFilePath( fileName ) );
					for ( auto& paper : list )
					{
						registerPaper( paper );
					}
				}
			}
		}


		void Db::registerPaper( const Paper& paper )
		{
			if ( !isPaperIdKnown( paper.id() ) )
			{
				mPapers.push_back( paper );
				mPapersNameMap[ paper.name() ] = paper;
				mPapersIdMap[ paper.id() ] = paper;
				mPaperIds.push_back( paper.id() );
				mPaperNames.push_back( paper.name() );
			}
			else
			{
				qWarning() << "Duplicate paper ID: " << paper.id();
			}
		}


		void Db::readCategories()
		{
			readCategoriesFromDir( FileUtil::systemTemplatesDir() );
		}


		void Db::readCategoriesFromDir( const QDir& dir )
		{
			XmlCategoryParser parser;

			for ( auto fileName : dir.entryList( QDir::Files ) )
			{
				if ( fileName == "categories.xml" )
				{
					auto list = parser.readFile( dir.absoluteFilePath( fileName ) );
					for ( auto& category : list )
					{
						registerCategory( category );
					}
				}
			}
		}


		void Db::registerCategory( const Category& category )
		{
			if ( !isCategoryIdKnown( category.id() ) )
			{
				mCategories.push_back( category );
				mCategoriesNameMap[ category.name() ] = category;
				mCategoriesIdMap[ category.id() ] = category;
				mCategoryIds.push_back( category.id() );
				mCategoryNames.push_back( category.name() );
			}
			else
			{
				qWarning() << "Duplicate category ID: " << category.id();
			}
		}


		void Db::readVendors()
		{
			readVendorsFromDir( FileUtil::systemTemplatesDir() );
		}


		void Db::readVendorsFromDir( const QDir& dir )
		{
			XmlVendorParser parser;

			for ( auto fileName : dir.entryList( QDir::Files ) )
			{
				if ( fileName == "vendors.xml" )
				{
					auto list = parser.readFile( dir.absoluteFilePath( fileName ) );
					for ( auto& vendor : list )
					{
						registerVendor( vendor );
					}
				}
			}
		}


		void Db::registerVendor( const Vendor& vendor )
		{
			if ( !isVendorNameKnown( vendor.name() ) )
			{
				mVendors.push_back( vendor );
				mVendorsNameMap[ vendor.name() ] = vendor;
				mVendorNames.push_back( vendor.name() );
			}
			else
			{
				qWarning() << "Duplicate vendor name: " << vendor.name();
			}
		}


		void Db::readTemplates()
		{
			readTemplatesFromDir( FileUtil::systemTemplatesDir(), false );
			readTemplatesFromDir( FileUtil::manualUserTemplatesDir(), false );
			readTemplatesFromDir( FileUtil::userTemplatesDir(), true );

			std::stable_sort( mTemplates.begin(), mTemplates.end(), partNameLessThan );
		}


		void Db::readTemplatesFromDir( const QDir& dir, bool isUserDefined )
		{
			QStringList filters;
			filters << "*-templates.xml" << "*.template";

			XmlTemplateParser parser;

			foreach ( QString fileName, dir.entryList( filters, QDir::Files ) )
			{
				parser.readFile( dir.absoluteFilePath( fileName ), isUserDefined );
			}
		}

	}
}
