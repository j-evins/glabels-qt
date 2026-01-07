//  XmlVendorParser.cpp
//
//  Copyright (C) 2013-2016  Jaye Evins <evins@snaught.com>
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


#include "XmlVendorParser.hpp"

#include "XmlUtil.hpp"

#include <QDebug>
#include <QDomDocument>
#include <QDomNode>
#include <QFile>


namespace glabels::model
{

        QList<Vendor> XmlVendorParser::readFile( const QString &fileName )
        {
                QFile file( fileName );

                if ( !file.open( QFile::ReadOnly | QFile::Text) )
                {
                        qWarning() << "Error: Cannot read file " << fileName
                                   << ": " << file.errorString();
                        return QList<Vendor>(); // Empty list
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
                        return QList<Vendor>(); // Empty list
                }

                QDomElement root = doc.documentElement();
                if ( root.tagName() != "Glabels-vendors" )
                {
                        qWarning() << "Error: Not a Glabels-vendors file.";
                        return QList<Vendor>(); // Empty list
                }

                return parseRootNode( root );
        }


        QList<Vendor> XmlVendorParser::parseRootNode( const QDomElement &node )
        {
                QList<Vendor> list;

                for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
                {
                        if ( child.toElement().tagName() == "Vendor" )
                        {
                                list.push_back( parseVendorNode( child.toElement() ) );
                        }
                        else if ( !child.isComment() )
                        {
                                qWarning() << "Warning: bad element: "
                                           << child.toElement().tagName()
                                           << ", Ignored.";
                        }
                }

                return list;
        }


        Vendor XmlVendorParser::parseVendorNode( const QDomElement &node )
        {
                QString name = XmlUtil::getStringAttr( node, "name", "" );
                QString url  = XmlUtil::getStringAttr( node, "url", "" );

                return Vendor( name, url );
        }


}
