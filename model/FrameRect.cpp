/*  FrameRect.cpp
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

#include "FrameRect.h"

#include "Constants.h"
#include "StrUtil.h"


namespace glabels
{
	namespace model
	{

		FrameRect::FrameRect( const Distance& w,
		                      const Distance& h,
		                      const Distance& r,
		                      const Distance& xWaste,
		                      const Distance& yWaste,
		                      const QString&  id )
			: Frame(id), mW(w), mH(h), mR(r), mXWaste(xWaste), mYWaste(yWaste)
		{
			mPath.addRoundedRect( 0, 0, mW.pt(), mH.pt(), mR.pt(), mR.pt() );
		
			mClipPath.addRoundedRect( -mXWaste.pt(), -mYWaste.pt(),
			                          mW.pt() + 2*mXWaste.pt(), mH.pt() + 2*mYWaste.pt(),
			                          mR.pt(), mR.pt() );
		}


		Frame* FrameRect::dup() const
		{
			return new FrameRect( *this );
		}


		Distance FrameRect::w() const
		{
			return mW;
		}

	
		Distance FrameRect::h() const
		{
			return mH;
		}


		Distance FrameRect::r() const
		{
			return mR;
		}

		
		Distance FrameRect::xWaste() const
		{
			return mXWaste;
		}

		
		Distance FrameRect::yWaste() const
		{
			return mYWaste;
		}


		QString FrameRect::sizeDescription( const Units& units ) const
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


		bool FrameRect::isSimilarTo( Frame* other ) const
		{
			if ( auto *otherRect = dynamic_cast<FrameRect*>(other) )
			{
				if ( (fabs( mW - otherRect->mW ) <= EPSILON) &&
				     (fabs( mH - otherRect->mH ) <= EPSILON) )
				{
					return true;
				}
			}
			return false;
		}


		const QPainterPath& FrameRect::path() const
		{
			return mPath;
		}


		const QPainterPath& FrameRect::clipPath() const
		{
			return mClipPath;
		}


		QPainterPath FrameRect::marginPath( const Distance& xSize,
		                                    const Distance& ySize ) const
		{
			Distance w = mW - 2*xSize;
			Distance h = mH - 2*ySize;
			Distance r = std::max( mR - std::min(xSize, ySize), Distance(0.0) );

			QPainterPath path;
			path.addRoundedRect( xSize.pt(), ySize.pt(), w.pt(), h.pt(), r.pt(), r.pt() );

			return path;
		}


	}
}


QDebug operator<<( QDebug dbg, const glabels::model::FrameRect& frame )
{
	QDebugStateSaver saver(dbg);

	dbg.nospace() << "FrameRect{ "
	              << frame.id() << "," 
	              << frame.w() << "," 
	              << frame.h() << "," 
	              << frame.r() << "," 
	              << frame.xWaste() << "," 
	              << frame.yWaste() << ","
	              << frame.layouts() << ","
	              << frame.markups()
	              << " }";

	return dbg;
}
