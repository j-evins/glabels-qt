//  XmlPaperParser.cpp
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


#include "XmlPaperParser.hpp"

#include "XmlUtil.hpp"

#include <QDebug>
#include <QDomDocument>
#include <QDomNode>
#include <QFile>


namespace glabels::model
{

        QList<Paper> XmlPaperParser::readFile( const QString &fileName )
        {
                QFile file( fileName );

                if ( !file.open( QFile::ReadOnly | QFile::Text ) )
                {
                        qWarning() << "Error: Cannot read file " << fileName
                                   << ": " << file.errorString();
                        return QList<Paper>(); // Empty list
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
                        return QList<Paper>(); // Empty list
                }

                QDomElement root = doc.documentElement();
                if ( root.tagName() != "Glabels-paper-sizes" )
                {
                        qWarning() << "Error: Not a Glabels-paper-sizes file.";
                        return QList<Paper>(); // Empty list
                }

                return parseRootNode( root );

        }


        QList<Paper> XmlPaperParser::parseRootNode( const QDomElement &node )
        {
                QList<Paper> list;

                for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
                {
                        if ( child.toElement().tagName() == "Paper-size" )
                        {
                                list.push_back( parsePaperSizeNode( child.toElement() ) );
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


        Paper XmlPaperParser::parsePaperSizeNode( const QDomElement &node )
        {
                QString id      = XmlUtil::getStringAttr( node, "id", "" );
                QString name    = XmlUtil::getI18nAttr( node, "name", "" );

                Distance width   = XmlUtil::getLengthAttr( node, "width", Distance(0) );
                Distance height  = XmlUtil::getLengthAttr( node, "height", Distance(0) );

                QString pwgSize = XmlUtil::getStringAttr( node, "pwg_size", "" );

                return Paper( id, name, width, height, pwgSize );
        }


}
