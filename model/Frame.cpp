/*  Frame.cpp
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



#include "Frame.h"

#include <QDebug>

#include <algorithm>


namespace glabels
{
	namespace model
	{

		Frame::Frame( const QString& id )
			: mId(id),
			  mNLabels(0),
			  mLayoutDescription("")
		{
			// empty
		}


		Frame::Frame( const Frame& other )
		{
			mId = other.mId;
			mNLabels = 0;

			for ( const auto& layout : other.mLayouts )
			{
				addLayout( layout );
			}
		
			for ( const auto& markup : other.mMarkups )
			{
				mMarkups.push_back( markup->clone() );
			}
		}


		QString Frame::id() const
		{
			return mId;
		}

	
		int Frame::nLabels() const
		{
			return mNLabels;
		}

	
		QString Frame::layoutDescription() const
		{
			return mLayoutDescription;
		}

	
		const std::list<Layout>& Frame::layouts() const
		{
			return mLayouts;
		}

	
		const std::list<std::unique_ptr<Markup>>& Frame::markups() const
		{
			return mMarkups;
		}
	

		QVector<Point> Frame::getOrigins() const
		{
			QVector<Point> origins( nLabels() );

			int i = 0;
			for ( auto& layout : mLayouts )
			{
				for ( int iy = 0; iy < layout.ny(); iy++ )
				{
					for ( int ix = 0; ix < layout.nx(); ix++ )
					{
						origins[i++] = Point( ix*layout.dx() + layout.x0(), iy*layout.dy() + layout.y0() );
					}
				}
			}

			std::stable_sort( origins.begin(), origins.end() );

			return origins;
		}


		void Frame::addLayout( const Layout& layout )
		{
			mLayouts.push_back( layout );

			// Update total number of labels
			mNLabels += layout.nx() * layout.ny();

			// Update layout description
			if ( mLayouts.size() == 1 )
			{
				// TRANSLATORS
				//:   %1 = number of labels across a page,
				//:   %2 = number of labels down a page,
				//:   %3 = total number of labels on a page (sheet).
				mLayoutDescription = QString( tr("%1 x %2 (%3 per sheet)") )
					.arg(layout.nx()).arg(layout.ny()).arg(mNLabels);
			}
			else
			{
				// TRANSLATORS
				//:   %1 is the total number of labels on a page (sheet).
				mLayoutDescription = QString( tr("%1 per sheet") ).arg(mNLabels);
			}
		}


		void Frame::addMarkup( const Markup& markup )
		{
			mMarkups.push_back( markup.clone() );
		}


		bool Frame::setH( Distance h )
		{
			// Default implementation does nothing
			return false;
		}

	}
}


QDebug operator<<( QDebug dbg, const glabels::model::Frame& frame )
{
	QDebugStateSaver saver(dbg);

	frame.print( dbg );

	return dbg;
}
