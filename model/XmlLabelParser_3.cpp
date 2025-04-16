/*  XmlLabelParser_3.cpp
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
#include "XmlLabelParser_3.h"

#include "DataCache.h"
#include "Model.h"
#include "ModelBarcodeObject.h"
#include "ModelBoxObject.h"
#include "ModelEllipseObject.h"
#include "ModelImageObject.h"
#include "ModelLineObject.h"
#include "ModelObject.h"
#include "ModelTextObject.h"
#include "XmlTemplateParser.h"
#include "XmlUtil.h"
#include "Size.h"

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


namespace
{
	const uint32_t GDK_PIXBUF_MAGIC_NUMBER {0x47646b50};
	const double FONT_SCALE_FACTOR {0.75};

	typedef enum
	{
		/* colorspace + alpha */
		GDK_PIXDATA_COLOR_TYPE_RGB    = 0x01,
		GDK_PIXDATA_COLOR_TYPE_RGBA   = 0x02,
		GDK_PIXDATA_COLOR_TYPE_MASK   = 0xff,
		/* width, support 8bits only currently */
		GDK_PIXDATA_SAMPLE_WIDTH_8    = 0x01 << 16,
		GDK_PIXDATA_SAMPLE_WIDTH_MASK = 0x0f << 16,
		/* encoding */
		GDK_PIXDATA_ENCODING_RAW      = 0x01 << 24,
		GDK_PIXDATA_ENCODING_RLE      = 0x02 << 24,
		GDK_PIXDATA_ENCODING_MASK     = 0x0f << 24
	} GdkPixdataType;
}


namespace glabels
{

	namespace model
	{


		Model*
		XmlLabelParser_3::parseRootNode( const QDomElement &node )
		{
			auto* label = new Model();

			/* Pass 1, extract data nodes to pre-load cache. */
			DataCache data;
			for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
			{
				auto element = child.toElement();
				if (!element.isNull() && element.tagName() == "Data" )
				{
					parseDataNode( element, data );
				}
			}

			/* Pass 2, now extract everything else. */
			for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
			{
				const auto childElement = child.toElement();
				if(childElement.isNull())
				{
					qCritical()<<"Can't convert the node to an element. Try to continue.";
					continue;
				}
				const QString tagName = childElement.tagName();
		
				if ( tagName == "Template" )
				{
					Template* tmplate = XmlTemplateParser().parseTemplateNode( childElement );
					if ( tmplate == nullptr )
					{
						qWarning() << "Unable to parse template";
						delete label;
						return nullptr;
					}
					label->setTmplate( tmplate ); // Copies arg
					delete tmplate;
				}
				else if ( tagName == "Objects" )
				{
					label->setRotate( parseRotateAttr( childElement ) );
					QList<ModelObject*> list = parseObjectsNode( childElement, data );
					foreach ( ModelObject* object, list )
					{
						label->addObject( object );
					}
				}
				else if ( tagName == "Merge" )
				{
					parseMergeNode( childElement, label );
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

			label->clearModified();
			return label;
		}


		QList<ModelObject*>
		XmlLabelParser_3::parseObjectsNode( const QDomElement &node, const DataCache& data )
		{
			QList<ModelObject*> list;

			for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
			{
				const auto childElement = child.toElement();
				if(childElement.isNull())
				{
					qCritical()<<"Can't convert the node to an element. Try to continue.";
					continue;
				}
				const QString tagName = childElement.tagName();
		
				if ( tagName == "Object-box" )
				{
					list.append( parseObjectBoxNode( childElement ) );
				}
				else if ( tagName == "Object-ellipse" )
				{
					list.append( parseObjectEllipseNode( childElement ) );
				}
				else if ( tagName == "Object-line" )
				{
					list.append( parseObjectLineNode( childElement ) );
				}
				else if ( tagName == "Object-text" )
				{
					list.append( parseObjectTextNode( childElement ) );
				}
				else if ( tagName == "Object-image" )
				{
					list.append( parseObjectImageNode( childElement, data ) );
				}
				else if ( tagName == "Object-barcode" )
				{
					list.append( parseObjectBarcodeNode( childElement ) );
				}
				else if ( !child.isComment() )
				{
					qWarning() << "Unexpected" << node.tagName() << "child:" << tagName;
				}
			}

			return list;
		}


		ModelBoxObject*
		XmlLabelParser_3::parseObjectBoxNode( const QDomElement &node )
		{
			/* position attrs */
			const Distance x0 = XmlUtil::getLengthAttr( node, "x", 0.0 );
			const Distance y0 = XmlUtil::getLengthAttr( node, "y", 0.0 );

			/* size attrs */
			const Distance w = XmlUtil::getLengthAttr( node, "w", 0 );
			const Distance h = XmlUtil::getLengthAttr( node, "h", 0 );

			/* line attrs */
			const Distance lineWidth = XmlUtil::getLengthAttr( node, "line_width", 1.0 );

			QString  key        = XmlUtil::getStringAttr( node, "line_color_field", "" );
			bool     field_flag = !key.isEmpty();
			uint32_t color      = XmlUtil::getUIntAttr( node, "line_color", 0 );
			const ColorNode lineColorNode( field_flag, color, key );

			/* fill attrs */
			key        = XmlUtil::getStringAttr( node, "fill_color_field", "" );
			field_flag = !key.isEmpty();
			color      = XmlUtil::getUIntAttr( node, "fill_color", 0 );
			const ColorNode fillColorNode( field_flag, color, key );

			/* affine attrs */
			const auto affineTransformation = parseAffineTransformation(node);

			/* shadow attrs */
			const bool     shadowState   = XmlUtil::getBoolAttr( node, "shadow", false );
			const Distance shadowX       = XmlUtil::getLengthAttr( node, "shadow_x", 0.0 );
			const Distance shadowY       = XmlUtil::getLengthAttr( node, "shadow_y", 0.0 );
			const double   shadowOpacity = XmlUtil::getDoubleAttr( node, "shadow_opacity", 1.0 );

			key        = XmlUtil::getStringAttr( node, "shadow_color_field", "" );
			field_flag = !key.isEmpty();
			color      = XmlUtil::getUIntAttr( node, "shadow_color", 0 );
			const ColorNode shadowColorNode( field_flag, color, key );

			return new ModelBoxObject( x0, y0, w, h, false /*lockAspectRatio*/,
			                           lineWidth, lineColorNode,
			                           fillColorNode,
						   affineTransformation,
			                           shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );
		}


		ModelEllipseObject*
		XmlLabelParser_3::parseObjectEllipseNode( const QDomElement &node )
		{
			/* position attrs */
			const Distance x0 = XmlUtil::getLengthAttr( node, "x", 0.0 );
			const Distance y0 = XmlUtil::getLengthAttr( node, "y", 0.0 );

			/* size attrs */
			const Distance w = XmlUtil::getLengthAttr( node, "w", 0 );
			const Distance h = XmlUtil::getLengthAttr( node, "h", 0 );

			/* line attrs */
			const Distance lineWidth = XmlUtil::getLengthAttr( node, "line_width", 1.0 );

			QString  key        = XmlUtil::getStringAttr( node, "line_color_field", "" );
			bool     field_flag = !key.isEmpty();
			uint32_t color      = XmlUtil::getUIntAttr( node, "line_color", 0 );
			const ColorNode lineColorNode( field_flag, color, key );

			/* fill attrs */
			key        = XmlUtil::getStringAttr( node, "fill_color_field", "" );
			field_flag = !key.isEmpty();
			color      = XmlUtil::getUIntAttr( node, "fill_color", 0 );
			const ColorNode fillColorNode( field_flag, color, key );

			/* affine attrs */
			const auto affineTransformation = parseAffineTransformation(node);

			/* shadow attrs */
			const bool     shadowState   = XmlUtil::getBoolAttr( node, "shadow", false );
			const Distance shadowX       = XmlUtil::getLengthAttr( node, "shadow_x", 0.0 );
			const Distance shadowY       = XmlUtil::getLengthAttr( node, "shadow_y", 0.0 );
			const double   shadowOpacity = XmlUtil::getDoubleAttr( node, "shadow_opacity", 1.0 );

			key        = XmlUtil::getStringAttr( node, "shadow_color_field", "" );
			field_flag = !key.isEmpty();
			color      = XmlUtil::getUIntAttr( node, "shadow_color", 0 );
			const ColorNode shadowColorNode( field_flag, color, key );

			return new ModelEllipseObject( x0, y0, w, h, false /*lockAspectRatio*/,
			                               lineWidth, lineColorNode,
			                               fillColorNode,
						       affineTransformation,
			                               shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );
		}


		ModelLineObject*
		XmlLabelParser_3::parseObjectLineNode( const QDomElement &node )
		{
			/* position attrs */
			const Distance x0 = XmlUtil::getLengthAttr( node, "x", 0.0 );
			const Distance y0 = XmlUtil::getLengthAttr( node, "y", 0.0 );

			/* size attrs of line */
			const Distance dx = XmlUtil::getLengthAttr( node, "dx", 0 );
			const Distance dy = XmlUtil::getLengthAttr( node, "dy", 0 );

			/* line attrs */
			const Distance lineWidth = XmlUtil::getLengthAttr( node, "line_width", 1.0 );

			QString  key        = XmlUtil::getStringAttr( node, "line_color_field", "" );
			bool     field_flag = !key.isEmpty();
			uint32_t color      = XmlUtil::getUIntAttr( node, "line_color", 0 );
			const ColorNode lineColorNode( field_flag, color, key );

			/* affine attrs */
			const auto affineTransformation = parseAffineTransformation(node);

			/* shadow attrs */
			const bool     shadowState   = XmlUtil::getBoolAttr( node, "shadow", false );
			const Distance shadowX       = XmlUtil::getLengthAttr( node, "shadow_x", 0.0 );
			const Distance shadowY       = XmlUtil::getLengthAttr( node, "shadow_y", 0.0 );
			const double   shadowOpacity = XmlUtil::getDoubleAttr( node, "shadow_opacity", 1.0 );

			key        = XmlUtil::getStringAttr( node, "shadow_color_field", "" );
			field_flag = !key.isEmpty();
			color      = XmlUtil::getUIntAttr( node, "shadow_color", 0 );
			const ColorNode shadowColorNode( field_flag, color, key );

			return new ModelLineObject( x0, y0, dx, dy,
			                            lineWidth, lineColorNode,
						    affineTransformation,
			                            shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );
		}


		ModelImageObject*
		XmlLabelParser_3::parseObjectImageNode( const QDomElement &node, const DataCache& data )
		{
			/* position attrs */
			const Distance x0 = XmlUtil::getLengthAttr( node, "x", 0.0 );
			const Distance y0 = XmlUtil::getLengthAttr( node, "y", 0.0 );

			/* size attrs */
			const Distance w = XmlUtil::getLengthAttr( node, "w", 0 );
			const Distance h = XmlUtil::getLengthAttr( node, "h", 0 );

			/* file attrs */
			QString key        = XmlUtil::getStringAttr( node, "field", "" );
			bool    field_flag = !key.isEmpty();
			const QString filename   = XmlUtil::getStringAttr( node, "src", "" );
			const TextNode filenameNode( field_flag, field_flag ? key : filename );

			/* affine attrs */
			const auto affineTransformation = parseAffineTransformation(node);

			/* shadow attrs */
			const bool     shadowState   = XmlUtil::getBoolAttr( node, "shadow", false );
			const Distance shadowX       = XmlUtil::getLengthAttr( node, "shadow_x", 0.0 );
			const Distance shadowY       = XmlUtil::getLengthAttr( node, "shadow_y", 0.0 );
			const double   shadowOpacity = XmlUtil::getDoubleAttr( node, "shadow_opacity", 1.0 );

			key            = XmlUtil::getStringAttr( node, "shadow_color_field", "" );
			field_flag     = !key.isEmpty();
			uint32_t color = XmlUtil::getUIntAttr( node, "shadow_color", 0 );
			const ColorNode shadowColorNode( field_flag, color, key );

			if ( filenameNode.isField() )
			{
				return new ModelImageObject( x0, y0, w, h, false /*lockAspectRatio*/,
				                             filenameNode,
							     affineTransformation,
				                             shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );
			}
			if ( data.hasImage( filename ) )
			{
				return new ModelImageObject( x0, y0, w, h, false /*lockAspectRatio*/,
							     filename, data.getImage( filename ),
							     affineTransformation,
							     shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );
			}
			if ( data.hasSvg( filename ) )
			{
				return new ModelImageObject( x0, y0, w, h, false /*lockAspectRatio*/,
							     filename, data.getSvg( filename ),
							     affineTransformation,
							     shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );
			}
			qWarning() << "Embedded file" << filename << "missing. Trying actual file.";
			return new ModelImageObject( x0, y0, w, h, false /*lockAspectRatio*/,
						     filenameNode,
						     affineTransformation,
						     shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );
		}


		ModelBarcodeObject*
		XmlLabelParser_3::parseObjectBarcodeNode( const QDomElement &node )
		{
			/* position attrs */
			const Distance x0 = XmlUtil::getLengthAttr( node, "x", 0.0 );
			const Distance y0 = XmlUtil::getLengthAttr( node, "y", 0.0 );

			/* size attrs */
			const Distance w = XmlUtil::getLengthAttr( node, "w", 0 );
			const Distance h = XmlUtil::getLengthAttr( node, "h", 0 );

			/* barcode attrs */
			auto backend = XmlUtil::getStringAttr( node, "backend", "" );
			// one major difference between glabels-3.0.dtd and glabels-4.0.dtd
			// is the lowercase of the style names
			auto style = XmlUtil::getStringAttr( node, "style", "" ).toLower();

			if ( backend == "built-in" )
			{
				backend = "";
			}
			else if ( backend == "libiec16022" )
			{
				backend = "";
				style = "datamatrix";
			}
			else if ( backend == "libqrencode" )
			{
				if ( barcode::Backends::style( "qrencode", "qrcode" ) != barcode::Backends::defaultStyle() )
				{
					backend = "qrencode";
					style = "qrcode";
				}
				else
				{
					// Will use defaultStyle if Zint not available
					backend = "zint";
					style = "qr";
				}
			}

			const barcode::Style bcStyle = barcode::Backends::style( backend, style );
			const bool bcTextFlag = XmlUtil::getBoolAttr( node, "text", true );
			const bool bcChecksumFlag = XmlUtil::getBoolAttr( node, "checksum", true );

			const QString  key        = XmlUtil::getStringAttr( node, "color_field", "" );
			const bool     field_flag = !key.isEmpty();
			const uint32_t color      = XmlUtil::getUIntAttr( node, "color", 0 );
			const ColorNode bcColorNode( field_flag, color, key );

			QString bcData = XmlUtil::getStringAttr( node, "data", "" );
			if(bcData.isEmpty())
			{
				bcData = "${" + XmlUtil::getStringAttr( node, "field", "" ) + "}";
			}

			/* affine attrs */
			const auto affineTransformation = parseAffineTransformation(node);

			return new ModelBarcodeObject( x0, y0, w, h, false /*lockAspectRatio*/,
			                               bcStyle, bcTextFlag, bcChecksumFlag, bcData, bcColorNode,
						       affineTransformation );
		}


		QTransform
		XmlLabelParser_3::parseAffineTransformation(const QDomElement &node)
		{
			return {XmlUtil::getDoubleAttr( node, "a0", 1.0 ),
				XmlUtil::getDoubleAttr( node, "a1", 0.0 ),
				XmlUtil::getDoubleAttr( node, "a2", 0.0 ),
				XmlUtil::getDoubleAttr( node, "a3", 1.0 ),
				XmlUtil::getDoubleAttr( node, "a4", 0.0 ),
				XmlUtil::getDoubleAttr( node, "a5", 0.0 )};
		}


		ModelTextObject*
		XmlLabelParser_3::parseObjectTextNode( const QDomElement &node )
		{
			/* position attrs */
			const Distance x0 = XmlUtil::getLengthAttr( node, "x", 0.0 );
			const Distance y0 = XmlUtil::getLengthAttr( node, "y", 0.0 );

			/* size attrs */
			const Distance w = XmlUtil::getLengthAttr( node, "w", 0 );
			const Distance h = XmlUtil::getLengthAttr( node, "h", 0 );

			/* justify attr */
			const Qt::Alignment textHAlign = getHAlignmentAttr( node, "justify", Qt::AlignLeft );

			/* valign attr */
			const Qt::Alignment textVAlign = getVAlignmentAttr( node, "valign", Qt::AlignTop );

			/* auto_shrink attr */
			const bool textAutoShrink = XmlUtil::getBoolAttr( node, "auto_shrink", false );

			/* affine attrs */
			const auto affineTransformation = parseAffineTransformation(node);

			/* shadow attrs */
			const bool     shadowState   = XmlUtil::getBoolAttr( node, "shadow", false );
			const Distance shadowX       = XmlUtil::getLengthAttr( node, "shadow_x", 0.0 );
			const Distance shadowY       = XmlUtil::getLengthAttr( node, "shadow_y", 0.0 );
			const double   shadowOpacity = XmlUtil::getDoubleAttr( node, "shadow_opacity", 1.0 );

			QString key            = XmlUtil::getStringAttr( node, "shadow_color_field", "" );
			bool field_flag        = !key.isEmpty();
			uint32_t color         = XmlUtil::getUIntAttr( node, "shadow_color", 0 );
			const ColorNode shadowColorNode( field_flag, color, key );

			/* font attrs */
			QString       fontFamily = "Sans";
			double        fontSize = 10.;
			QFont::Weight fontWeight = QFont::Normal;
			bool          fontItalicFlag = false;

			/* text attrs */
			double textLineSpacing = 1.;
			QTextOption::WrapMode textWrapMode = QTextOption::WordWrap;

			ColorNode textColorNode;

			/* deserialize contents. */
			QTextDocument document;
			QTextCursor cursor( &document );
			bool firstBlock = true;
			for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
			{
				const auto element = child.toElement();
				if(element.isNull())
				{
					qCritical()<<"Can't convert the node to an element. Try to continue.";
					continue;
				}

				if(element.tagName() == "Span")
				{
					QString text;
					for(QDomNode textPartElement = element.firstChild()
					    ; !textPartElement.isNull()
					    ; textPartElement = textPartElement.nextSibling())
					{
						const QDomText textpart = textPartElement.toText();
						if (!textpart.isNull())
						{
							text += textpart.data();
						}
						else
						{
							if(textPartElement.toElement().tagName() == "NL")
							{
								text += "\n";
							}
							else if (textPartElement.toElement().tagName() == "Field")
							{
								text += "${" + XmlUtil::getStringAttr( textPartElement.toElement(), "name", "" ) + "}";
							}
						}
					}

					if ( !firstBlock )
					{
						cursor.insertBlock();
					}
					firstBlock = false;

					cursor.insertText( text );

					/* font attrs */
					fontFamily        = XmlUtil::getStringAttr( element, "font_family", "Sans" );
					fontSize          = XmlUtil::getDoubleAttr( element, "font_size", 10 ) * FONT_SCALE_FACTOR;
					fontWeight        = getWeightAttr( element, "font_weight", QFont::Normal );
					fontItalicFlag    = XmlUtil::getBoolAttr( element, "font_italic", false );

					/* color attr */
					key        = XmlUtil::getStringAttr( element, "color_field", "" );
					field_flag = !key.isEmpty();
					color      = XmlUtil::getUIntAttr( element, "color", 0 );

					textColorNode = ColorNode(field_flag, color, key );

					/* text attrs */
					textLineSpacing   = XmlUtil::getDoubleAttr( element, "line_spacing", 1 );
					textWrapMode = QTextOption::WordWrap;

				}
				else if ( !child.isComment() )
				{
					qWarning() << "Unexpected" << node.tagName() << "child:" << node.tagName();
				}
			}
			const QString text = document.toPlainText();

			auto textNode = new ModelTextObject( x0, y0, w, h, false /*lockAspectRatio*/, text,
							     fontFamily, fontSize, fontWeight, fontItalicFlag, false,
							     textColorNode, textHAlign, textVAlign, textWrapMode, textLineSpacing,
							     textAutoShrink,
							     affineTransformation,
							     shadowState, shadowX, shadowY, shadowOpacity, shadowColorNode );

			// The size of the textnode does not fit the qt world. So it's needed to
			// recalculate the size depending on the data.
			textNode->setSize(textNode->naturalSize());

			return textNode;
		}


		bool
		XmlLabelParser_3::parseRotateAttr( const QDomElement &node )
		{
			return XmlUtil::getBoolAttr( node, "rotate", false );
		}


		void
		XmlLabelParser_3::parseMergeNode( const QDomElement &node, Model* label )
		{
			const QString type = XmlUtil::getStringAttr( node, "type", "None" );
			const QString src  = XmlUtil::getStringAttr( node, "src", "" );

			merge::Merge* merge = merge::Factory::createMerge( type );
			merge->setSource( src );

			label->setMerge( merge );
		}


		void
		XmlLabelParser_3::parseDataNode( const QDomElement &node, DataCache& data )
		{
			for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
			{
				const auto childElement = child.toElement();
				if(childElement.isNull())
				{
					qCritical()<<"Can't convert the node to an element. Try to continue.";
					continue;
				}
				const QString tagName = childElement.tagName();

				if ( tagName == "File" )
				{
					parseFileNode( childElement, data );
				}
				else if (tagName == "Pixdata")
				{
					parsePixdataNode( childElement, data);
				}
				else if ( !child.isComment() )
				{
					qWarning() << "Unexpected" << node.tagName() << "child:" << tagName;
				}
			}
		}


		void
		XmlLabelParser_3::parsePixdataNode( const QDomElement& node, DataCache& data )
		{
			const QString name     = XmlUtil::getStringAttr( node, "name", "" );
			const QString encoding = XmlUtil::getStringAttr( node, "encoding", "base64" );

			/*
			 * Struct of the GdkPixdata from the header file
			 */
			struct _GdkPixdata
			{
				uint32_t magic;        /* GDK_PIXBUF_MAGIC_NUMBER */
				int32_t  length;       /* <1 to disable length checks, otherwise:
							* GDK_PIXDATA_HEADER_LENGTH + pixel_data length*/
				uint32_t pixdata_type; /* GdkPixdataType */
				uint32_t rowstride;
				uint32_t width;
				uint32_t height;
				//uint8_t *pixel_data;
			} mypixdata{};

			if ( encoding.toLower() == "base64" )
			{

				QByteArray ba = QByteArray::fromBase64( node.text().toUtf8() );

				// Use the QDataStream to import the header cause it is in big endian
				QDataStream ds(ba);
				ds.setByteOrder(QDataStream::ByteOrder::BigEndian);

				ds >> mypixdata.magic
				   >> mypixdata.length
				   >> mypixdata.pixdata_type
				   >> mypixdata.rowstride
				   >> mypixdata.width
				   >> mypixdata.height;

				if(mypixdata.magic !=GDK_PIXBUF_MAGIC_NUMBER)
				{
					qCritical() << "GDK pixbuf magic is not correct. Abort reading of pixdata. "
						    << "Node:" << node.tagName();
					return;
				}

				// check if the data fits in a sigend int
				if(mypixdata.width > INT32_MAX || mypixdata.height > INT32_MAX || mypixdata.rowstride > INT32_MAX)
				{
					qCritical() << "rowstride, width or height is to large. Abort reading of pixdata. "
						    << "Node:" << node.tagName();;
					return;
				}

				const auto width = static_cast<int32_t>(mypixdata.width);
				const auto height = static_cast<int32_t>(mypixdata.height);
				const auto rowstride = static_cast<int32_t>(mypixdata.rowstride);

				QImage::Format pixformat;
				int32_t rawpadding;
				if((mypixdata.pixdata_type & GDK_PIXDATA_COLOR_TYPE_MASK) == GDK_PIXDATA_COLOR_TYPE_RGB)
				{
					pixformat = QImage::Format_RGB888;
					rawpadding = rowstride - (3 * width);
				}
				else if((mypixdata.pixdata_type & GDK_PIXDATA_COLOR_TYPE_MASK) == GDK_PIXDATA_COLOR_TYPE_RGBA)
				{
					pixformat = QImage::Format_RGBA8888;
					rawpadding = rowstride - (4 * width);
				}
				else
				{
					qCritical() << "pixdata color type is unknown. Abort reading of pixdata. "
						    << "Node:" << node.tagName();
					return;
				}

				if(rawpadding < 0){
					qCritical() << "padding to is negativ. Abort reading of pixdata. "
						    << "Node:" << node.tagName();
					return;
				}

				QImage image(width, height, pixformat);

				const auto padding = static_cast<int32_t>(rawpadding);

				int x = 0;
				int y = 0;
				uint8_t r,g,b,a;
				for(y = 0; y < height; y++)
				{
					for(x = 0; x < width; x++)
					{
						if(pixformat == QImage::Format_RGB888)
						{
							ds >> r >> g >> b;
							image.setPixelColor(x, y, QColor(r, g, b));
						}
						else
						{
							ds >> r >> g >> b >> a;
							image.setPixelColor(x, y, QColor(r, g, b, a));
						}

					}
					ds.skipRawData(padding);
				}

				data.addImage( name, image );
			}
			else
			{
				qWarning() << "Unexpected encoding:" << encoding << "node:" << node.tagName();
			}
		}


		void
		XmlLabelParser_3::parseFileNode( const QDomElement& node, DataCache& data )
		{
			const QString name     = XmlUtil::getStringAttr( node, "name", "" );
			const QString format = XmlUtil::getStringAttr( node, "format", "invalid" );

			if ( format == "SVG" )
			{
				data.addSvg( name, node.text().toUtf8() );
			}
			else
			{
				qCritical() << "Unknown embedded file format:" << format;
			}
		}


		Qt::Alignment
		XmlLabelParser_3::getHAlignmentAttr( const QDomElement& node, const QString& name,
						const Qt::Alignment default_value )
		{

			const QString valueString = node.attribute( name, "" );
			if ( !valueString.isEmpty())
			{
				if ( valueString == "Right" )
				{
					return Qt::AlignRight;
				}
				if ( valueString == "Center")
				{
					return Qt::AlignHCenter;
				}
				if ( valueString == "Left" )
				{
					return Qt::AlignLeft;
				}
			}
			return default_value;
		}


		Qt::Alignment
		XmlLabelParser_3::getVAlignmentAttr( const QDomElement& node, const QString& name,
						     const Qt::Alignment default_value )
		{

			const QString valueString = node.attribute( name, "" );
			if ( ! valueString.isEmpty() )
			{
				if ( valueString == "Bottom" )
				{
					return Qt::AlignBottom;
				}
				if ( valueString == "Center" )
				{
					return Qt::AlignVCenter;
				}
				if ( valueString == "Top" )
				{
					return Qt::AlignTop;
				}
			}

			return default_value;
		}


		QFont::Weight
		XmlLabelParser_3::getWeightAttr( const QDomElement& node, const QString&     name,
						 const QFont::Weight default_value )
		{
			const QString valueString = node.attribute( name, "" );
			if ( !valueString.isEmpty() )
			{
				if ( valueString == "Bold" )
				{
					return QFont::Bold;
				}
				if ( valueString == "Regular" )
				{
					return QFont::Normal;
				}
			}

			return default_value;
		}


	}  // namespace model

}  // namespace glabels
