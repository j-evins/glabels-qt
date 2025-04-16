/*  XmlLabelCreator.cpp
 *
 *  Copyright (C) 2014  Jaye Evins <evins@snaught.com>
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

#include "XmlLabelCreator.h"

#include "Model.h"
#include "ModelObject.h"
#include "ModelBarcodeObject.h"
#include "ModelBoxObject.h"
#include "ModelEllipseObject.h"
#include "ModelLineObject.h"
#include "ModelImageObject.h"
#include "ModelTextObject.h"
#include "DataCache.h"
#include "FileUtil.h"
#include "Variables.h"
#include "XmlTemplateCreator.h"
#include "XmlUtil.h"

#include "merge/Factory.h"
#include "merge/None.h"

#include <QByteArray>
#include <QFile>
#include <QTextBlock>
#include <QTextDocument>
#include <QBuffer>
#include <QtDebug>


namespace glabels
{
	namespace model
	{

		void
		XmlLabelCreator::writeFile( Model* model, const QString& fileName )
		{
			QFile file( fileName );
			if ( !file.open( QFile::WriteOnly | QFile::Text) )
			{
				qWarning() << "Error: Cannot write file " << fileName
				           << ": " << file.errorString();
				return;
			}

			model->setFileName( fileName );
			model->clearModified();
			
			QDomDocument doc;
			createDoc( doc, model );

			QByteArray buffer = doc.toByteArray( 2 );
			file.write( buffer.data(), buffer.size() );
		}


		void
		XmlLabelCreator::writeBuffer( const Model* model, QByteArray& buffer )
		{
			QDomDocument doc;

			createDoc( doc, model );
			buffer = doc.toByteArray( 2 );
		}


		void
		XmlLabelCreator::serializeObjects( const QList<ModelObject*>& objects,
		                                   const Model*               model,
		                                   QByteArray&                buffer )
		{
			QDomDocument doc;

			QDomNode xmlNode( doc.createProcessingInstruction( "xml", "version=\"1.0\"" ) );
			doc.appendChild( xmlNode );

			QDomElement root = doc.createElement( "Glabels-objects" );
			doc.appendChild( root );
			XmlUtil::setStringAttr( root, "version", "4.0" );

			createDataNode( root, model, objects );
			createObjectsNode( root, model, objects, false );

			buffer = doc.toByteArray( 2 );
		}


		void
		XmlLabelCreator::createDoc( QDomDocument& doc, const Model* model )
		{
			QDomNode xmlNode( doc.createProcessingInstruction( "xml", "version=\"1.0\"" ) );
			doc.appendChild( xmlNode );

			QDomElement root = doc.createElement( "Glabels-document" );
			doc.appendChild( root );
			XmlUtil::setStringAttr( root, "version", "4.0" );

			XmlTemplateCreator().createTemplateNode( root, model->tmplate() );

			createObjectsNode( root, model, model->objectList(), model->rotate() );

			if ( model->merge() && !dynamic_cast<merge::None*>(model->merge()) )
			{
				createMergeNode( root, model );
			}

			if ( model->variables()->size() != 0 )
			{
				createVariablesNode( root, model );
			}

			createDataNode( root, model, model->objectList() );
		}


		void
		XmlLabelCreator::createObjectsNode( QDomElement&               parent,
		                                    const Model*               model,
		                                    const QList<ModelObject*>& objects,
		                                    bool                       rotate )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Objects" );
			parent.appendChild( node );

			XmlUtil::setStringAttr( node, "id", "0" );
			XmlUtil::setBoolAttr( node, "rotate", rotate );

			foreach ( ModelObject* object, objects )
			{
				if ( auto* boxObject = dynamic_cast<ModelBoxObject*>(object) )
				{
					createObjectBoxNode( node, boxObject );
				}
				else if ( auto* ellipseObject = dynamic_cast<ModelEllipseObject*>(object) )
				{
					createObjectEllipseNode( node, ellipseObject );
				}
				else if ( auto* lineObject = dynamic_cast<ModelLineObject*>(object) )
				{
					createObjectLineNode( node, lineObject );
				}
				else if ( auto* imageObject = dynamic_cast<ModelImageObject*>(object) )
				{
					createObjectImageNode( node, model, imageObject );
				}
				else if ( auto* barcodeObject = dynamic_cast<ModelBarcodeObject*>(object) )
				{
					createObjectBarcodeNode( node, barcodeObject );
				}
				else if ( auto* textObject = dynamic_cast<ModelTextObject*>(object) )
				{
					createObjectTextNode( node, textObject );
				}
				else
				{
					Q_ASSERT_X( false, "XmlLabelCreator::createObjectsNode", "Invalid object type." );
				}
			}
		}


		void
		XmlLabelCreator::createObjectBoxNode( QDomElement &parent, const ModelBoxObject* object )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Object-box" );
			parent.appendChild( node );

			/* position attrs */
			createPositionAttrs( node, object );

			/* size attrs */
			createSizeAttrs( node, object );

			/* line attrs */
			createLineAttrs( node, object );

			/* fill attrs */
			createFillAttrs( node, object );

			/* affine attrs */
			createAffineAttrs( node, object );

			/* shadow attrs */
			createShadowAttrs( node, object );
		}


		void
		XmlLabelCreator::createObjectEllipseNode( QDomElement &parent, const ModelEllipseObject* object )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Object-ellipse" );
			parent.appendChild( node );

			/* position attrs */
			createPositionAttrs( node, object );

			/* size attrs */
			createSizeAttrs( node, object );

			/* line attrs */
			createLineAttrs( node, object );

			/* fill attrs */
			createFillAttrs( node, object );

			/* affine attrs */
			createAffineAttrs( node, object );

			/* shadow attrs */
			createShadowAttrs( node, object );
		}


		void
		XmlLabelCreator::createObjectLineNode( QDomElement &parent, const ModelLineObject* object )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Object-line" );
			parent.appendChild( node );

			/* position attrs */
			createPositionAttrs( node, object );

			/* size attrs of line */
			XmlUtil::setLengthAttr( node, "dx", object->w() );
			XmlUtil::setLengthAttr( node, "dy", object->h() );

			/* line attrs */
			createLineAttrs( node, object );

			/* affine attrs */
			createAffineAttrs( node, object );

			/* shadow attrs */
			createShadowAttrs( node, object );
		}


		void
		XmlLabelCreator::createObjectImageNode( QDomElement&            parent,
		                                        const Model*            model,
		                                        const ModelImageObject* object )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Object-image" );
			parent.appendChild( node );

			/* position attrs */
			createPositionAttrs( node, object );

			/* size attrs */
			createSizeAttrs( node, object );

			/* file attrs */
			if ( object->filenameNode().isField() )
			{
				XmlUtil::setStringAttr( node, "src_field", object->filenameNode().data() );
			}
			else
			{
				QString fn = FileUtil::makeRelativeIfInDir( model->dir(), object->filenameNode().data() );
				XmlUtil::setStringAttr( node, "src", fn );
			}

			/* affine attrs */
			createAffineAttrs( node, object );

			/* shadow attrs */
			createShadowAttrs( node, object );
		}


		void
		XmlLabelCreator::createObjectBarcodeNode( QDomElement &parent, const ModelBarcodeObject* object )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Object-barcode" );
			parent.appendChild( node );

			/* position attrs */
			createPositionAttrs( node, object );

			/* size attrs */
			createSizeAttrs( node, object );

			/* barcode attrs */
			XmlUtil::setStringAttr( node, "backend", object->bcStyle().backendId() );
			XmlUtil::setStringAttr( node, "style", object->bcStyle().id() );
			XmlUtil::setBoolAttr( node, "text", object->bcTextFlag() );
			XmlUtil::setBoolAttr( node, "checksum", object->bcChecksumFlag() );
			if ( object->bcColorNode().isField() )
			{
				XmlUtil::setStringAttr( node, "color_field", object->bcColorNode().key() );
			}
			else
			{
				XmlUtil::setUIntAttr( node, "color", object->bcColorNode().rgba() );
			}
			XmlUtil::setStringAttr( node, "data", object->bcData() );

			/* affine attrs */
			createAffineAttrs( node, object );
		}


		void
		XmlLabelCreator::createObjectTextNode( QDomElement &parent, const ModelTextObject* object )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Object-text" );
			parent.appendChild( node );

			/* position attrs */
			createPositionAttrs( node, object );

			/* size attrs */
			createSizeAttrs( node, object );

			/* color attr */
			if ( object->textColorNode().isField() )
			{
				XmlUtil::setStringAttr( node, "color_field", object->textColorNode().key() );
			}
			else
			{
				XmlUtil::setUIntAttr( node, "color", object->textColorNode().rgba() );
			}

			/* font attrs */
			XmlUtil::setStringAttr( node, "font_family", object->fontFamily() );
			XmlUtil::setDoubleAttr( node, "font_size", object->fontSize() );
			XmlUtil::setWeightAttr( node, "font_weight", object->fontWeight() );
			XmlUtil::setBoolAttr( node, "font_italic", object->fontItalicFlag() );
			XmlUtil::setBoolAttr( node, "font_underline", object->fontUnderlineFlag() );

			/* text attrs */
			XmlUtil::setAlignmentAttr( node, "align", object->textHAlign() );
			XmlUtil::setAlignmentAttr( node, "valign", object->textVAlign() );
			XmlUtil::setWrapModeAttr( node, "wrap", object->textWrapMode() );
			XmlUtil::setDoubleAttr( node, "line_spacing", object->textLineSpacing() );
			XmlUtil::setBoolAttr( node, "auto_shrink", object->textAutoShrink() );

			/* affine attrs */
			createAffineAttrs( node, object );

			/* shadow attrs */
			createShadowAttrs( node, object );

			/* serialize text contents */
			QTextDocument document( object->text() );
			int nBlocks = document.blockCount();
			for ( int iBlock = 0; iBlock < nBlocks; iBlock++ )
			{
				createPNode( node, document.findBlockByNumber(iBlock).text() );
			}
		}


		void
		XmlLabelCreator::createPNode( QDomElement &parent, const QString& blockText )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "p" );
			parent.appendChild( node );

			node.appendChild( doc.createTextNode( blockText ) );
		}


		void
		XmlLabelCreator::createPositionAttrs( QDomElement &node, const ModelObject* object )
		{
			XmlUtil::setLengthAttr( node, "x", object->x0() );
			XmlUtil::setLengthAttr( node, "y", object->y0() );
		}

	
		void
		XmlLabelCreator::createSizeAttrs( QDomElement &node, const ModelObject* object )
		{
			XmlUtil::setLengthAttr( node, "w", object->w() );
			XmlUtil::setLengthAttr( node, "h", object->h() );
			XmlUtil::setBoolAttr( node, "lock_aspect_ratio", object->lockAspectRatio() );
		}

	
		void
		XmlLabelCreator::createLineAttrs( QDomElement &node, const ModelObject* object )
		{
			XmlUtil::setLengthAttr( node, "line_width", object->lineWidth() );
			if ( object->lineColorNode().isField() )
			{
				XmlUtil::setStringAttr( node, "line_color_field", object->lineColorNode().key() );
			}
			else
			{
				XmlUtil::setUIntAttr( node, "line_color", object->lineColorNode().rgba() );
			}
		}

	
		void
		XmlLabelCreator::createFillAttrs( QDomElement &node, const ModelObject* object )
		{
			if ( object->fillColorNode().isField() )
			{
				XmlUtil::setStringAttr( node, "fill_color_field", object->fillColorNode().key() );
			}
			else
			{
				XmlUtil::setUIntAttr( node, "fill_color", object->fillColorNode().rgba() );
			}
		}


		void
		XmlLabelCreator::createAffineAttrs( QDomElement &node, const ModelObject* object )
		{
			QTransform a = object->matrix();
	
			XmlUtil::setDoubleAttr( node, "a0", a.m11() );
			XmlUtil::setDoubleAttr( node, "a1", a.m12() );
			XmlUtil::setDoubleAttr( node, "a2", a.m21() );
			XmlUtil::setDoubleAttr( node, "a3", a.m22() );
			XmlUtil::setDoubleAttr( node, "a4", a.dx() );
			XmlUtil::setDoubleAttr( node, "a5", a.dy() );
		}


		void
		XmlLabelCreator::createShadowAttrs( QDomElement &node, const ModelObject* object )
		{
			XmlUtil::setBoolAttr( node, "shadow", object->shadow() );

			XmlUtil::setLengthAttr( node, "shadow_x", object->shadowX() );
			XmlUtil::setLengthAttr( node, "shadow_y", object->shadowY() );

			if ( object->fillColorNode().isField() )
			{
				XmlUtil::setStringAttr( node, "shadow_color_field", object->shadowColorNode().key() );
			}
			else
			{
				XmlUtil::setUIntAttr( node, "shadow_color", object->shadowColorNode().rgba() );
			}

			XmlUtil::setDoubleAttr( node, "shadow_opacity", object->shadowOpacity() );
		}


		void
		XmlLabelCreator::createMergeNode( QDomElement &parent, const Model* model )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Merge" );
			parent.appendChild( node );

			QString id = model->merge()->id();
			QString src = model->merge()->source();
			
			XmlUtil::setStringAttr( node, "type", id );

			switch ( merge::Factory::idToType( id ) )
			{
			case merge::Factory::NONE:
			case merge::Factory::FIXED:
				break;

			case merge::Factory::FILE:
				{
					QString fn = FileUtil::makeRelativeIfInDir( model->dir(), src );
					XmlUtil::setStringAttr( node, "src", fn );
				}
				break;

			default:
				qWarning() << "XmlLabelCreator::createMergeNode(): Should not be reached!";
				break;
			}
		}


		void
		XmlLabelCreator::createVariablesNode( QDomElement &parent, const Model* model )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Variables" );
			parent.appendChild( node );

			for ( const auto& v : *model->variables() )
			{
				createVariableNode( node, v );
			}
		}


		void
		XmlLabelCreator::createVariableNode( QDomElement &parent, const Variable& v )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Variable" );
			parent.appendChild( node );

			XmlUtil::setStringAttr( node, "type", Variable::typeToIdString( v.type() ) );
			XmlUtil::setStringAttr( node, "name", v.name() );
			XmlUtil::setStringAttr( node, "initialValue", v.initialValue() );
			
			if ( (v.type() == Variable::Type::INTEGER) ||
			     (v.type() == Variable::Type::FLOATING_POINT) )
			{
				XmlUtil::setStringAttr( node, "increment",
				                        Variable::incrementToIdString( v.increment() ) );
				
				if ( v.increment() != Variable::Increment::NEVER )
				{
					XmlUtil::setStringAttr( node, "stepSize", v.stepSize() );
				}
			}
		}


		void
		XmlLabelCreator::createDataNode( QDomElement&               parent,
		                                 const Model*               model,
		                                 const QList<ModelObject*>& objects )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Data" );
			parent.appendChild( node );

			DataCache data( objects );

			foreach ( QString name, data.imageNames() )
			{
				QString fn = FileUtil::makeRelativeIfInDir( model->dir(), name );
				createPngFileNode( node, fn, data.getImage( name ) );
			}

			foreach ( QString name, data.svgNames() )
			{
				QString fn = FileUtil::makeRelativeIfInDir( model->dir(), name );
				createSvgFileNode( node, fn, data.getSvg( name ) );
			}
		}


		void
		XmlLabelCreator::createPngFileNode( QDomElement &parent, const QString& name, const QImage& image )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "File" );
			parent.appendChild( node );

			XmlUtil::setStringAttr( node, "name", name );
			XmlUtil::setStringAttr( node, "mimetype", "image/png" );
			XmlUtil::setStringAttr( node, "encoding", "base64" );

			QByteArray ba;
			QBuffer buffer(&ba);
			buffer.open(QIODevice::WriteOnly);
			image.save(&buffer, "PNG");
			QByteArray ba64 = ba.toBase64();

			node.appendChild( doc.createTextNode( QString( ba64 ) ) );
		}


		void
		XmlLabelCreator::createSvgFileNode( QDomElement &parent, const QString& name, const QByteArray& svg )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "File" );
			parent.appendChild( node );

			XmlUtil::setStringAttr( node, "name", name );
			XmlUtil::setStringAttr( node, "mimetype", "image/svg+xml" );
			XmlUtil::setStringAttr( node, "encoding", "cdata" );

			node.appendChild( doc.createCDATASection( QString( svg ) ) );
		}
	}
}
