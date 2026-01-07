//  XmlTemplateParser.cpp
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


#include "XmlTemplateParser.hpp"

#include "Db.hpp"
#include "FrameRect.hpp"
#include "FrameCd.hpp"
#include "FrameRound.hpp"
#include "FrameEllipse.hpp"
#include "FramePath.hpp"
#include "FrameContinuous.hpp"
#include "Layout.hpp"
#include "Markup.hpp"
#include "Template.hpp"
#include "XmlUtil.hpp"

#include <QDebug>
#include <QDomDocument>
#include <QDomNode>
#include <QFile>


namespace glabels::model
{

        QList<Template> XmlTemplateParser::readFile( const QString& fileName )
        {
                QFile file( fileName );

                if ( !file.open( QFile::ReadOnly | QFile::Text) )
                {
                        qWarning() << "Error: Cannot read file " << fileName
                                   << ": " << file.errorString();
                        return QList<Template>(); // Empty list
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
                        return QList<Template>(); // Empty list
                }

                QDomElement root = doc.documentElement();
                if ( root.tagName() != "Glabels-templates" )
                {
                        qWarning() << "Error: Not a Glabels-templates file";
                        return QList<Template>(); // Empty list
                }

                return parseRootNode( root, fileName, TEMPLATE_PASS );
        }


        QList<Template> XmlTemplateParser::readEquivsFromFile( const QString& fileName )
        {
                QFile file( fileName );

                if ( !file.open( QFile::ReadOnly | QFile::Text) )
                {
                        qWarning() << "Error: Cannot read file " << fileName
                                   << ": " << file.errorString();
                        return QList<Template>(); // Empty list
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
                        return QList<Template>(); // Empty list
                }

                QDomElement root = doc.documentElement();
                if ( root.tagName() != "Glabels-templates" )
                {
                        qWarning() << "Error: Not a Glabels-templates file";
                        return QList<Template>(); // Empty list
                }

                return parseRootNode( root, fileName, EQUIV_PASS );
        }


        QList<Template> XmlTemplateParser::parseRootNode( const QDomElement& node,
                                                          const QString&     fileName,
                                                          Pass               pass )
        {
                QList<Template> list;

                for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
                {
                        if ( child.toElement().tagName() == "Template" )
                        {
                                bool isEquivNode = child.toElement().hasAttribute( "equiv" );
                                if ( ( (pass == TEMPLATE_PASS) && !isEquivNode ) ||
                                     ( (pass == EQUIV_PASS) && isEquivNode ) )
                                {
                                        auto tmplate = parseTemplateNode( child.toElement(), fileName );
                                        if ( !tmplate.isNull() )
                                        {
                                                list.push_back( tmplate );
                                        }
                                        else
                                        {
                                                qWarning() << "Warning: could not create template, Ignored.";
                                        }
                                }
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


        Template XmlTemplateParser::parseTemplateNode( const QDomElement& node,
                                                       const QString&     fileName )
        {
                QString brand = XmlUtil::getStringAttr( node, "brand", "" );
                QString part  = XmlUtil::getStringAttr( node, "part", "" );

                if ( (brand == "") || (part == "") )
                {
                        // Try the deprecated "name" attribute.
                        QString name = XmlUtil::getStringAttr( node, "name", "" );
                        if ( name != "" )
                        {
                                QStringList fields = name.split( " ", Qt::SkipEmptyParts );
                                brand = fields[0];
                                part  = fields[1];
                        }
                        else
                        {
                                qWarning() << "Error: missing name or brand/part attributes.";
                                return Template();
                        }
                }


                Template tmplate;

                QString equivPart = XmlUtil::getStringAttr( node, "equiv", "" );
                if ( equivPart != nullptr )
                {
                        tmplate = Template::fromEquiv( brand, part, equivPart );

                        for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
                        {
                                if ( child.toElement().tagName() == "Meta" )
                                {
                                        parseMetaNode( child.toElement(), tmplate );
                                }
                                else if ( !child.isComment() )
                                {
                                        qWarning() << "Warning: bad element: "
                                                   << child.toElement().tagName()
                                                   << ", Ignored.";
                                }
                        }
                }
                else
                {
                        QString description = XmlUtil::getI18nAttr( node, "description", "" );
                        QString paperId     = XmlUtil::getStringAttr( node, "size", "" );

                        if ( Db::isPaperIdKnown( paperId ) )
                        {
                                auto paper = Db::lookupPaperFromId( paperId );

                                tmplate = Template( brand, part, description,
                                                    paper.id(), paper.width(), paper.height(), 0, fileName );
                        }
                        else
                        {
                                Distance width  = XmlUtil::getLengthAttr( node, "width", Distance(0) );
                                Distance height = XmlUtil::getLengthAttr( node, "height", Distance(0) );
                                Distance rollWidth = XmlUtil::getLengthAttr( node, "roll_width", Distance(0) );

                                tmplate = Template( brand,
                                                    part,
                                                    description,
                                                    paperId,
                                                    width,
                                                    height,
                                                    rollWidth,
                                                    fileName );
                        }

                        for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
                        {
                                if ( child.toElement().tagName() == "Meta" )
                                {
                                        parseMetaNode( child.toElement(), tmplate );
                                }
                                else if ( child.toElement().tagName() == "Label-rectangle" )
                                {
                                        parseLabelRectangleNode( child.toElement(), tmplate );
                                }
                                else if ( child.toElement().tagName() == "Label-ellipse" )
                                {
                                        parseLabelEllipseNode( child.toElement(), tmplate );
                                }
                                else if ( child.toElement().tagName() == "Label-round" )
                                {
                                        parseLabelRoundNode( child.toElement(), tmplate );
                                }
                                else if ( child.toElement().tagName() == "Label-cd" )
                                {
                                        parseLabelCdNode( child.toElement(), tmplate );
                                }
                                else if ( child.toElement().tagName() == "Label-path" )
                                {
                                        parseLabelPathNode( child.toElement(), tmplate );
                                }
                                else if ( child.toElement().tagName() == "Label-continuous" )
                                {
                                        parseLabelContinuousNode( child.toElement(), tmplate );
                                }
                                else if ( !child.isComment() )
                                {
                                        qWarning() << "Warning: bad element: "
                                                   << child.toElement().tagName()
                                                   << ", Ignored.";
                                }
                        }
                }

                return tmplate;
        }


        void XmlTemplateParser::parseMetaNode( const QDomElement& node, Template& tmplate )
        {
                QString productUrl = XmlUtil::getStringAttr( node, "product_url", "" );
                if ( productUrl != "" )
                {
                        tmplate.setProductUrl( productUrl );
                }

                QString categoryId = XmlUtil::getStringAttr( node, "category", "" );
                if ( categoryId != "" )
                {
                        tmplate.addCategory( categoryId );
                }
        }


        void XmlTemplateParser::parseLabelRectangleNode( const QDomElement& node, Template& tmplate )
        {
                QString id = XmlUtil::getStringAttr( node, "id", "0" );

                Distance w   = XmlUtil::getLengthAttr( node, "width", Distance(0) );
                Distance h   = XmlUtil::getLengthAttr( node, "height", Distance(0) );
                Distance r   = XmlUtil::getLengthAttr( node, "round", Distance(0) );

                Distance xWaste, yWaste;

                Distance waste = XmlUtil::getLengthAttr( node, "waste", Distance(-1) );
                if ( waste >= Distance(0) )
                {
                        xWaste = waste;
                        yWaste = waste;
                }
                else
                {
                        xWaste = XmlUtil::getLengthAttr( node, "x_waste", Distance(0) );
                        yWaste = XmlUtil::getLengthAttr( node, "y_waste", Distance(0) );
                }

                FrameRect frame( w, h, r, xWaste, yWaste, id );

                parseLabelNodeCommon( node, frame );

                tmplate.addFrame( frame );
        }


        void XmlTemplateParser::parseLabelEllipseNode( const QDomElement& node, Template& tmplate )
        {
                QString id    = XmlUtil::getStringAttr( node, "id", "0" );

                Distance w     = XmlUtil::getLengthAttr( node, "width", Distance(0) );
                Distance h     = XmlUtil::getLengthAttr( node, "height", Distance(0) );
                Distance waste = XmlUtil::getLengthAttr( node, "waste", Distance(0) );

                FrameEllipse frame( w, h, waste, id );

                parseLabelNodeCommon( node, frame );

                tmplate.addFrame( frame );
        }


        void XmlTemplateParser::parseLabelRoundNode( const QDomElement& node, Template& tmplate )
        {
                QString id    = XmlUtil::getStringAttr( node, "id", "0" );

                Distance r     = XmlUtil::getLengthAttr( node, "radius", Distance(0) );
                Distance waste = XmlUtil::getLengthAttr( node, "waste", Distance(0) );

                FrameRound frame( r, waste, id );

                parseLabelNodeCommon( node, frame );

                tmplate.addFrame( frame );
        }


        void XmlTemplateParser::parseLabelCdNode( const QDomElement& node, Template& tmplate )
        {
                QString id    = XmlUtil::getStringAttr( node, "id", "0" );

                Distance r1    = XmlUtil::getLengthAttr( node, "radius", Distance(0) );
                Distance r2    = XmlUtil::getLengthAttr( node, "hole", Distance(0) );
                Distance w     = XmlUtil::getLengthAttr( node, "width", Distance(0) );
                Distance h     = XmlUtil::getLengthAttr( node, "height", Distance(0) );
                Distance waste = XmlUtil::getLengthAttr( node, "waste", Distance(0) );

                FrameCd frame( r1, r2, w, h, waste, id );

                parseLabelNodeCommon( node, frame );

                tmplate.addFrame( frame );
        }


        void XmlTemplateParser::parseLabelPathNode( const QDomElement& node, Template& tmplate )
        {
                QString id    = XmlUtil::getStringAttr( node, "id", "0" );

                Units        dUnits = XmlUtil::getUnitsAttr( node, "d_units", Units::pc() );
                QPainterPath d      = XmlUtil::getPathDataAttr( node, "d", dUnits );

                Distance xWaste, yWaste;

                Distance waste = XmlUtil::getLengthAttr( node, "waste", Distance(-1) );
                if ( waste >= Distance(0) )
                {
                        xWaste = waste;
                        yWaste = waste;
                }
                else
                {
                        xWaste = XmlUtil::getLengthAttr( node, "x_waste", Distance(0) );
                        yWaste = XmlUtil::getLengthAttr( node, "y_waste", Distance(0) );
                }

                FramePath frame( d, xWaste, yWaste, dUnits, id );

                parseLabelNodeCommon( node, frame );

                tmplate.addFrame( frame );
        }


        void XmlTemplateParser::parseLabelContinuousNode( const QDomElement& node, Template& tmplate )
        {
                QString id = XmlUtil::getStringAttr( node, "id", "0" );

                Distance w        = XmlUtil::getLengthAttr( node, "width", Distance(0) );
                Distance h        = XmlUtil::getLengthAttr( node, "height", Distance(0) );
                Distance hMin     = XmlUtil::getLengthAttr( node, "min_height", Distance(0) );
                Distance hMax     = XmlUtil::getLengthAttr( node, "max_height", Distance(0) );
                Distance hDefault = XmlUtil::getLengthAttr( node, "default_height", Distance(0) );

                FrameContinuous frame( w, hMin, hMax, hDefault, id );
                if ( h > Distance(0) )
                {
                        frame.setH( h );
                }

                parseLabelNodeCommon( node, frame );

                tmplate.addFrame( frame );
        }


        void XmlTemplateParser::parseLabelNodeCommon( const QDomElement& node, Frame& frame )
        {
                for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
                {
                        if ( child.toElement().tagName() == "Layout" )
                        {
                                parseLayoutNode( child.toElement(), frame );
                        }
                        else if ( child.toElement().tagName() == "Markup-margin" )
                        {
                                parseMarkupMarginNode( child.toElement(), frame );
                        }
                        else if ( child.toElement().tagName() == "Markup-line" )
                        {
                                parseMarkupLineNode( child.toElement(), frame );
                        }
                        else if ( child.toElement().tagName() == "Markup-circle" )
                        {
                                parseMarkupCircleNode( child.toElement(), frame );
                        }
                        else if ( child.toElement().tagName() == "Markup-rect" )
                        {
                                parseMarkupRectNode( child.toElement(), frame );
                        }
                        else if ( child.toElement().tagName() == "Markup-ellipse" )
                        {
                                parseMarkupEllipseNode( child.toElement(), frame );
                        }
                        else if ( !child.isComment() )
                        {
                                qWarning() << "Warning: bad element: "
                                           << child.toElement().tagName()
                                           << ", Ignored.";
                        }
                }
        }


        void XmlTemplateParser::parseLayoutNode( const QDomElement& node, Frame& frame )
        {
                int      nX = XmlUtil::getIntAttr( node, "nx", 1 );
                int      nY = XmlUtil::getIntAttr( node, "ny", 1 );

                Distance x0 = XmlUtil::getLengthAttr( node, "x0", Distance(0) );
                Distance y0 = XmlUtil::getLengthAttr( node, "y0", Distance(0) );

                Distance dX = XmlUtil::getLengthAttr( node, "dx", Distance(0) );
                Distance dY = XmlUtil::getLengthAttr( node, "dy", Distance(0) );

                frame.addLayout( Layout( nX, nY, x0, y0, dX, dY ) );
        }


        void XmlTemplateParser::parseMarkupMarginNode( const QDomElement& node, Frame& frame )
        {
                Distance size  = XmlUtil::getLengthAttr( node, "size", Distance(0) );
                Distance xSize = XmlUtil::getLengthAttr( node, "x_size", Distance(0) );
                Distance ySize = XmlUtil::getLengthAttr( node, "y_size", Distance(0) );

                if ( size > Distance(0) )
                {
                        frame.addMarkup( MarkupMargin( size ) );
                }
                else
                {
                        frame.addMarkup( MarkupMargin( xSize, ySize ) );
                }
        }


        void XmlTemplateParser::parseMarkupLineNode( const QDomElement& node, Frame& frame )
        {
                Distance x1 = XmlUtil::getLengthAttr( node, "x1", Distance(0) );
                Distance y1 = XmlUtil::getLengthAttr( node, "y1", Distance(0) );
                Distance x2 = XmlUtil::getLengthAttr( node, "x2", Distance(0) );
                Distance y2 = XmlUtil::getLengthAttr( node, "y2", Distance(0) );

                frame.addMarkup( MarkupLine( x1, y1, x2, y2 ) );
        }


        void XmlTemplateParser::parseMarkupCircleNode( const QDomElement& node, Frame& frame )
        {
                Distance x0 = XmlUtil::getLengthAttr( node, "x0", Distance(0) );
                Distance y0 = XmlUtil::getLengthAttr( node, "y0", Distance(0) );
                Distance r  = XmlUtil::getLengthAttr( node, "radius", Distance(0) );

                frame.addMarkup( MarkupCircle( x0, y0, r ) );
        }


        void XmlTemplateParser::parseMarkupRectNode( const QDomElement& node, Frame& frame )
        {
                Distance x1 = XmlUtil::getLengthAttr( node, "x1", Distance(0) );
                Distance y1 = XmlUtil::getLengthAttr( node, "y1", Distance(0) );
                Distance w  = XmlUtil::getLengthAttr( node, "w", Distance(0) );
                Distance h  = XmlUtil::getLengthAttr( node, "h", Distance(0) );
                Distance r  = XmlUtil::getLengthAttr( node, "r", Distance(0) );

                frame.addMarkup( MarkupRect( x1, y1, w, h, r ) );
        }


        void XmlTemplateParser::parseMarkupEllipseNode( const QDomElement& node, Frame& frame )
        {
                Distance x1 = XmlUtil::getLengthAttr( node, "x1", Distance(0) );
                Distance y1 = XmlUtil::getLengthAttr( node, "y1", Distance(0) );
                Distance w  = XmlUtil::getLengthAttr( node, "w", Distance(0) );
                Distance h  = XmlUtil::getLengthAttr( node, "h", Distance(0) );

                frame.addMarkup( MarkupEllipse( x1, y1, w, h ) );
        }


}
