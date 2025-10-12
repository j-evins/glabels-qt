/*  XmlCategoryParser.cpp
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

#include "XmlCategoryParser.h"

#include "Category.h"
#include "Db.h"
#include "XmlUtil.h"

#include <QDomDocument>
#include <QDomNode>
#include <QFile>
#include <QtDebug>


namespace glabels
{
	namespace model
	{

		bool XmlCategoryParser::readFile( const QString &fileName )
		{
			QFile file( fileName );

			if ( !file.open( QFile::ReadOnly | QFile::Text) )
			{
				qWarning() << "Error: Cannot read file " << fileName
				           << ": " << file.errorString();
				return false;
			}


			QDomDocument doc;
			QString      errorString;
			int          errorLine;
			int          errorColumn;

			if ( !doc.setContent( &file, false, &errorString, &errorLine, &errorColumn ) )
			{
				qWarning() << "Error: Parse error at line " << errorLine
				           << "column " << errorColumn
				           << ": " << errorString;
				return false;
			}

			QDomElement root = doc.documentElement();
			if ( root.tagName() != "Glabels-categories" )
			{
				qWarning() << "Error: Not a Glabels-categories file.";
				return false;
			}

			parseRootNode( root );
			return true;
		}


		void XmlCategoryParser::parseRootNode( const QDomElement &node )
		{
			for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
			{
				if ( child.toElement().tagName() == "Category" )
				{
					parseCategoryNode( child.toElement() );
				}
				else if ( !child.isComment() )
				{
					qWarning() << "Warning: bad element: "
					           << child.toElement().tagName()
					           << ", Ignored.";
				}
			}
		}


		void XmlCategoryParser::parseCategoryNode( const QDomElement &node )
		{
			QString id   = XmlUtil::getStringAttr( node, "id", "" );
			QString name = XmlUtil::getI18nAttr( node, "name", "" );

			auto category = Category( id, name );
			Db::registerCategory( category );
		}


	}
}
