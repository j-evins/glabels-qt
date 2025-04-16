/*  FramePath.cpp
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

#include "FramePath.h"

#include "Constants.h"
#include "StrUtil.h"


namespace glabels
{
	namespace model
	{

		FramePath::FramePath( const QPainterPath& path,
		                      const Distance&     xWaste,
		                      const Distance&     yWaste,
		                      const Units&        originalUnits,
		                      const QString&      id )
			: Frame(id), mXWaste(xWaste), mYWaste(yWaste), mPath(path), mOriginalUnits(originalUnits)
		{
			QRectF r = path.boundingRect();
			
			mW = Distance::pt( r.width() );
			mH = Distance::pt( r.height() );

			mClipPath.addRect( r.x()-mXWaste.pt(), r.y()-mYWaste.pt(),
			                   r.width() + 2*mXWaste.pt(), r.height() + 2*mYWaste.pt() );
		}

	
		Frame* FramePath::dup() const
		{
			return new FramePath( *this );
		}


		Distance FramePath::w() const
		{
			return mW;
		}

	
		Distance FramePath::h() const
		{
			return mH;
		}


		Distance FramePath::xWaste() const
		{
			return mXWaste;
		}

		
		Distance FramePath::yWaste() const
		{
			return mYWaste;
		}


		Units FramePath::originalUnits() const
		{
			return mOriginalUnits;
		}


		QString FramePath::sizeDescription( const Units& units ) const
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


		bool FramePath::isSimilarTo( Frame* other ) const
		{
			if ( auto *otherPath = dynamic_cast<FramePath*>(other) )
			{
				if ( mPath == otherPath->mPath )
				{
					return true;
				}
			}
			return false;
		}


		const QPainterPath& FramePath::path() const
		{
			return mPath;
		}


		const QPainterPath& FramePath::clipPath() const
		{
			return mClipPath;
		}


		QPainterPath FramePath::marginPath( const Distance& xSize,
		                                    const Distance& ySize ) const
		{
			return mPath; // No margin
		}


	}
}


QDebug operator<<( QDebug dbg, const glabels::model::FramePath& frame )
{
	QDebugStateSaver saver(dbg);

	dbg.nospace() << "FramePath{ "
	              << frame.id() << "," 
	              << frame.path() << "," 
	              << frame.xWaste() << "," 
	              << frame.yWaste() << "," 
	              << frame.layouts() << ","
	              << frame.markups()
	              << " }";

	return dbg;
}
