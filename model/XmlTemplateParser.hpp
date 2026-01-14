//  XmlTemplateParser.hpp
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

#ifndef model_XmlTemplateParser_hpp
#define model_XmlTemplateParser_hpp


#include "Template.hpp"

#include <QDomElement>
#include <QList>
#include <QString>


namespace glabels::model
{

        class XmlTemplateParser
        {
        public:
                XmlTemplateParser() = default;

                QList<Template> readFile( const QString& fileName );
                QList<Template> readEquivsFromFile( const QString& fileName );
                Template parseTemplateNode( const QDomElement& node,
                                            const QString&     fileName = "" );

        private:
                enum Pass { TEMPLATE_PASS, EQUIV_PASS };

                QList<Template> parseRootNode( const QDomElement& node,
                                               const QString&     fileName,
                                               Pass               pass );

                void parseMetaNode( const QDomElement& node, Template& tmplate );
                void parseLabelRectangleNode( const QDomElement& node, Template& tmplate );
                void parseLabelEllipseNode( const QDomElement& node, Template& tmplate );
                void parseLabelRoundNode( const QDomElement& node, Template& tmplate );
                void parseLabelCdNode( const QDomElement& node, Template& tmplate );
                void parseLabelPathNode( const QDomElement& node, Template& tmplate );
                void parseLabelContinuousNode( const QDomElement& node, Template& tmplate );
                void parseLabelNodeCommon( const QDomElement& node, Frame& frame );
                void parseLayoutNode( const QDomElement& node, Frame& frame );
                void parseMarkupMarginNode( const QDomElement& node, Frame& frame );
                void parseMarkupLineNode( const QDomElement& node, Frame& frame );
                void parseMarkupCircleNode( const QDomElement& node, Frame& frame );
                void parseMarkupRectNode( const QDomElement& node, Frame& frame );
                void parseMarkupEllipseNode( const QDomElement& node, Frame& frame );

        };

}


#endif // model_XmlTemplateParser_hpp
