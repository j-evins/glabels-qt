/*  XmlLabelParser.cpp
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

#include "XmlLabelParser.h"

#include "Model.h"
#include "ModelObject.h"
#include "ModelBarcodeObject.h"
#include "ModelBoxObject.h"
#include "ModelEllipseObject.h"
#include "ModelImageObject.h"
#include "ModelLineObject.h"
#include "ModelTextObject.h"
#include "XmlTemplateParser.h"
#include "XmlUtil.h"
#include "DataCache.h"

#include "XmlLabelParser_3.h"

#include "barcode/Backends.h"
#include "merge/Factory.h"

#include <QByteArray>
#include <QFile>
#include <QTextCursor>
#include <QTextDocument>
#include <QtDebug>

#if HAVE_ZLIB
#include <zlib.h>
#endif

namespace glabels
{
	namespace model
	{

		Model*
		XmlLabelParser::readFile( const QString& fileName )
		{
			QFile file( fileName );

			if ( !file.open( QFile::ReadOnly ) )
			{
				qWarning() << "Error: Cannot read file" << fileName
				           << ":" << file.errorString();
				return nullptr;
			}

			QDomDocument doc;
			bool         success;
			QString      errorString;
			int          errorLine;
			int          errorColumn;

			QByteArray rawData = file.readAll();
			if ( ((rawData[0]&0xFF) == 0x1F) && ((rawData[1]&0xFF) == 0x8b) ) // gzip magic number 0x1F, 0x8B
			{
#if HAVE_ZLIB
				// gzip compressed format
				QByteArray unzippedData;
				gunzip( rawData, unzippedData );
				success = doc.setContent( unzippedData, false, &errorString, &errorLine, &errorColumn );
#else
				qWarning() << "Warning: Cannot read compressed glabels project file!  gLabels not built with ZLIB.";
				return nullptr;
#endif
			}
			else
			{
				// plain text
				success = doc.setContent( rawData, false, &errorString, &errorLine, &errorColumn );
			}

			if ( !success )
			{
				qWarning() << "Error: Parse error at line " << errorLine
				           << "column " << errorColumn
				           << ": " << errorString;
				return nullptr;
			}
	

			QDomElement root = doc.documentElement();
			if ( root.tagName() != "Glabels-document" )
			{
				qWarning() << "Error: Not a Glabels-document file";
				return nullptr;
			}

			return parseRootNode( root, fileName );
		}


		Model*
		XmlLabelParser::readBuffer( const QByteArray& buffer )
		{
			QDomDocument doc;
			QString      errorString;
			int          errorLine;
			int          errorColumn;

			if ( !doc.setContent( buffer, false, &errorString, &errorLine, &errorColumn ) )
			{
				qWarning() << "Error: Parse error at line " << errorLine
				           << "column " << errorColumn
				           << ": " << errorString;
				return nullptr;
			}

			QDomElement root = doc.documentElement();
			if ( root.tagName() != "Glabels-document" )
			{
				qWarning() << "Error: Not a Glabels-document file";
				return nullptr;
			}

			return parseRootNode( root, QString() );
		}


		QList<ModelObject*>
		XmlLabelParser::deserializeObjects( const QByteArray& buffer, const Model* model )
		{
			QList<ModelObject*> list;
	
			QDomDocument doc;
			QString      errorString;
			int          errorLine;
			int          errorColumn;

			if ( !doc.setContent( buffer, false, &errorString, &errorLine, &errorColumn ) )
			{
				qWarning() << "Error: Parse error at line " << errorLine
				           << "column " << errorColumn
				           << ": " << errorString;
				return list;
			}

			QDomElement root = doc.documentElement();
			if ( root.tagName() != "Glabels-objects" )
			{
				qWarning() << "Error: Not a Glabels-objects stream";
				return list;
			}

			/* Pass 1, extract data nodes to pre-load cache. */
			DataCache data;
			for ( QDomNode child = root.firstChild(); !child.isNull(); child = child.nextSibling() )
			{
				if ( child.toElement().tagName() == "Data" )
				{
					parseDataNode( child.toElement(), model, data );
				}
			}

			/* Pass 2, now extract objects. */
			for ( QDomNode child = root.firstChild(); !child.isNull(); child = child.nextSibling() )
			{
				if ( child.toElement().tagName() == "Objects" )
				{
					list = parseObjectsNode( child.toElement(), model, data );
				}
			}

			return list;
		}


#if HAVE_ZLIB
		void
		XmlLabelParser::gunzip( const QByteArray& data, QByteArray& result )
		{
			result.clear();

			if (data.size() <= 4) {
				qWarning("XmlLabelParser::gunzip: Input data is truncated");
				return;
			}

			// setup stream for inflate()
			z_stream strm;
			strm.zalloc = Z_NULL;
			strm.zfree = Z_NULL;
			strm.opaque = Z_NULL;
			strm.avail_in = data.size();
			strm.next_in = (Bytef*)(data.data());

			int ret = inflateInit2(&strm, MAX_WBITS + 16); // gzip decoding
			if (ret != Z_OK)
			{
				return;
			}

			static const int CHUNK_SIZE = 1024;
			char out[CHUNK_SIZE];

			// run inflate(), one chunk at a time
			do {
				strm.avail_out = CHUNK_SIZE;
				strm.next_out = (Bytef*)(out);

				ret = inflate(&strm, Z_NO_FLUSH);
				Q_ASSERT(ret != Z_STREAM_ERROR);  // state not clobbered

				if ( (ret == Z_NEED_DICT) || (ret == Z_DATA_ERROR) || (ret == Z_MEM_ERROR) )
				{
					// clean up
					inflateEnd(&strm);
					return;
				}

				result.append(out, CHUNK_SIZE - strm.avail_out);
			} while (strm.avail_out == 0);

			// clean up
			inflateEnd(&strm);
		}
#endif
		

		Model*
		XmlLabelParser::parseRootNode( const QDomElement &node, const QString& fileName )
		{
			QString version = XmlUtil::getStringAttr( node, "version", "" );
			if ( version != "4.0" )
			{
				// Attempt to import as version 3.0 format (glabels 2.0 - glabels 3.4)
				auto* model = XmlLabelParser_3::parseRootNode( node );
				if ( model )
				{
					model->setFileName( fileName );
				}
				return model;
			}

			auto* model = new Model();
			model->setFileName( fileName );

			/* Pass 1, extract data nodes to pre-load cache. */
			DataCache data;
			for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
			{
				if ( child.toElement().tagName() == "Data" )
				{
					parseDataNode( child.toElement(), model, data );
				}
			}

			/* Pass 2, now extract everything else. */
			for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
			{
				QString tagName = child.toElement().tagName();
		
				if ( tagName == "Template" )
				{
					Template* tmplate = XmlTemplateParser().parseTemplateNode( child.toElement() );
					if ( tmplate == nullptr )
					{
						qWarning() << "Unable to parse template";
						delete model;
						return nullptr;
					}
					model->setTmplate( tmplate ); // Copies arg
					delete tmplate;
				}
				else if ( tagName == "Objects" )
				{
					model->setRotate( parseRotateAttr( child.toElement() ) );
					auto list = parseObjectsNode( child.toElement(), model, data );
					foreach ( ModelObject* object, list )
					{
						model->addObject( object );
					}
				}
				else if ( tagName == "Merge" )
				{
					parseMergeNode( child.toElement(), model );
				}
				else if ( tagName == "Variables" )
				{
					parseVariablesNode( child.toElement(), model );
				}
				else if ( tagName == "Data" )
				{
					/* Handled in pass 1. */
				}
				else if ( !child.isComment() )
				{
					qWarning() << "Unexpected" << node.tagName() << "child:" << tagName;
				}
			}

			model->clearModified();
			return model;
		}


		QList<ModelObject*>
		XmlLabelParser::parseObjectsNode( const QDomElement& node,
		                                  const Model*       model,
		                                  const DataCache&   data )
		{
			QList<ModelObject*> list;

			for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
			{
				QString tagName = child.toElement().tagName();
		
				if ( tagName == "Object-box" )
				{
					list.append( parseObjectBoxNode( child.toElement() ) );
				}
				else if ( tagName == "Object-ellipse" )
				{
					list.append( parseObjectEllipseNode( child.toElement() ) );
				}
				else if ( tagName == "Object-line" )
				{
					list.append( parseObjectLineNode( child.toElement() ) );
				}
				else if ( tagName == "Object-text" )
				{
					list.append( parseObjectTextNode( child.toElement() ) );
				}
				else if ( tagName == "Object-image" )
				{
					list.append( parseObjectImageNode( child.toElement(), model, data ) );
				}
				else if ( tagName == "Object-barcode" )
				{
					list.append( parseObjectBarcodeNode( child.toElement() ) );
				}
				else if ( !child.isComment() )
				{
					qWarning() << "Unexpected" << node.tagName() << "child:" << tagName;
				}
			}

			return list;
		}


		ModelBoxObject*
		XmlLabelParser::parseObjectBoxNode( const QDomElement &node )
		{
			/* position attrs */
			Distance x0 = XmlUtil::getLengthAttr( node, "x", 0.0 );
			Distance y0 = XmlUtil::getLengthAttr( node, "y", 0.0 );

			/* size attrs */
			Distance w = XmlUtil::getLengthAttr( node, "w", 0 );
			Distance h = XmlUtil::getLengthAttr( node, "h", 0 );
			bool lockAspectRatio = XmlUtil::getBoolAttr( node, "lock_aspect_ratio", false );

			/* line attrs */
			Distance lineWidth = XmlUtil::getLengthAttr( node, "line_width", 1.0 );

			QString  key        = XmlUtil::getStringAttr( node, "line_color_field", "" );
			bool     field_flag = !key.isEmpty();
			uint32_t color      = XmlUtil::getUIntAttr( node, "line_color", 0xFF );
			ColorNode lineColorNode( field_flag, color, key );

			/* fill attrs */
			key        = XmlUtil::getStringAttr( node, "fill_color_field", "" );
			field_flag = !key.isEmpty();
			color      = XmlUtil::getUIntAttr( node, "fill_color", 0xFF );
			ColorNode fillColorNode( field_flag, color, key );
        
			/* affine attrs */
			double a[6];
			a[0] = XmlUtil::getDoubleAttr( node, "a0", 1.0 );
			a[1] = XmlUtil::getDoubleAttr( node, "a1", 0.0 );
			a[2] = XmlUtil::getDoubleAttr( node, "a2", 0.0 );
			a[3] = XmlUtil::getDoubleAttr( node, "a3", 1.0 );
			a[4] = XmlUtil::getDoubleAttr( node, "a4", 0.0 );
			a[5] = XmlUtil::getDoubleAttr( node, "a5", 0.0 );

			/* shadow attrs */
			bool     shadowState   = XmlUtil::getBoolAttr( node, "shadow", false );
			Distance shadowX       = XmlUtil::getLengthAttr( node, "shadow_x", 0.0 );
			Distance shadowY       = XmlUtil::getLengthAttr( node, "shadow_y", 0.0 );
			double   shadowOpacity = XmlUtil::getDoubleAttr( node, "shadow_opacity", 1.0 );

			key        = XmlUtil::getStringAttr( node, "shadow_color_field", "" );
			field_flag = !key.isEmpty();
			color      = XmlUtil::getUIntAttr( node, "shadow_color", 0xFF );
			ColorNode shadowColorNode( field_flag, color, key );

			return new ModelBoxObject( x0, y0, w, h, lockAspectRatio,
			                           lineWidth, lineColorNode,
			                           fillColorNode,
			                           QTransform( a[0], a[1], a[2], a[3], a[4], a[5] ),
			                           shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );
		}


		ModelEllipseObject*
		XmlLabelParser::parseObjectEllipseNode( const QDomElement &node )
		{
			/* position attrs */
			Distance x0 = XmlUtil::getLengthAttr( node, "x", 0.0 );
			Distance y0 = XmlUtil::getLengthAttr( node, "y", 0.0 );

			/* size attrs */
			Distance w = XmlUtil::getLengthAttr( node, "w", 0 );
			Distance h = XmlUtil::getLengthAttr( node, "h", 0 );
			bool lockAspectRatio = XmlUtil::getBoolAttr( node, "lock_aspect_ratio", false );

			/* line attrs */
			Distance lineWidth = XmlUtil::getLengthAttr( node, "line_width", 1.0 );

			QString  key        = XmlUtil::getStringAttr( node, "line_color_field", "" );
			bool     field_flag = !key.isEmpty();
			uint32_t color      = XmlUtil::getUIntAttr( node, "line_color", 0xFF );
			ColorNode lineColorNode( field_flag, color, key );

			/* fill attrs */
			key        = XmlUtil::getStringAttr( node, "fill_color_field", "" );
			field_flag = !key.isEmpty();
			color      = XmlUtil::getUIntAttr( node, "fill_color", 0xFF );
			ColorNode fillColorNode( field_flag, color, key );
        
			/* affine attrs */
			double a[6];
			a[0] = XmlUtil::getDoubleAttr( node, "a0", 1.0 );
			a[1] = XmlUtil::getDoubleAttr( node, "a1", 0.0 );
			a[2] = XmlUtil::getDoubleAttr( node, "a2", 0.0 );
			a[3] = XmlUtil::getDoubleAttr( node, "a3", 1.0 );
			a[4] = XmlUtil::getDoubleAttr( node, "a4", 0.0 );
			a[5] = XmlUtil::getDoubleAttr( node, "a5", 0.0 );

			/* shadow attrs */
			bool     shadowState   = XmlUtil::getBoolAttr( node, "shadow", false );
			Distance shadowX       = XmlUtil::getLengthAttr( node, "shadow_x", 0.0 );
			Distance shadowY       = XmlUtil::getLengthAttr( node, "shadow_y", 0.0 );
			double   shadowOpacity = XmlUtil::getDoubleAttr( node, "shadow_opacity", 1.0 );

			key        = XmlUtil::getStringAttr( node, "shadow_color_field", "" );
			field_flag = !key.isEmpty();
			color      = XmlUtil::getUIntAttr( node, "shadow_color", 0xFF );
			ColorNode shadowColorNode( field_flag, color, key );

			return new ModelEllipseObject( x0, y0, w, h, lockAspectRatio,
			                               lineWidth, lineColorNode,
			                               fillColorNode,
			                               QTransform( a[0], a[1], a[2], a[3], a[4], a[5] ),
			                               shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );
		}


		ModelLineObject*
		XmlLabelParser::parseObjectLineNode( const QDomElement &node )
		{
			/* position attrs */
			Distance x0 = XmlUtil::getLengthAttr( node, "x", 0.0 );
			Distance y0 = XmlUtil::getLengthAttr( node, "y", 0.0 );

			/* size attrs of line */
			Distance dx = XmlUtil::getLengthAttr( node, "dx", 0 );
			Distance dy = XmlUtil::getLengthAttr( node, "dy", 0 );

			/* line attrs */
			Distance lineWidth = XmlUtil::getLengthAttr( node, "line_width", 1.0 );

			QString  key        = XmlUtil::getStringAttr( node, "line_color_field", "" );
			bool     field_flag = !key.isEmpty();
			uint32_t color      = XmlUtil::getUIntAttr( node, "line_color", 0xFF );
			ColorNode lineColorNode( field_flag, color, key );

			/* affine attrs */
			double a[6];
			a[0] = XmlUtil::getDoubleAttr( node, "a0", 1.0 );
			a[1] = XmlUtil::getDoubleAttr( node, "a1", 0.0 );
			a[2] = XmlUtil::getDoubleAttr( node, "a2", 0.0 );
			a[3] = XmlUtil::getDoubleAttr( node, "a3", 1.0 );
			a[4] = XmlUtil::getDoubleAttr( node, "a4", 0.0 );
			a[5] = XmlUtil::getDoubleAttr( node, "a5", 0.0 );

			/* shadow attrs */
			bool     shadowState   = XmlUtil::getBoolAttr( node, "shadow", false );
			Distance shadowX       = XmlUtil::getLengthAttr( node, "shadow_x", 0.0 );
			Distance shadowY       = XmlUtil::getLengthAttr( node, "shadow_y", 0.0 );
			double   shadowOpacity = XmlUtil::getDoubleAttr( node, "shadow_opacity", 1.0 );

			key        = XmlUtil::getStringAttr( node, "shadow_color_field", "" );
			field_flag = !key.isEmpty();
			color      = XmlUtil::getUIntAttr( node, "shadow_color", 0xFF );
			ColorNode shadowColorNode( field_flag, color, key );

			return new ModelLineObject( x0, y0, dx, dy,
			                            lineWidth, lineColorNode,
			                            QTransform( a[0], a[1], a[2], a[3], a[4], a[5] ),
			                            shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );
		}


		ModelImageObject*
		XmlLabelParser::parseObjectImageNode( const QDomElement& node,
		                                      const Model*       model,
		                                      const DataCache&   data )
		{
			/* position attrs */
			Distance x0 = XmlUtil::getLengthAttr( node, "x", 0.0 );
			Distance y0 = XmlUtil::getLengthAttr( node, "y", 0.0 );

			/* size attrs */
			Distance w = XmlUtil::getLengthAttr( node, "w", 0 );
			Distance h = XmlUtil::getLengthAttr( node, "h", 0 );
			bool lockAspectRatio = XmlUtil::getBoolAttr( node, "lock_aspect_ratio", false );

			/* file attrs */
			QString key        = XmlUtil::getStringAttr( node, "src_field", "" );
			bool    field_flag = !key.isEmpty();
			QString filename   = XmlUtil::getStringAttr( node, "src", "" );
			TextNode filenameNode( field_flag, field_flag ? key : filename );

			/* affine attrs */
			double a[6];
			a[0] = XmlUtil::getDoubleAttr( node, "a0", 1.0 );
			a[1] = XmlUtil::getDoubleAttr( node, "a1", 0.0 );
			a[2] = XmlUtil::getDoubleAttr( node, "a2", 0.0 );
			a[3] = XmlUtil::getDoubleAttr( node, "a3", 1.0 );
			a[4] = XmlUtil::getDoubleAttr( node, "a4", 0.0 );
			a[5] = XmlUtil::getDoubleAttr( node, "a5", 0.0 );

			/* shadow attrs */
			bool     shadowState   = XmlUtil::getBoolAttr( node, "shadow", false );
			Distance shadowX       = XmlUtil::getLengthAttr( node, "shadow_x", 0.0 );
			Distance shadowY       = XmlUtil::getLengthAttr( node, "shadow_y", 0.0 );
			double   shadowOpacity = XmlUtil::getDoubleAttr( node, "shadow_opacity", 1.0 );

			key            = XmlUtil::getStringAttr( node, "shadow_color_field", "" );
			field_flag     = !key.isEmpty();
			uint32_t color = XmlUtil::getUIntAttr( node, "shadow_color", 0xFF );
			ColorNode shadowColorNode( field_flag, color, key );

			if ( filenameNode.isField() )
			{
				return new ModelImageObject( x0, y0, w, h, lockAspectRatio,
				                             filenameNode,
				                             QTransform( a[0], a[1], a[2], a[3], a[4], a[5] ),
				                             shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );
			}
			else
			{
				QString fn = QDir::cleanPath( model->dir().absoluteFilePath( filename ) );

				if ( data.hasImage( fn ) )
				{
					return new ModelImageObject( x0, y0, w, h, lockAspectRatio,
					                             filename, data.getImage( fn ),
					                             QTransform( a[0], a[1], a[2], a[3], a[4], a[5] ),
					                             shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );
				}
				else if ( data.hasSvg( fn ) )
				{
					return new ModelImageObject( x0, y0, w, h, lockAspectRatio,
					                             filename, data.getSvg( fn ),
					                             QTransform( a[0], a[1], a[2], a[3], a[4], a[5] ),
					                             shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );
				}
				else
				{
					if ( !filename.isEmpty() )
					{
						qWarning() << "Embedded file" << fn << "missing. Trying actual file.";
						filenameNode.setData( fn );
					}
					return new ModelImageObject( x0, y0, w, h, lockAspectRatio,
					                             filenameNode,
					                             QTransform( a[0], a[1], a[2], a[3], a[4], a[5] ),
					                             shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );
				}
			}
		}


		ModelBarcodeObject*
		XmlLabelParser::parseObjectBarcodeNode( const QDomElement &node )
		{
			/* position attrs */
			Distance x0 = XmlUtil::getLengthAttr( node, "x", 0.0 );
			Distance y0 = XmlUtil::getLengthAttr( node, "y", 0.0 );

			/* size attrs */
			Distance w = XmlUtil::getLengthAttr( node, "w", 0 );
			Distance h = XmlUtil::getLengthAttr( node, "h", 0 );
			bool lockAspectRatio = XmlUtil::getBoolAttr( node, "lock_aspect_ratio", false );

			/* barcode attrs */
			barcode::Style bcStyle = barcode::Backends::style( XmlUtil::getStringAttr( node, "backend", "" ),
			                                                   XmlUtil::getStringAttr( node, "style", "") );
			bool bcTextFlag = XmlUtil::getBoolAttr( node, "text", true );
			bool bcChecksumFlag = XmlUtil::getBoolAttr( node, "checksum", true );

			QString  key        = XmlUtil::getStringAttr( node, "color_field", "" );
			bool     field_flag = !key.isEmpty();
			uint32_t color      = XmlUtil::getUIntAttr( node, "color", 0xFF );
			ColorNode bcColorNode( field_flag, color, key );

			QString bcData = XmlUtil::getStringAttr( node, "data", "" );

			/* affine attrs */
			double a[6];
			a[0] = XmlUtil::getDoubleAttr( node, "a0", 1.0 );
			a[1] = XmlUtil::getDoubleAttr( node, "a1", 0.0 );
			a[2] = XmlUtil::getDoubleAttr( node, "a2", 0.0 );
			a[3] = XmlUtil::getDoubleAttr( node, "a3", 1.0 );
			a[4] = XmlUtil::getDoubleAttr( node, "a4", 0.0 );
			a[5] = XmlUtil::getDoubleAttr( node, "a5", 0.0 );

			return new ModelBarcodeObject( x0, y0, w, h, lockAspectRatio,
			                               bcStyle, bcTextFlag, bcChecksumFlag, bcData, bcColorNode,
			                               QTransform( a[0], a[1], a[2], a[3], a[4], a[5] ) );
		}


		ModelTextObject*
		XmlLabelParser::parseObjectTextNode( const QDomElement &node )
		{
			/* position attrs */
			Distance x0 = XmlUtil::getLengthAttr( node, "x", 0.0 );
			Distance y0 = XmlUtil::getLengthAttr( node, "y", 0.0 );

			/* size attrs */
			Distance w = XmlUtil::getLengthAttr( node, "w", 0 );
			Distance h = XmlUtil::getLengthAttr( node, "h", 0 );
			bool lockAspectRatio = XmlUtil::getBoolAttr( node, "lock_aspect_ratio", false );

			/* color attr */
			QString  key        = XmlUtil::getStringAttr( node, "color_field", "" );
			bool     field_flag = !key.isEmpty();
			uint32_t color      = XmlUtil::getUIntAttr( node, "color", 0xFF );
			ColorNode textColorNode( field_flag, color, key );

			/* font attrs */
			QString       fontFamily        = XmlUtil::getStringAttr( node, "font_family", "Sans" );
			double        fontSize          = XmlUtil::getDoubleAttr( node, "font_size", 10 );
			QFont::Weight fontWeight        = XmlUtil::getWeightAttr( node, "font_weight", QFont::Normal );
			bool          fontItalicFlag    = XmlUtil::getBoolAttr( node, "font_italic", false );
			bool          fontUnderlineFlag = XmlUtil::getBoolAttr( node, "font_underline", false );

			/* text attrs */
			double textLineSpacing             = XmlUtil::getDoubleAttr( node, "line_spacing", 1 );
			Qt::Alignment textHAlign           = XmlUtil::getAlignmentAttr( node, "align", Qt::AlignLeft );
			Qt::Alignment textVAlign           = XmlUtil::getAlignmentAttr( node, "valign", Qt::AlignTop );
			QTextOption::WrapMode textWrapMode = XmlUtil::getWrapModeAttr( node, "wrap", QTextOption::WordWrap );
			bool textAutoShrink                = XmlUtil::getBoolAttr( node, "auto_shrink", false );

			/* affine attrs */
			double a[6];
			a[0] = XmlUtil::getDoubleAttr( node, "a0", 1.0 );
			a[1] = XmlUtil::getDoubleAttr( node, "a1", 0.0 );
			a[2] = XmlUtil::getDoubleAttr( node, "a2", 0.0 );
			a[3] = XmlUtil::getDoubleAttr( node, "a3", 1.0 );
			a[4] = XmlUtil::getDoubleAttr( node, "a4", 0.0 );
			a[5] = XmlUtil::getDoubleAttr( node, "a5", 0.0 );

			/* shadow attrs */
			bool     shadowState   = XmlUtil::getBoolAttr( node, "shadow", false );
			Distance shadowX       = XmlUtil::getLengthAttr( node, "shadow_x", 0.0 );
			Distance shadowY       = XmlUtil::getLengthAttr( node, "shadow_y", 0.0 );
			double   shadowOpacity = XmlUtil::getDoubleAttr( node, "shadow_opacity", 1.0 );

			key        = XmlUtil::getStringAttr( node, "shadow_color_field", "" );
			field_flag = !key.isEmpty();
			color      = XmlUtil::getUIntAttr( node, "shadow_color", 0xFF );
			ColorNode shadowColorNode( field_flag, color, key );

			/* deserialize contents. */
			QTextDocument document;
			QTextCursor cursor( &document );
			bool firstBlock = true;
			for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
			{
				QString tagName = child.toElement().tagName();
		
				if ( tagName == "p" )
				{
					if ( !firstBlock )
					{
						cursor.insertBlock();
					}
					firstBlock = false;
					cursor.insertText( parsePNode( child.toElement() ) );
				}
				else if ( !child.isComment() )
				{
					qWarning() << "Unexpected" << node.tagName() << "child:" << tagName;
				}
			}
			QString text = document.toPlainText();

			return new ModelTextObject( x0, y0, w, h, lockAspectRatio,
			                            text,
			                            fontFamily, fontSize, fontWeight, fontItalicFlag, fontUnderlineFlag,
			                            textColorNode, textHAlign, textVAlign, textWrapMode, textLineSpacing,
			                            textAutoShrink,
			                            QTransform( a[0], a[1], a[2], a[3], a[4], a[5] ),
			                            shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );
		}


		QString
		XmlLabelParser::parsePNode( const QDomElement &node )
		{
			return node.text();
		}


		bool
		XmlLabelParser::parseRotateAttr( const QDomElement &node )
		{
			return XmlUtil::getBoolAttr( node, "rotate", false );
		}

	
		void
		XmlLabelParser::parseMergeNode( const QDomElement &node, Model* model )
		{
			QString id  = XmlUtil::getStringAttr( node, "type", "None" );
			QString src = XmlUtil::getStringAttr( node, "src", "" );

			merge::Merge* merge = merge::Factory::createMerge( id );

			switch ( merge::Factory::idToType( id ) )
			{
			case merge::Factory::NONE:
			case merge::Factory::FIXED:
				break;

			case merge::Factory::FILE:
				{
					QString fn = QDir::cleanPath( model->dir().absoluteFilePath( src ) );
					merge->setSource( fn );
				}
				break;

			default:
				qWarning() << "XmlLabelParser::parseMergeNode(): Should not be reached!";
				break;
			}

			model->setMerge( merge );
		}


		void
		XmlLabelParser::parseVariablesNode( const QDomElement &node, Model* model )
		{
			for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
			{
				QString tagName = child.toElement().tagName();
		
				if ( tagName == "Variable" )
				{
					parseVariableNode( child.toElement(), model );
				}
				else if ( !child.isComment() )
				{
					qWarning() << "Unexpected" << node.tagName() << "child:" << tagName;
				}
			}
		}


		void
		XmlLabelParser::parseVariableNode( const QDomElement &node, Model* model )
		{
			QString typeString      = XmlUtil::getStringAttr( node, "type", "string" );
			QString name            = XmlUtil::getStringAttr( node, "name", "unknown" );
			QString initialValue    = XmlUtil::getStringAttr( node, "initialValue", "0" );
			QString incrementString = XmlUtil::getStringAttr( node, "increment", "never" );
			QString stepSize        = XmlUtil::getStringAttr( node, "stepSize", "0" );

			auto type      = Variable::idStringToType( typeString );
			auto increment = Variable::idStringToIncrement( incrementString );

			Variable v( type, name, initialValue, increment, stepSize );
			model->variables()->addVariable( v );
		}


		void
		XmlLabelParser::parseDataNode( const QDomElement &node,
		                               const Model*       model,
		                               DataCache&         data )
		{
			for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
			{
				QString tagName = child.toElement().tagName();

				if ( tagName == "File" )
				{
					parseFileNode( child.toElement(), model, data );
				}
				else if ( !child.isComment() )
				{
					qWarning() << "Unexpected" << node.tagName() << "child:" << tagName;
				}
			}
		}


		void
		XmlLabelParser::parseFileNode( const QDomElement& node,
		                               const Model*       model,
		                               DataCache&         data )
		{
			QString name     = XmlUtil::getStringAttr( node, "name", "" );
			QString mimetype = XmlUtil::getStringAttr( node, "mimetype", "image/png" );
			QString encoding = XmlUtil::getStringAttr( node, "encoding", "base64" );

			// Rewrite name as absolute file path
			QString fn = QDir::cleanPath( model->dir().absoluteFilePath( name ) );

			if ( mimetype == "image/png" )
			{
				if ( encoding == "base64" )
				{
					QByteArray ba64 = node.text().toUtf8();
					QByteArray ba = QByteArray::fromBase64( ba64 );
					QImage image;
					image.loadFromData( ba, "PNG" );

					data.addImage( fn, image );
				}
				else
				{
					qWarning() << "Unexpected encoding:" << encoding << "node:" << node.tagName(); 
				}
			}
			else if ( mimetype == "image/svg+xml" )
			{
				data.addSvg( fn, node.text().toUtf8() );
			}
		}


	}
}
