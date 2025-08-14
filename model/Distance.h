/*  Distance.h
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

#ifndef model_Distance_h
#define model_Distance_h


#include "Units.h"

#include <QCoreApplication>
#include <QString>
#include <QtMath>


namespace glabels
{
	namespace model
	{

		class Distance
		{
			Q_DECLARE_TR_FUNCTIONS(Distance)

		public:
			Distance();
			Distance( double d, Units::Enum unitsEnum = Units::PT );
			Distance( double d, const Units& units );
			Distance( double d, const QString& unitsId );

			static Distance pt( double dPts );
			static Distance in( double dInches );
			static Distance mm( double dMm );
			static Distance cm( double dCm );
			static Distance pc( double dPicas );
			static Distance fromString( const QString& string );


			double pt() const;
			double in() const;
			double mm() const;
			double cm() const;
			double pc() const;
			double inUnits( const Units& units ) const;
			double inUnits( Units::Enum unitsEnum ) const;
			double inUnits( const QString& unitsId ) const;


			QString toString( const Units& units ) const;
			QString toString( Units::Enum unitsEnum ) const;
			QString toString( const QString& unitsId ) const;

			Distance& operator+=( const Distance& d );
			Distance& operator-=( const Distance& d );
			Distance& operator*=( double f );
			Distance operator-();

			friend inline Distance operator+( const Distance& d1, const Distance& d2 );
			friend inline Distance operator-( const Distance& d1, const Distance& d2 );
			friend inline Distance operator*( double x, const Distance& d );
			friend inline Distance operator*( const Distance& d, double x );
			friend inline double   operator/( const Distance& d1, const Distance& d2 );
			friend inline Distance operator/( const Distance& d, double x );
		
			friend inline bool operator<( const Distance& d1, const Distance& d2 );
			friend inline bool operator<=( const Distance& d1, const Distance& d2 );
			friend inline bool operator>( const Distance& d1, const Distance& d2 );
			friend inline bool operator>=( const Distance& d1, const Distance& d2 );
			friend inline bool operator==( const Distance& d1, const Distance& d2 );
			friend inline bool operator!=( const Distance& d1, const Distance& d2 );
		
			friend inline Distance fabs( const Distance& d );
			friend inline Distance min( const Distance& d1, const Distance& d2 );
			friend inline Distance max( const Distance& d1, const Distance& d2 );
			friend inline Distance fmod( const Distance& d1, const Distance& d2 );

		
		private:
			double mDPts;

		};

	}
}


// Debugging support
QDebug operator<<( QDebug dbg, const glabels::model::Distance& distance );


//
// Inline methods
//
	
#include "Constants.h"

namespace glabels
{
	namespace model
	{

		inline Distance::Distance() : mDPts(0)
		{
		}

	
		inline Distance Distance::pt( double dPts )
		{
			Distance d;
			d.mDPts = dPts;
			return d;
		}


		inline Distance Distance::in( double dInches )
		{
			Distance d;
			d.mDPts = dInches * PTS_PER_INCH;
			return d;
		}


		inline Distance Distance::mm( double dMm )
		{
			Distance d;
			d.mDPts = dMm * PTS_PER_MM;
			return d;
		}


		inline Distance Distance::cm( double dCm )
		{
			Distance d;
			d.mDPts = dCm * PTS_PER_CM;
			return d;
		}


		inline Distance Distance::pc( double dPicas )
		{
			Distance d;
			d.mDPts = dPicas * PTS_PER_PICA;
			return d;
		}


		inline double Distance::pt() const
		{
			return mDPts;
		}
	

		inline double Distance::in() const
		{
			return mDPts / PTS_PER_INCH;
		}
	

		inline double Distance::mm() const
		{
			return mDPts / PTS_PER_MM;
		}
	

		inline double Distance::cm() const
		{
			return mDPts / PTS_PER_CM;
		}
	

		inline double Distance::pc() const
		{
			return mDPts / PTS_PER_PICA;
		}


		inline Distance& Distance::operator+=( const Distance& d )
		{
			mDPts += d.mDPts;
			return *this;
		}

	
		inline Distance& Distance::operator-=( const Distance& d )
		{
			mDPts -= d.mDPts;
			return *this;
		}

	
		inline Distance& Distance::operator*=( double f )
		{
			mDPts *= f;
			return *this;
		}

	
		inline Distance Distance::operator-()
		{
			return Distance::pt( -mDPts );
		}

	
		inline Distance operator+( const Distance& d1, const Distance& d2 )
		{
			return Distance::pt( d1.mDPts + d2.mDPts );
		}
	

		inline Distance operator-( const Distance& d1, const Distance& d2 )
		{
			return Distance::pt( d1.mDPts - d2.mDPts );
		}
	

		inline Distance operator*( double x, const Distance& d )
		{
			return Distance::pt( x * d.mDPts );
		}


		inline Distance operator*( const Distance& d, double x )
		{
			return Distance::pt( d.mDPts * x );
		}

	
		inline double operator/( const Distance& d1, const Distance& d2 )
		{
			return d1.mDPts / d2.mDPts;
		}


		inline Distance operator/( const Distance& d, double x )
		{
			return Distance::pt( d.mDPts / x );
		}


		inline bool operator<( const Distance& d1, const Distance& d2 )
		{
			return d1.mDPts < d2.mDPts;
		}
	

		inline bool operator<=( const Distance& d1, const Distance& d2 )
		{
			return d1.mDPts <= d2.mDPts;
		}
	

		inline bool operator>( const Distance& d1, const Distance& d2 )
		{
			return d1.mDPts > d2.mDPts;
		}
	

		inline bool operator>=( const Distance& d1, const Distance& d2 )
		{
			return d1.mDPts >= d2.mDPts;
		}
	

		inline bool operator==( const Distance& d1, const Distance& d2 )
		{
			return d1.mDPts == d2.mDPts;
		}


		inline bool operator!=( const Distance& d1, const Distance& d2 )
		{
			return d1.mDPts != d2.mDPts;
		}


		inline Distance fabs( const Distance& d )
		{
			return Distance::pt( qFabs( d.mDPts ) );
		}

	
		inline Distance min( const Distance& d1, const Distance& d2 )
		{
			return (d1.mDPts < d2.mDPts) ? d1 : d2;
		}

	
		inline Distance max( const Distance& d1, const Distance& d2 )
		{
			return (d1.mDPts > d2.mDPts) ? d1 : d2;
		}


		inline Distance fmod( const Distance& d1, const Distance& d2 )
		{
			return Distance::pt( std::fmod( d1.mDPts, d2.mDPts ) );
		}

	}
}


#endif // model_Distance_h
