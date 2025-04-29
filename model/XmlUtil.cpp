/*  XmlUtil.cpp
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

#include "XmlUtil.h"

#include <QTextStream>
#include <QCoreApplication>
#include <QDebug>
#include <QIODevice>


namespace glabels
{
	namespace model
	{

		//
		// Static data
		//
		XmlUtil* XmlUtil::mInstance = nullptr;


		XmlUtil::XmlUtil()
		{
			mUnits = Units(Units::PT);
		}


		void XmlUtil::init()
		{
			if ( mInstance == nullptr )
			{
				mInstance = new XmlUtil();
			}
		}


		Units XmlUtil::units()
		{
			init();

			return mInstance->mUnits;
		}


		void XmlUtil::setUnits( const Units& units )
		{
			init();

			mInstance->mUnits = units;
		}


		QString XmlUtil::getStringAttr( const QDomElement& node,
		                                const QString&     name,
		                                const QString&     default_value )
		{
			init();

			return node.attribute( name, default_value );
		}


		double XmlUtil::getDoubleAttr( const QDomElement& node,
		                               const QString&     name,
		                               double             default_value )
		{
			init();

			QString valueString = node.attribute( name, "" );
			if ( valueString != "" )
			{
				bool ok;
				double value = valueString.toDouble( &ok );

				if ( !ok )
				{
					qWarning() << "Error: bad double value in attribute "
					           << node.tagName() << ":" << name << "=" << valueString;
					return default_value;
				}

				return value;
			}

			return default_value;
		}


		bool XmlUtil::getBoolAttr( const QDomElement& node,
		                           const QString&     name,
		                           bool               default_value )
		{
			init();

			QString valueString = node.attribute( name, "" );
			if ( valueString != "" )
			{
				bool ok;
				int intValue = valueString.toInt( &ok );

				if ( (valueString == "True") ||
				     (valueString == "TRUE") ||
				     (valueString == "true") ||
				     (ok && (intValue == 1) ) )
				{
					return true;
				}

				if ( (valueString == "False") ||
				     (valueString == "FALSE") ||
				     (valueString == "false") ||
				     (ok && (intValue == 0) ) )
				{
					return false;
				}

				qWarning() << "Error: bad boolean value in attribute "
				           << node.tagName() << ":" << name << "=" << valueString;
				return default_value;
			}

			return default_value;
		}


		int XmlUtil::getIntAttr( const QDomElement& node,
		                         const QString&     name,
		                         int                default_value )
		{
			init();

			QString valueString = node.attribute( name, "" );
			if ( valueString != "" )
			{
				bool ok;
				int value = valueString.toInt( &ok );

				if ( !ok )
				{
					qWarning() << "Error: bad integer value in attribute "
					           << node.tagName() << ":" << name << "=" << valueString;
					return default_value;
				}

				return value;
			}

			return default_value;
		}


		uint32_t XmlUtil::getUIntAttr( const QDomElement& node,
		                               const QString&     name,
		                               uint32_t           default_value )
		{
			init();

			QString valueString = node.attribute( name, "" );
			if ( valueString != "" )
			{
				bool ok;
				uint32_t value = valueString.toUInt( &ok, 0 );

				if ( !ok )
				{
					qWarning() << "Error: bad unsigned integer value in attribute "
					           << node.tagName() << ":" << name << "=" << valueString;
					return default_value;
				}

				return value;
			}

			return default_value;
		}


		QString  XmlUtil::getI18nAttr( const QDomElement& node,
		                               const QString&     name,
		                               const QString&     default_value )
		{
			init();

			QString i18nString = node.attribute( QString("_").append(name), "" );

			if ( i18nString == "" )
			{
				return node.attribute( name, default_value );
			}

			return QCoreApplication::translate( "XmlStrings", i18nString.toUtf8().constData() );
		}


		Distance XmlUtil::getLengthAttr( const QDomElement& node,
		                                 const QString&     name,
		                                 const Distance&    default_value )
		{
			init();

			QString valueString = node.attribute( name, "" );
			if ( valueString != "" )
			{
				double value;
				QString unitsString;
				QTextStream valueStream( &valueString, QIODevice::ReadOnly );

				valueStream >> value >> unitsString;

				if ( !unitsString.isEmpty() && !Units::isIdValid( unitsString ) )
				{
					qWarning() << "Error: bad length value in attribute "
					           << node.tagName() << ":" <<  name << "=" << valueString;
					return default_value;
				}

				return Distance( value, unitsString );
			}

			return default_value;
		}


		QFont::Weight XmlUtil::getWeightAttr( const QDomElement& node,
		                                      const QString&     name,
		                                      QFont::Weight      default_value )
		{
			init();

			QString valueString = node.attribute( name, "" );
			if ( valueString != "" )
			{
				if ( valueString == "bold" )
				{
					return QFont::Bold;
				}
				else if ( valueString == "normal" )
				{
					return QFont::Normal;
				}
				else
				{
					qWarning() << "Error: bad weight value in attribute "
					           << node.tagName() << ":" <<  name << "=" << valueString;
					return default_value;
				}
			}

			return default_value;
		}


		Qt::Alignment XmlUtil::getAlignmentAttr( const QDomElement& node,
		                                         const QString&     name,
		                                         Qt::Alignment      default_value )
		{
			init();

			QString valueString = node.attribute( name, "" );
			if ( valueString != "" )
			{
				if ( valueString == "right" )
				{
					return Qt::AlignRight;
				}
				else if ( valueString == "hcenter" )
				{
					return Qt::AlignHCenter;
				}
				else if ( valueString == "left" )
				{
					return Qt::AlignLeft;
				}
				else if ( valueString == "bottom" )
				{
					return Qt::AlignBottom;
				}
				else if ( valueString == "vcenter" )
				{
					return Qt::AlignVCenter;
				}
				else if ( valueString == "top" )
				{
					return Qt::AlignTop;
				}
				else
				{
					qWarning() << "Error: bad alignment value in attribute "
					           << node.tagName() << ":" <<  name << "=" << valueString;
					return default_value;
				}
			}

			return default_value;
		}


		QTextOption::WrapMode XmlUtil::getWrapModeAttr( const QDomElement&    node,
		                                                const QString&        name,
		                                                QTextOption::WrapMode default_value )
		{
			init();

			QString valueString = node.attribute( name, "" );
			if ( valueString != "" )
			{
				if ( valueString == "word" )
				{
					return QTextOption::WordWrap;
				}
				else if ( valueString == "anywhere" )
				{
					return QTextOption::WrapAnywhere;
				}
				else if ( valueString == "none" )
				{
					return QTextOption::NoWrap;
				}
				else
				{
					qWarning() << "Error: bad wrap mode value in attribute "
					           << node.tagName() << ":" <<  name << "=" << valueString;
					return default_value;
				}
			}

			return default_value;
		}


		Units XmlUtil::getUnitsAttr( const QDomElement&    node,
		                             const QString&        name,
		                             const Units&          default_value )
		{
			init();

			QString valueString = node.attribute( name, "" );
			if ( valueString != "" )
			{
				return Units( valueString );
			}

			return default_value;
		}


		QPainterPath XmlUtil::getPathDataAttr( const QDomElement&    node,
		                                       const QString&        name,
		                                       const Units&          units )
		{
			init();

			QPainterPath d;

			//
			// Simple path data parser
			//
			QStringList tokens = node.attribute( name, "" ).split( " ", Qt::SkipEmptyParts );

			enum { CMD, MX, MY, MDX, MDY, LX, LY, LDX, LDY, HX, HDX, VY, VDY } state = CMD;
			Distance x  = 0;
			Distance y  = 0;
			Distance dx = 0;
			Distance dy = 0;
			QPointF  c;

			for ( int i = 0; i < tokens.size(); i++ )
			{
				switch (state)
				{
				case CMD:
					switch (tokens[i][0].unicode())
					{
					case 'M':
						state = MX;
						break;
					case 'm':
						state = MDX;
						break;
					case 'L':
						state = LX;
						break;
					case 'l':
						state = LDX;
						break;
					case 'H':
						state = HX;
						break;
					case 'h':
						state = HDX;
						break;
					case 'V':
						state = VY;
						break;
					case 'v':
						state = VDY;
						break;
					case 'Z':
					case 'z':
						d.closeSubpath();
						state = CMD;
						break;
					}
					break;
				case MX:
					x = Distance( tokens[i].toDouble(), units );
					state = MY;
					break;
				case MY:
					y = Distance( tokens[i].toDouble(), units );
					d.moveTo( x.pt(), y.pt() );
					state = CMD;
					break;
				case MDX:
					dx = Distance( tokens[i].toDouble(), units );
					state = MDY;
					break;
				case MDY:
					dy = Distance( tokens[i].toDouble(), units );
					c = d.currentPosition();
					d.moveTo( c.x()+x.pt(), c.y()+y.pt() );
					state = CMD;
					break;
				case LX:
					x = Distance( tokens[i].toDouble(), units );
					state = LY;
					break;
				case LY:
					y = Distance( tokens[i].toDouble(), units );
					d.lineTo( x.pt(), y.pt() );
					state = CMD;
					break;
				case LDX:
					dx = Distance( tokens[i].toDouble(), units );
					state = LDY;
					break;
				case LDY:
					dy = Distance( tokens[i].toDouble(), units );
					c = d.currentPosition();
					d.lineTo( c.x()+dx.pt(), c.y()+dy.pt() );
					state = CMD;
					break;
				case HX:
					x = Distance( tokens[i].toDouble(), units );
					c = d.currentPosition();
					d.lineTo( x.pt(), c.y() );
					state = CMD;
					break;
				case HDX:
					dx = Distance( tokens[i].toDouble(), units );
					c = d.currentPosition();
					d.lineTo( c.x()+dx.pt(), c.y() );
					state = CMD;
					break;
				case VY:
					y = Distance( tokens[i].toDouble(), units );
					c = d.currentPosition();
					d.lineTo( c.x(), y.pt() );
					state = CMD;
					break;
				case VDY:
					dy = Distance( tokens[i].toDouble(), units );
					c = d.currentPosition();
					d.lineTo( c.x(), c.y()+dy.pt() );
					state = CMD;
					break;
				}
			}

			return d;
		}


		void XmlUtil::setStringAttr( QDomElement&   node,
		                             const QString& name,
		                             const QString& value )
		{
			init();

			node.setAttribute( name, value );
		}


		void XmlUtil::setDoubleAttr( QDomElement&   node,
		                             const QString& name,
		                             double         value )
		{
			init();

			node.setAttribute( name, QString::number(value) );
		}


		void XmlUtil::setBoolAttr( QDomElement&   node,
		                           const QString& name,
		                           bool           value )
		{
			init();

			node.setAttribute( name, value ? "true" : "false" );
		}


		void XmlUtil::setIntAttr( QDomElement&   node,
		                          const QString& name,
		                          int            value )
		{
			init();

			node.setAttribute( name, QString::number(value) );
		}


		void XmlUtil::setUIntAttr( QDomElement&   node,
		                           const QString& name,
		                           uint32_t       value )
		{
			init();

			node.setAttribute( name, "0x" + QString::number(value, 16) );
		}


		void XmlUtil::setLengthAttr( QDomElement&    node,
		                             const QString&  name,
		                             const Distance& value )
		{
			init();

			Units units = mInstance->mUnits;
			node.setAttribute( name, QString::number(value.inUnits(units)) + units.toIdString() );
		}


		void XmlUtil::setWeightAttr( QDomElement&    node,
		                             const QString&  name,
		                             QFont::Weight   value )
		{
			switch (value)
			{
			case QFont::Bold:
				node.setAttribute( name, "bold" );
				break;
			default:
				node.setAttribute( name, "normal" );
				break;
			}
		}
	

		void XmlUtil::setAlignmentAttr( QDomElement&    node,
		                                const QString&  name,
		                                Qt::Alignment   value )
		{
			switch (value)
			{
			case Qt::AlignRight:
				node.setAttribute( name, "right" );
				break;
			case Qt::AlignHCenter:
				node.setAttribute( name, "hcenter" );
				break;
			case Qt::AlignLeft:
				node.setAttribute( name, "left" );
				break;
			case Qt::AlignBottom:
				node.setAttribute( name, "bottom" );
				break;
			case Qt::AlignVCenter:
				node.setAttribute( name, "vcenter" );
				break;
			case Qt::AlignTop:
				node.setAttribute( name, "top" );
				break;
			default:
				node.setAttribute( name, "left" );
				break;
			}
		}

	
		void XmlUtil::setWrapModeAttr( QDomElement&          node,
		                               const QString&        name,
		                               QTextOption::WrapMode value )
		{
			switch (value)
			{
			case QTextOption::WordWrap:
				node.setAttribute( name, "word" );
				break;
			case QTextOption::WrapAnywhere:
				node.setAttribute( name, "anywhere" );
				break;
			case QTextOption::NoWrap:
			case QTextOption::ManualWrap:
				node.setAttribute( name, "none" );
				break;
			default:
				node.setAttribute( name, "word" );
				break;
			}
		}

	
		void XmlUtil::setUnitsAttr( QDomElement&   node,
		                            const QString& name,
		                            const Units&   value )
		{
			node.setAttribute( name, value.toIdString() );
		}

	
		void XmlUtil::setPathDataAttr( QDomElement&        node,
		                               const QString&      name,
		                               const QPainterPath& path,
		                               const Units&        units )
		{
			QString pathString;
			for ( int i = 0; i < path.elementCount(); i++ )
			{
				auto element = path.elementAt( i );

				// QPainterPath is natively in pts
				Distance x = Distance::pt( element.x );
				Distance y = Distance::pt( element.y );

				// Translate desired units for path data
				double xValue = x.inUnits( units );
				double yValue = y.inUnits( units );

				if ( element.isMoveTo() )
				{
					pathString.append( QString( "M %1 %2" ).arg( xValue ).arg( yValue ) );
				}
				else if ( element.isLineTo() )
				{
					pathString.append( QString( "L %1 %2" ).arg( xValue ).arg( yValue ) );
				}

				if ( i < (path.elementCount() - 1) )
				{
					pathString.append( " " );
				}
			}

			node.setAttribute( name, pathString );
		}

	
	}
}
