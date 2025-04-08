/*  XmlLabelParser_3.h
 *
 *  Copyright (C) 2014-2016  Jaye Evins <evins@snaught.com>
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

#ifndef model_XmlLabelParser_3_h
#define model_XmlLabelParser_3_h


#include <QDomElement>
#include <QFont>
#include <QObject>
#include <QTextOption>

namespace glabels
{
	namespace model
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
		class XmlLabelParser_3
		{

		public:
			static Model* parseRootNode( const QDomElement &node );

		private:

			static QList<ModelObject*> parseObjectsNode( const QDomElement &node, const DataCache& data );
			static ModelBoxObject* parseObjectBoxNode( const QDomElement &node );
			static ModelEllipseObject* parseObjectEllipseNode( const QDomElement &node );
			static ModelLineObject* parseObjectLineNode( const QDomElement &node );
			static ModelImageObject* parseObjectImageNode( const QDomElement &node, const DataCache& data );
			static ModelBarcodeObject* parseObjectBarcodeNode( const QDomElement &node );
			static ModelTextObject* parseObjectTextNode( const QDomElement &node );
			static bool parseRotateAttr( const QDomElement &node );
			static void parseMergeNode( const QDomElement &node, Model* label );
			static void parseDataNode( const QDomElement &node, DataCache& data );
			static void parsePixdataNode( const QDomElement &node, DataCache& data );
			static void parseFileNode( const QDomElement &node, DataCache& data );

			static Qt::Alignment getHAlignmentAttr( const QDomElement& node,
					 const QString& name, const Qt::Alignment default_value );
			static Qt::Alignment getVAlignmentAttr( const QDomElement& node,
					 const QString& name, const Qt::Alignment default_value );
			static QFont::Weight getWeightAttr( const QDomElement& node,
					 const QString& name, const QFont::Weight default_value );

			static QTransform parseAffineTransformation(const QDomElement &node);

		};

	}  // namespace model
}  // namespace glabels


#endif // model_XmlLabelParser_3_h
