//  Db.cpp
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


#include "Db.hpp"

#include "Config.hpp"
#include "StrUtil.hpp"
#include "FileUtil.hpp"
#include "Settings.hpp"
#include "XmlCategoryParser.hpp"
#include "XmlPaperParser.hpp"
#include "XmlTemplateParser.hpp"
#include "XmlTemplateCreator.hpp"
#include "XmlVendorParser.hpp"

#include <QDebug>
#include <QtGlobal>

#include <algorithm>


//
// Private
//
namespace
{
        using namespace glabels::model;

        const QString    empty = "";

        bool partNameLessThan( const Template& a, const Template& b )
        {
                return StrUtil::comparePartNames( a.name(), b.name() ) < 0;
        }
}


namespace glabels::model
{

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

        QMap<QString,Template> Db::mUserTemplatesNameMap;


        void Db::init()
        {
                readPapers();
                readCategories();
                readVendors();
                readTemplates();
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


        QList<Template> Db::templates()
        {
                auto list = mTemplates;
                list.append( mUserTemplatesNameMap.values() );

                std::stable_sort( list.begin(), list.end(), partNameLessThan );

                return list;
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

                auto it2 = mUserTemplatesNameMap.find( name );
                if ( it2 != mUserTemplatesNameMap.end() )
                {
                        return *it2;
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

                auto name = Template::brandPartToName( brand, part );

                auto it = mTemplatesNameMap.find( name );
                if ( it != mTemplatesNameMap.end() )
                {
                        return *it;
                }

                auto it2 = mUserTemplatesNameMap.find( name );
                if ( it2 != mUserTemplatesNameMap.end() )
                {
                        return *it2;
                }

                qWarning() << "Unknown template brand, part: " << brand << ", " << part;
                return Template();
        }


        const Template Db::lookupUserTemplateFromBrandPart( const QString& brand, const QString& part )
        {
                if ( brand.isEmpty() || part.isEmpty() )
                {
                        qWarning() << "NULL template brand and/or part.";
                        return Template();
                }

                auto name = Template::brandPartToName( brand, part );

                auto it = mUserTemplatesNameMap.find( name );
                if ( it != mUserTemplatesNameMap.end() )
                {
                        return *it;
                }

                qWarning() << "Unknown user template brand, part: " << brand << ", " << part;
                return Template();
        }


        bool Db::isTemplateKnown( const QString& brand, const QString& part )
        {
                auto name = Template::brandPartToName( brand, part );
                return mTemplatesNameMap.contains( name ) || mUserTemplatesNameMap.contains( name );
        }


        bool Db::isSystemTemplateKnown( const QString& brand, const QString& part )
        {
                auto name = Template::brandPartToName( brand, part );
                return mTemplatesNameMap.contains( name );
        }


        bool Db::isUserTemplateKnown( const QString& brand, const QString& part )
        {
                auto name = Template::brandPartToName( brand, part );
                return mUserTemplatesNameMap.contains( name );
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

                for ( auto& tmplate2 : templates() )
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


        QString Db::userTemplateFileName( const QString& brand, const QString& part )
        {
                QString fileName = brand + "_" + part + ".template";
                return FileUtil::userTemplatesDir().filePath( fileName );
        }


        void Db::registerUserTemplate( const Template& tmplate )
        {
                if ( isTemplateKnown( tmplate.brand(), tmplate.part() ) )
                {
                        qWarning() << "Duplicate template name: " << tmplate.name();
                        return;
                }

                QString fileName = userTemplateFileName( tmplate.brand(), tmplate.part() );

                // Write file
                if ( XmlTemplateCreator().writeTemplate( tmplate, fileName ) )
                {
                        // Add template to list of registered templates
                        mUserTemplatesNameMap[ tmplate.name() ] = tmplate;
                        mUserTemplatesNameMap[ tmplate.name() ].setFileName( fileName );
                        mUserTemplatesNameMap[ tmplate.name() ].setIsUserDefined( true );

                        Settings::addToRecentTemplateList( tmplate.name() );

                }
                else
                {
                        qWarning() << "Problem writing user template" << fileName;
                }
        }


        void Db::deleteUserTemplateByBrandPart( const QString& brand, const QString& part )
        {
                auto tmplate = lookupUserTemplateFromBrandPart( brand, part );
                if ( !tmplate.isNull() && tmplate.isUserDefined() )
                {
                        mUserTemplatesNameMap.remove( Template::brandPartToName( brand, part ) );

                        QFile( tmplate.fileName() ).remove();
                }
                else
                {
                        qWarning() << "Not a user defined template:" << tmplate.name();
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
                readTemplatesFromDir( FileUtil::systemTemplatesDir() );
                readTemplatesFromDir( FileUtil::manualUserTemplatesDir() );

                std::stable_sort( mTemplates.begin(), mTemplates.end(), partNameLessThan );

                readUserTemplatesFromDir( FileUtil::userTemplatesDir() );
        }


        void Db::readTemplatesFromDir( const QDir& dir )
        {
                QStringList filters;
                filters << "*-templates.xml" << "*.template";

                XmlTemplateParser parser;

                for ( auto& fileName : dir.entryList( filters, QDir::Files ) )
                {
                        auto list = parser.readFile( dir.absoluteFilePath( fileName ) );
                        for ( auto& tmplate : list )
                        {
                                registerTemplate( tmplate );
                        }

                        list = parser.readEquivsFromFile( dir.absoluteFilePath( fileName ) );
                        for ( auto& tmplate : list )
                        {
                                registerTemplate( tmplate );
                        }
                }
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


        void Db::readUserTemplatesFromDir( const QDir& dir )
        {
                QStringList filters;
                filters << "*-templates.xml" << "*.template";

                XmlTemplateParser parser;

                for ( auto& fileName : dir.entryList( filters, QDir::Files ) )
                {
                        auto list = parser.readFile( dir.absoluteFilePath( fileName ) );
                        for ( auto& tmplate : list )
                        {
                                registerUserTemplate( tmplate );
                        }
                }
        }

}
