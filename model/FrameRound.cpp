/*  FrameRound.cpp
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

#include "FrameRound.h"

#include "Constants.h"
#include "StrUtil.h"


namespace glabels
{
	namespace model
	{

		FrameRound::FrameRound( const Distance& r,
		                        const Distance& waste,
		                        const QString&  id )
			: Frame(id), mR(r), mWaste(waste)
		{
			mPath.addEllipse( 0, 0, 2*mR.pt(), 2*mR.pt() );
			mClipPath.addEllipse( -mWaste.pt(), -mWaste.pt(),
			                      2*(mR+mWaste).pt(), 2*(mR+mWaste).pt() );
		}
	

		Frame* FrameRound::dup() const
		{
			return new FrameRound( *this );
		}

	
		Distance FrameRound::w() const
		{
			return 2*mR;
		}

	
		Distance FrameRound::h() const
		{
			return 2*mR;
		}

	
		Distance FrameRound::r() const
		{
			return mR;
		}


		Distance FrameRound::waste() const
		{
			return mWaste;
		}


		QString FrameRound::sizeDescription( const Units& units ) const
		{
			if ( units.toEnum() == Units::IN )
			{
				QString dStr = StrUtil::formatFraction( 2 * mR.in() );

				return QString("%1 %2 %3").arg(dStr).arg(units.toTrName()).arg(tr("diameter"));
			}
			else
			{
				return QString("%1 %2 %3").arg(2 * mR.inUnits(units), 0, 'g', 5)
			                                  .arg(units.toTrName()).arg(tr("diameter"));
			}
		}


		bool FrameRound::isSimilarTo( Frame* other ) const
		{
			if ( auto *otherRound = dynamic_cast<FrameRound*>(other) )
			{
				if ( fabs( mR - otherRound->mR ) <= EPSILON )
				{
					return true;
				}
			}
			return false;
		}


		const QPainterPath& FrameRound::path() const
		{
			return mPath;
		}


		const QPainterPath& FrameRound::clipPath() const
		{
			return mClipPath;
		}


		QPainterPath FrameRound::marginPath( const Distance& xSize,
		                                     const Distance& ySize ) const
		{
			// Note: ignore ySize, assume xSize == ySize
			Distance size = xSize;

			Distance r = mR - size;

			QPainterPath path;
			path.addEllipse( size.pt(), size.pt(), 2*r.pt(), 2*r.pt() );

			return path;
		}

	}
}


QDebug operator<<( QDebug dbg, const glabels::model::FrameRound& frame )
{
	QDebugStateSaver saver(dbg);

	dbg.nospace() << "FrameRound{ "
	              << frame.id() << "," 
	              << frame.r() << "," 
	              << frame.waste() << "," 
	              << frame.layouts() << ","
	              << frame.markups()
	              << " }";

	return dbg;
}
