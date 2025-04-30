/*  Frame.h
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

#ifndef model_Frame_h
#define model_Frame_h


#include "Distance.h"
#include "Layout.h"
#include "Point.h"

#include <QCoreApplication>
#include <QDebug>
#include <QList>
#include <QPainterPath>
#include <QString>
#include <QVector>


namespace glabels
{
	namespace model
	{
	
		// Forward references
		class Markup;


		class Frame
		{
			Q_DECLARE_TR_FUNCTIONS(Frame)

		protected:
			Frame( const QString& id = "0" );
			Frame( const Frame& other );

		public:
			virtual ~Frame();
			virtual Frame* dup() const = 0;

			QString id() const;
			int nLabels() const;
			QString layoutDescription() const;
			const QList<Layout>&  layouts() const;
			const QList<Markup*>& markups() const;

			QVector<Point> getOrigins() const;

			void addLayout( const Layout& layout );
			void addMarkup( Markup* markup );

			virtual Distance w() const = 0;
			virtual Distance h() const = 0;

			virtual void setH( const Distance& h );

			virtual QString sizeDescription( const Units& units ) const = 0;
			virtual bool isSimilarTo( Frame* other ) const = 0;

			virtual const QPainterPath& path() const = 0;
			virtual const QPainterPath& clipPath() const = 0;
			virtual QPainterPath marginPath( const Distance& xSize,
			                                 const Distance& ySize ) const = 0;


		private:
			QString mId;
			int     mNLabels;
			QString mLayoutDescription;

			QList<Layout>  mLayouts;
			QList<Markup*> mMarkups;
		};

	}
}


// Debugging support
QDebug operator<<( QDebug dbg, const glabels::model::Frame& frame );


#endif // model_Frame_h
