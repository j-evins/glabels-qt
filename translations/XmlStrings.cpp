/*  XmlStrings.cpp
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

#include <QCoreApplication>
#include <QStringList>
#include <QFile>
#include <QDomDocument>
#include <QDomNode>
#include <QDomAttr>
#include <QTextStream>
#include <QDebug>


namespace
{
	QStringList stringList;
}


void parseElement( const QDomElement& node )
{
	// Examine each attribute for translatable strings
	QDomNamedNodeMap attrNodes = node.attributes();
	for ( int i = 0; i < attrNodes.count(); i++ )
	{
		QDomAttr attr = attrNodes.item(i).toAttr();
		if ( attr.name().at(0) == '_' )
		{
			if ( !stringList.contains( attr.value() ) )
			{
				stringList.append( attr.value() );
			}
		}
	}

	// Recurse over children
	for ( QDomNode child = node.firstChild(); !child.isNull(); child = child.nextSibling() )
	{
		parseElement( child.toElement() );
	}
}


void parseFile( const QString& filename )
{
	QFile file( filename );

	if ( file.open( QFile::ReadOnly|QFile::Text ) )
	{
		QDomDocument doc;

		if ( doc.setContent( &file, false ) )
		{
			QDomElement root = doc.documentElement();

			parseElement( root );
		}
	}
}


int main( int argc, char *argv[] )
{
	QCoreApplication app( argc, argv );

	QStringList filenameList = app.arguments();
	filenameList.removeFirst();  // Remove 0th argument, which is the command name

	foreach ( QString filename, filenameList )
	{
		parseFile( filename );
	}

	stringList.sort();

	QTextStream out( stdout );

	out << "// Automatically generated with " << app.arguments().at(0) << Qt::endl;
	out << "//" << Qt::endl;
	out << "// Sources:" << Qt::endl;
	foreach ( QString filename, filenameList )
	{
		out << "//         " << filename << Qt::endl;
	}
	out << "//" << Qt::endl;
		
	foreach ( QString string, stringList )
	{
		out << "QT_TRANSLATE_NOOP( \"XmlStrings\", \"" << string << "\" );" << Qt::endl;
	}
}
