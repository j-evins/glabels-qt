/*  FrameEllipse.cpp
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

#include "FrameEllipse.h"

#include "Constants.h"
#include "StrUtil.h"


namespace glabels
{
	namespace model
	{

		FrameEllipse::FrameEllipse( const Distance& w,
		                            const Distance& h,
		                            const Distance& waste,
		                            const QString&  id )
			: Frame(id), mW(w), mH(h), mWaste(waste)
		{
			mPath.addEllipse( 0, 0, mW.pt(), mH.pt() );
			mClipPath.addEllipse( -mWaste.pt(), -mWaste.pt(), (mW+2*mWaste).pt(), (mH+2*mWaste).pt() );
		}


		Frame* FrameEllipse::dup() const
		{
			return new FrameEllipse( *this );
		}


		Distance FrameEllipse::w() const
		{
			return mW;
		}

	
		Distance FrameEllipse::h() const
		{
			return mH;
		}
	

		Distance FrameEllipse::waste() const
		{
			return mWaste;
		}


		QString FrameEllipse::sizeDescription( const Units& units ) const
		{
			if ( units.toEnum() == Units::IN )
			{
				QString wStr = StrUtil::formatFraction( mW.in() );
				QString hStr = StrUtil::formatFraction( mH.in() );

				return QString("%1 x %2 %3").arg(wStr).arg(hStr).arg(units.toTrName());
			}
			else
			{
				return QString("%1 x %2 %3").arg(mW.inUnits(units), 0, 'g', 5)
			                                 .arg(mH.inUnits(units), 0, 'g', 5)
				                         .arg(units.toTrName());
			}
		}


		bool FrameEllipse::isSimilarTo( Frame* other ) const
		{
			if ( auto* otherEllipse = dynamic_cast<FrameEllipse*>(other) )
			{
				if ( (fabs( mW - otherEllipse->mW ) <= EPSILON) &&
				     (fabs( mH - otherEllipse->mH ) <= EPSILON) )
				{
					return true;
				}
			}
			return false;
		}


		const QPainterPath& FrameEllipse::path() const
		{
			return mPath;
		}

	
		const QPainterPath& FrameEllipse::clipPath() const
		{
			return mClipPath;
		}

	
		QPainterPath FrameEllipse::marginPath( const Distance& xSize,
		                                       const Distance& ySize ) const
		{
			// Note: ignore ySize, assume xSize == ySize
			Distance size = xSize;
			
			Distance w = mW - 2*size;
			Distance h = mH - 2*size;

			QPainterPath path;
			path.addEllipse( size.pt(), size.pt(), w.pt(), h.pt() );

			return path;
		}

	}
}


QDebug operator<<( QDebug dbg, const glabels::model::FrameEllipse& frame )
{
	QDebugStateSaver saver(dbg);

	dbg.nospace() << "FrameEllipse{ "
	              << frame.id() << "," 
	              << frame.w() << "," 
	              << frame.h() << "," 
	              << frame.waste() << "," 
	              << frame.layouts() << ","
	              << frame.markups()
	              << " }";

	return dbg;
}
