/*  FrameContinuous.cpp
 *
 *  Copyright (C) 2018  Jaye Evins <evins@snaught.com>
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

#include "FrameContinuous.h"

#include "Constants.h"
#include "StrUtil.h"


namespace glabels
{
	namespace model
	{

		FrameContinuous::FrameContinuous( const Distance& w,
		                                  const Distance& hMin,
		                                  const Distance& hMax,
		                                  const Distance& hDefault,
		                                  const QString&  id )
			: Frame(id), mW(w), mHMin(hMin), mHMax(hMax), mHDefault(hDefault), mH(hDefault)
		{
			mPath.addRect( 0, 0, mW.pt(), mH.pt() );
		}

	
		Frame* FrameContinuous::dup() const
		{
			return new FrameContinuous( *this );
		}


		Distance FrameContinuous::w() const
		{
			return mW;
		}

	
		Distance FrameContinuous::h() const
		{
			return mH;
		}


		Distance FrameContinuous::hMin() const
		{
			return mHMin;
		}

		
		Distance FrameContinuous::hMax() const
		{
			return mHMax;
		}

		
		Distance FrameContinuous::hDefault() const
		{
			return mHDefault;
		}


		void FrameContinuous::setH( const Distance& h )
		{
			mH = h;
			mPath = QPainterPath(); // clear path
			mPath.addRect( 0, 0, mW.pt(), mH.pt() );
		}
		

		QString FrameContinuous::sizeDescription( const Units& units ) const
		{
			if ( units.toEnum() == Units::IN )
			{
				QString wStr = StrUtil::formatFraction( mW.in() );

				return QString("%1 %2 %3").arg(wStr).arg(units.toTrName()).arg(tr("wide"));
			}
			else
			{
				return QString("%1 %2 %3").arg(mW.inUnits(units), 0, 'f', 3).arg(units.toTrName()).arg(tr("wide"));
			}
		}


		bool FrameContinuous::isSimilarTo( Frame* other ) const
		{
			if ( auto *otherContinuous = dynamic_cast<FrameContinuous*>(other) )
			{
				if ( fabs( mW - otherContinuous->mW ) <= EPSILON )
				{
					return true;
				}
			}
			return false;
		}


		const QPainterPath& FrameContinuous::path() const
		{
			return mPath;
		}


		const QPainterPath& FrameContinuous::clipPath() const
		{
			return mPath;
		}


		QPainterPath FrameContinuous::marginPath( const Distance& xSize,
		                                          const Distance& ySize ) const
		{
			Distance w = mW - 2*xSize;
			Distance h = mH - 2*ySize;

			QPainterPath path;
			path.addRect( xSize.pt(), ySize.pt(), w.pt(), h.pt() );

			return path;
		}


	}
}


QDebug operator<<( QDebug dbg, const glabels::model::FrameContinuous& frame )
{
	QDebugStateSaver saver(dbg);

	dbg.nospace() << "FrameContinuous{ "
	              << frame.id() << "," 
	              << frame.w() << "," 
	              << frame.h() << "," 
	              << frame.hMin() << "," 
	              << frame.hMax() << "," 
	              << frame.hDefault() << ","
	              << frame.layouts() << ","
	              << frame.markups()
	              << " }";

	return dbg;
}
