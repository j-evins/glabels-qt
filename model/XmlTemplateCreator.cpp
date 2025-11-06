/*  XmlTemplateCreator.cpp
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

#include "XmlTemplateCreator.h"

#include "Db.h"
#include "Template.h"
#include "XmlUtil.h"

#include <QDomNode>
#include <QFile>
#include <QtDebug>


namespace glabels
{
	namespace model
	{

		bool XmlTemplateCreator::writeTemplates( const QList<Template> tmplates, const QString &fileName )
		{
			QDomDocument doc( "Glabels-templates" );
			QDomElement root = doc.createElement( "Glabels-templates" );
			doc.appendChild( root );

			for ( auto& tmplate : tmplates )
			{
				createTemplateNode( root, tmplate );
			}

			QFile file( fileName );

			if ( !file.open( QFile::WriteOnly | QFile::Text) )
			{
				qWarning() << "Error: Cannot open file " << fileName
				           << ": " << file.errorString();
				return false;
			}


			if ( file.write( doc.toByteArray() ) < 0 )
			{
				qWarning() << "Error: Cannot write file " << fileName
				           << ": " << file.errorString();
				return false;
			}
			
			return true;
		}


		bool XmlTemplateCreator::writeTemplate( const Template& tmplate, const QString& fileName )
		{
			QList<Template> tmplates;

			tmplates.append(tmplate);

			return writeTemplates( tmplates, fileName );
		}


		void XmlTemplateCreator::createTemplateNode( QDomElement &parent, const Template& tmplate )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Template" );
			parent.appendChild( node );

			XmlUtil::setStringAttr( node, "brand", tmplate.brand() );
			XmlUtil::setStringAttr( node, "part", tmplate.part() );

			XmlUtil::setStringAttr( node, "size", tmplate.paperId() );
			if ( tmplate.isSizeOther() )
			{
				XmlUtil::setLengthAttr( node, "width", tmplate.pageWidth() );
				XmlUtil::setLengthAttr( node, "height", tmplate.pageHeight() );
			}
			if ( tmplate.isRoll() )
			{
				XmlUtil::setLengthAttr( node, "roll_width", tmplate.rollWidth() );
			}

			XmlUtil::setStringAttr( node, "description", tmplate.description() );

			if ( !tmplate.productUrl().isEmpty() )
			{
				createMetaNode( node, "product_url", tmplate.productUrl() );
			}
#if TODO
			for ( auto& categoryId : tmplate.categoryIds() )
			{
				createMetaNode( node, "category", categoryId );
			}
#endif

			if ( auto frame = tmplate.frame() )
			{
				createLabelNode( node, frame );
			}
		}


		void XmlTemplateCreator::createMetaNode( QDomElement &parent, const QString& attr, const QString& value )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Meta" );
			parent.appendChild( node );

			XmlUtil::setStringAttr( node, attr, value );
		}


		void XmlTemplateCreator::createLabelNode( QDomElement &parent, const Frame* frame )
		{
			if ( const auto* frameRect = dynamic_cast<const FrameRect*>(frame) )
			{
				createLabelRectangleNode( parent, frameRect );
			}
			else if ( const auto* frameEllipse = dynamic_cast<const FrameEllipse*>(frame) )
			{
				createLabelEllipseNode( parent, frameEllipse );
			}
			else if ( const auto* frameRound = dynamic_cast<const FrameRound*>(frame) )
			{
				createLabelRoundNode( parent, frameRound );
			}
			else if ( const auto* frameCd = dynamic_cast<const FrameCd*>(frame) )
			{
				createLabelCdNode( parent, frameCd );
			}
			else if ( const auto* framePath = dynamic_cast<const FramePath*>(frame) )
			{
				createLabelPathNode( parent, framePath );
			}
			else if ( const auto* frameContinuous = dynamic_cast<const FrameContinuous*>(frame) )
			{
				createLabelContinuousNode( parent, frameContinuous );
			}
			else
			{
				Q_ASSERT_X( false, "XmlTemplateCreator::createLabelNode", "Invalid frame type." );
			}
		}


		void XmlTemplateCreator::createLabelRectangleNode( QDomElement &parent, const FrameRect* frame )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Label-rectangle" );
			parent.appendChild( node );

			XmlUtil::setStringAttr( node, "id",      frame->id() );
			XmlUtil::setLengthAttr( node, "width",   frame->w() );
			XmlUtil::setLengthAttr( node, "height",  frame->h() );
			XmlUtil::setLengthAttr( node, "round",   frame->r() );
			XmlUtil::setLengthAttr( node, "x_waste", frame->xWaste() );
			XmlUtil::setLengthAttr( node, "y_waste", frame->yWaste() );

			createLabelNodeCommon( node, frame );
		}


		void XmlTemplateCreator::createLabelEllipseNode( QDomElement &parent, const FrameEllipse* frame )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Label-ellipse" );
			parent.appendChild( node );

			XmlUtil::setStringAttr( node, "id",      frame->id() );
			XmlUtil::setLengthAttr( node, "width",   frame->w() );
			XmlUtil::setLengthAttr( node, "height",  frame->h() );
			XmlUtil::setLengthAttr( node, "waste",   frame->waste() );

			createLabelNodeCommon( node, frame );
		}


		void XmlTemplateCreator::createLabelRoundNode( QDomElement &parent, const FrameRound* frame )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Label-round" );
			parent.appendChild( node );

			XmlUtil::setStringAttr( node, "id",      frame->id() );
			XmlUtil::setLengthAttr( node, "radius",  frame->r() );
			XmlUtil::setLengthAttr( node, "waste",   frame->waste() );

			createLabelNodeCommon( node, frame );
		}


		void XmlTemplateCreator::createLabelCdNode( QDomElement &parent, const FrameCd* frame )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Label-cd" );
			parent.appendChild( node );

			XmlUtil::setStringAttr( node, "id",      frame->id() );
			XmlUtil::setLengthAttr( node, "radius",  frame->r1() );
			XmlUtil::setLengthAttr( node, "hole",    frame->r2() );
			XmlUtil::setLengthAttr( node, "waste",   frame->waste() );
			if ( frame->w() != Distance(0) )
			{
				XmlUtil::setLengthAttr( node, "width",   frame->w() );
			}
			if ( frame->h() != Distance(0) )
			{
				XmlUtil::setLengthAttr( node, "height",  frame->h() );
			}

			createLabelNodeCommon( node, frame );
		}


		void XmlTemplateCreator::createLabelPathNode( QDomElement &parent, const FramePath* frame )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Label-path" );
			parent.appendChild( node );

			XmlUtil::setStringAttr(   node, "id",      frame->id() );
			XmlUtil::setLengthAttr(   node, "x_waste", frame->xWaste() );
			XmlUtil::setLengthAttr(   node, "y_waste", frame->yWaste() );
			XmlUtil::setUnitsAttr(    node, "d_units", frame->originalUnits() );
			XmlUtil::setPathDataAttr( node, "d",       frame->path(), frame->originalUnits() );

			createLabelNodeCommon( node, frame );
		}


		void XmlTemplateCreator::createLabelContinuousNode( QDomElement &parent, const FrameContinuous* frame )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Label-continuous" );
			parent.appendChild( node );

			XmlUtil::setStringAttr( node, "id",             frame->id() );
			XmlUtil::setLengthAttr( node, "width",          frame->w() );
			XmlUtil::setLengthAttr( node, "height",         frame->h() );
			XmlUtil::setLengthAttr( node, "min_height",     frame->hMin() );
			XmlUtil::setLengthAttr( node, "max_height",     frame->hMin() );
			XmlUtil::setLengthAttr( node, "default_height", frame->hDefault() );

			createLabelNodeCommon( node, frame );
		}


		void XmlTemplateCreator::createLabelNodeCommon( QDomElement &node, const Frame *frame )
		{
			for ( auto& markup : frame->markups() )
			{
				if ( auto* markupMargin = dynamic_cast<MarkupMargin*>(markup.get()) )
				{
					createMarkupMarginNode( node, markupMargin );
				}
				else if ( auto* markupLine = dynamic_cast<MarkupLine*>(markup.get()) )
				{
					createMarkupLineNode( node, markupLine );
				}
				else if ( auto* markupCircle = dynamic_cast<MarkupCircle*>(markup.get()) )
				{
					createMarkupCircleNode( node, markupCircle );
				}
				else if ( auto* markupRect = dynamic_cast<MarkupRect*>(markup.get()) )
				{
					createMarkupRectNode( node, markupRect );
				}
				else if ( auto* markupEllipse = dynamic_cast<MarkupEllipse*>(markup.get()) )
				{
					createMarkupEllipseNode( node, markupEllipse );
				}
				else
				{
					Q_ASSERT_X( false, "XmlTemplateCreator::createLabelNodeCommon", "Invalid markup type." );
				}
			}

			for ( auto& layout : frame->layouts() )
			{
				createLayoutNode( node, layout );
			}
		}


		void XmlTemplateCreator::createLayoutNode( QDomElement& parent, const Layout& layout )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Layout" );
			parent.appendChild( node );

			XmlUtil::setIntAttr( node, "nx", layout.nx() );
			XmlUtil::setIntAttr( node, "ny", layout.ny() );

			XmlUtil::setLengthAttr( node, "x0", layout.x0() );
			XmlUtil::setLengthAttr( node, "y0", layout.y0() );

			XmlUtil::setLengthAttr( node, "dx", layout.dx() );
			XmlUtil::setLengthAttr( node, "dy", layout.dy() );
		}


		void XmlTemplateCreator::createMarkupMarginNode( QDomElement& parent, const MarkupMargin* markup )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Markup-margin" );
			parent.appendChild( node );

			if ( markup->xSize() == markup->ySize() )
			{
				XmlUtil::setLengthAttr( node, "size", markup->xSize() );
			}
			else
			{
				XmlUtil::setLengthAttr( node, "x_size", markup->xSize() );
				XmlUtil::setLengthAttr( node, "y_size", markup->ySize() );
			}
		}


		void XmlTemplateCreator::createMarkupLineNode( QDomElement& parent, const MarkupLine* markup )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Markup-line" );
			parent.appendChild( node );

			XmlUtil::setLengthAttr( node, "x1", markup->x1() );
			XmlUtil::setLengthAttr( node, "y1", markup->y1() );
			XmlUtil::setLengthAttr( node, "x2", markup->x2() );
			XmlUtil::setLengthAttr( node, "y2", markup->y2() );
		}


		void XmlTemplateCreator::createMarkupCircleNode( QDomElement& parent, const MarkupCircle* markup )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Markup-circle" );
			parent.appendChild( node );

			XmlUtil::setLengthAttr( node, "x0", markup->x0() );
			XmlUtil::setLengthAttr( node, "y0", markup->y0() );
			XmlUtil::setLengthAttr( node, "radius", markup->r() );
		}


		void XmlTemplateCreator::createMarkupRectNode( QDomElement& parent, const MarkupRect* markup )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Markup-rect" );
			parent.appendChild( node );

			XmlUtil::setLengthAttr( node, "x1", markup->x1() );
			XmlUtil::setLengthAttr( node, "y1", markup->y1() );
			XmlUtil::setLengthAttr( node, "w",  markup->w() );
			XmlUtil::setLengthAttr( node, "h",  markup->h() );
			XmlUtil::setLengthAttr( node, "r",  markup->r() );
		}


		void XmlTemplateCreator::createMarkupEllipseNode( QDomElement& parent, const MarkupEllipse* markup )
		{
			QDomDocument doc = parent.ownerDocument();
			QDomElement node = doc.createElement( "Markup-ellipse" );
			parent.appendChild( node );

			XmlUtil::setLengthAttr( node, "x1", markup->x1() );
			XmlUtil::setLengthAttr( node, "y1", markup->y1() );
			XmlUtil::setLengthAttr( node, "w",  markup->w() );
			XmlUtil::setLengthAttr( node, "h",  markup->h() );
		}


	}
}
