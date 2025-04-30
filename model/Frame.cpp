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

#include "FrameCd.h"
#include "FrameContinuous.h"
#include "FrameEllipse.h"
#include "FramePath.h"
#include "FrameRect.h"
#include "FrameRound.h"
#include "Markup.h"

#include <algorithm>


namespace glabels
{
	namespace model
	{

		Frame::Frame( const QString& id )
			: mId(id), mNLabels(0), mLayoutDescription("")
		{
			// empty
		}


		Frame::Frame( const Frame& other )
		{
			mId = other.mId;
			mNLabels = 0;

			foreach ( const Layout& layout, other.mLayouts )
			{
				addLayout( layout );
			}
		
			foreach ( Markup *markup, other.mMarkups )
			{
				addMarkup( markup->dup() );
			}
		}


		Frame::~Frame()
		{
			while ( !mMarkups.isEmpty() )
			{
				delete mMarkups.takeFirst();
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

	
		const QList<Layout>& Frame::layouts() const
		{
			return mLayouts;
		}

	
		const QList<Markup*>& Frame::markups() const
		{
			return mMarkups;
		}
	

		QVector<Point> Frame::getOrigins() const
		{
			QVector<Point> origins( nLabels() );

			int i = 0;
			foreach ( const Layout& layout, mLayouts )
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
			mLayouts << layout;

			// Update total number of labels
			mNLabels += layout.nx() * layout.ny();

			// Update layout description
			if ( mLayouts.size() == 1 )
			{
				/*
				 * Translators: %1 = number of labels across a page,
				 *              %2 = number of labels down a page,
				 *              %3 = total number of labels on a page (sheet).
				 */
				mLayoutDescription = QString( tr("%1 x %2 (%3 per sheet)") )
					.arg(layout.nx()).arg(layout.ny()).arg(mNLabels);
			}
			else
			{
				/* Translators: %1 is the total number of labels on a page (sheet). */
				mLayoutDescription = QString( tr("%1 per sheet") ).arg(mNLabels);
			}
		}


		void Frame::addMarkup( Markup *markup )
		{
			mMarkups << markup;
		}


		void Frame::setH( const Distance& h )
		{
			// Default implementation does nothing
		}

	}
}


QDebug operator<<( QDebug dbg, const glabels::model::Frame& frame )
{
	if ( auto* frameCd = dynamic_cast<const glabels::model::FrameCd*>(&frame) )
	{
		dbg << *frameCd;
		return dbg;
	}
	else if ( auto* frameContinuous = dynamic_cast<const glabels::model::FrameContinuous*>(&frame) )
	{
		dbg << *frameContinuous;
		return dbg;
	}
	else if ( auto* frameEllipse = dynamic_cast<const glabels::model::FrameEllipse*>(&frame) )
	{
		dbg << *frameEllipse;
		return dbg;
	}
	else if ( auto* framePath = dynamic_cast<const glabels::model::FramePath*>(&frame) )
	{
		dbg << *framePath;
		return dbg;
	}
	else if ( auto* frameRect = dynamic_cast<const glabels::model::FrameRect*>(&frame) )
	{
		dbg << *frameRect;
		return dbg;
	}
	else if ( auto* frameRound = dynamic_cast<const glabels::model::FrameRound*>(&frame) )
	{
		dbg << *frameRound;
		return dbg;
	}
	else
	{
		QDebugStateSaver saver(dbg);

		dbg.nospace() << "UNKNOWN FRAME";

		return dbg;
	}
}
