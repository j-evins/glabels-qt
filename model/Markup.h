/*  Markup.h
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

#ifndef model_Markup_h
#define model_Markup_h


#include "Frame.h"

#include <QPainterPath>


namespace glabels
{
	namespace model
	{

		class Markup
		{
		public:
			virtual ~Markup() = default;

			virtual Markup* dup() const = 0;

			virtual QPainterPath path( const Frame* frame ) const;

		protected:
			QPainterPath mPath;
		};


		class MarkupMargin : public Markup
		{
		public:
			MarkupMargin( const Distance& size );

			MarkupMargin( const Distance& xSize,
			              const Distance& ySize );

			QPainterPath path( const Frame* frame ) const override;

			Distance xSize() const;
			Distance ySize() const;

			Markup* dup() const override;

		private:
			Distance  mXSize;
			Distance  mYSize;
		};


		class MarkupLine : public Markup
		{
		public:
			MarkupLine( const Distance& x1,
			            const Distance& y1,
			            const Distance& x2,
			            const Distance& y2 );

			Distance x1() const;
			Distance y1() const;
			Distance x2() const;
			Distance y2() const;

			Markup* dup() const override;

		private:
			Distance  mX1;
			Distance  mY1;
			Distance  mX2;
			Distance  mY2;
		};


		class MarkupRect : public Markup
		{
		public:
			MarkupRect( const Distance& x1,
			            const Distance& y1,
			            const Distance& w,
			            const Distance& h,
			            const Distance& r );

			Distance x1() const;
			Distance y1() const;
			Distance w() const;
			Distance h() const;
			Distance r() const;

			Markup* dup() const override;

		private:
			Distance  mX1;
			Distance  mY1;
			Distance  mW;
			Distance  mH;
			Distance  mR;
		};


		class MarkupEllipse : public Markup
		{
		public:
			MarkupEllipse( const Distance& x1,
			               const Distance& y1,
			               const Distance& w,
			               const Distance& h );

			Distance x1() const;
			Distance y1() const;
			Distance w() const;
			Distance h() const;

			Markup* dup() const override;

		private:
			Distance  mX1;
			Distance  mY1;
			Distance  mW;
			Distance  mH;
		};


		class MarkupCircle : public Markup
		{
		public:
			MarkupCircle( const Distance& x0,
			              const Distance& y0,
			              const Distance& r );

			Distance x0() const;
			Distance y0() const;
			Distance r() const;

			Markup* dup() const override;

		private:
			Distance  mX0;
			Distance  mY0;
			Distance  mR;
		};
	

	}
}


#endif // model_Markup_h
