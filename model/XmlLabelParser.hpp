//  XmlLabelParser.hpp
//
//  Copyright (C) 2014-2016  Jaye Evins <evins@snaught.com>
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

#ifndef model_XmlLabelParser_hpp
#define model_XmlLabelParser_hpp


#include <QObject>
#include <QDomElement>


namespace glabels::model
{

        // Forward references
        class Model;
        class ModelObject;
        class ModelBoxObject;
        class ModelEllipseObject;
        class ModelLineObject;
        class ModelImageObject;
        class ModelBarcodeObject;
        class ModelTextObject;
        class DataCache;


        ///
        /// XmlLabelParser
        ///
        class XmlLabelParser : public QObject
        {
                Q_OBJECT

        public:
                static Model* readFile( const QString& fileName );

                static Model* readBuffer( const QByteArray& buffer );

                static QList<ModelObject*> deserializeObjects( const QByteArray& buffer,
                                                               const Model*      model );

        private:
                static void gunzip( const QByteArray& gzippedData,
                                    QByteArray&       data );

                static Model* parseRootNode( const QDomElement& node,
                                             const QString&     fileName );

                static QList<ModelObject*> parseObjectsNode( const QDomElement& node,
                                                             const Model*       model,
                                                             const DataCache&   data );

                static ModelBoxObject* parseObjectBoxNode( const QDomElement& node );

                static ModelEllipseObject* parseObjectEllipseNode( const QDomElement& node );

                static ModelLineObject* parseObjectLineNode( const QDomElement& node );

                static ModelImageObject* parseObjectImageNode( const QDomElement& node,
                                                               const Model*       model,
                                                               const DataCache&   data );

                static ModelBarcodeObject* parseObjectBarcodeNode( const QDomElement& node );

                static ModelTextObject* parseObjectTextNode( const QDomElement& node );

                static QString parsePNode( const QDomElement& node );

                static bool parseRotateAttr( const QDomElement& node );

                static void parseMergeNode( const QDomElement& node,
                                            Model*             model );

                static void parseVariablesNode( const QDomElement& node,
                                                Model*             model );

                static void parseVariableNode( const QDomElement& node,
                                               Model*             model );

                static void parseDataNode( const QDomElement& node,
                                           const Model*       model,
                                           DataCache&         data );

                static void parseFileNode( const QDomElement& node,
                                           const Model*       model,
                                           DataCache&         data );

        };

}


#endif // model_XmlLabelParser_hpp
