/*  Distance.cpp
 *
 *  Copyright (C) 2016  Jaye Evins <evins@snaught.com>
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

#include "Distance.h"

#include <QTextStream>
#include <QDebug>
#include <QIODevice>


namespace glabels
{
	namespace model
	{

		Distance::Distance( double d, Units::Enum unitsEnum )
		{
			switch (unitsEnum)
			{
			case Units::PT:
				mDPts = d;
				break;
			case Units::IN:
				mDPts = d * PTS_PER_INCH;
				break;
			case Units::MM:
				mDPts = d * PTS_PER_MM;
				break;
			case Units::CM:
				mDPts = d * PTS_PER_CM;
				break;
			case Units::PC:
				mDPts = d * PTS_PER_PICA;
				break;
			default:
				mDPts = d;
				break;
			}
		}


		Distance::Distance( double d, const Units& units )
		{
			switch (units.toEnum())
			{
			case Units::PT:
				mDPts = d;
				break;
			case Units::IN:
				mDPts = d * PTS_PER_INCH;
				break;
			case Units::MM:
				mDPts = d * PTS_PER_MM;
				break;
			case Units::CM:
				mDPts = d * PTS_PER_CM;
				break;
			case Units::PC:
				mDPts = d * PTS_PER_PICA;
				break;
			default:
				mDPts = d;
				break;
			}
		}


		Distance::Distance( double d, const QString& unitsId )
		{
			Units units = Units( unitsId );
		
			switch (units.toEnum())
			{
			case Units::PT:
				mDPts = d;
				break;
			case Units::IN:
				mDPts = d * PTS_PER_INCH;
				break;
			case Units::MM:
				mDPts = d * PTS_PER_MM;
				break;
			case Units::CM:
				mDPts = d * PTS_PER_CM;
				break;
			case Units::PC:
				mDPts = d * PTS_PER_PICA;
				break;
			default:
				mDPts = d;
				break;
			}
		}


		Distance Distance::fromString( const QString& string )
		{
			QString stringCopy = string;
			QTextStream valueStream( &stringCopy, QIODevice::ReadOnly );

			double value;
			QString unitsString;
			valueStream >> value >> unitsString;

			if ( !unitsString.isEmpty() && !Units::isIdValid( unitsString ) )
			{
				qWarning() << "Invalid Units in string: \"" << string << "\"";
			}

			return Distance( value, unitsString );
		}


		double Distance::inUnits( const Units& units ) const
		{
			double d;

			switch (units.toEnum())
			{
			case Units::PT:
				d = pt();
				break;
			case Units::IN:
				d = in();
				break;
			case Units::MM:
				d = mm();
				break;
			case Units::CM:
				d = cm();
				break;
			case Units::PC:
				d = pc();
				break;
			}

			return d;
		}
	

		double Distance::inUnits( Units::Enum unitsEnum ) const
		{
			double d;

			switch (unitsEnum)
			{
			case Units::PT:
				d = pt();
				break;
			case Units::IN:
				d = in();
				break;
			case Units::MM:
				d = mm();
				break;
			case Units::CM:
				d = cm();
				break;
			case Units::PC:
				d = pc();
				break;
			}

			return d;
		}
	

		double Distance::inUnits( const QString& unitsId ) const
		{
			return inUnits( Units( unitsId ) );
		}


		QString Distance::toString( const Units& units ) const
		{
			return QString::number( inUnits(units) ) + units.toIdString();
		}


		QString Distance::toString( Units::Enum unitsEnum ) const
		{
			Units units(unitsEnum);
			return QString::number( inUnits(units) ) + units.toIdString();
		}


		QString Distance::toString( const QString& unitsId ) const
		{
			Units units(unitsId);
			return QString::number( inUnits(units) ) + units.toIdString();
		}

	}
}


QDebug operator<<( QDebug dbg, const glabels::model::Distance& distance )
{
	QDebugStateSaver saver(dbg);

	dbg.nospace() << distance.pt() << "pt";

	return dbg;
}
