/*  DrawingPrimitives.cpp
 *
 *  Copyright (C) 2013  Jaye Evins <evins@snaught.com>
 *
 *  This file is part of glbarcode++.
 *
 *  glbarcode++ is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  glbarcode++ is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with glbarcode++.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "DrawingPrimitives.h"


namespace glbarcode
{

	DrawingPrimitive::DrawingPrimitive( double x, double y )
		: mX(x), mY(y)
	{
	}


	double DrawingPrimitive::x() const
	{
		return mX;
	}


	double DrawingPrimitive::y() const
	{
		return mY;
	}



	DrawingPrimitiveLine::DrawingPrimitiveLine( double x, double y, double w, double h )
		: DrawingPrimitive( x, y ), mW(w), mH(h)
	{
	}


	double DrawingPrimitiveLine::w() const
	{
		return mW;
	}


	double DrawingPrimitiveLine::h() const
	{
		return mH;
	}



	DrawingPrimitiveBox::DrawingPrimitiveBox( double x, double y, double w, double h )
		: DrawingPrimitive( x, y ), mW(w), mH(h)
	{
	}


	double DrawingPrimitiveBox::w() const
	{
		return mW;
	}


	double DrawingPrimitiveBox::h() const
	{
		return mH;
	}



	DrawingPrimitiveText::DrawingPrimitiveText( double x, double y, double size, const std::string& text, HAlign halign )
		: DrawingPrimitive( x, y ), mSize(size), mText(text), mHalign(halign)
	{
	}


	double DrawingPrimitiveText::size() const
	{
		return mSize;
	}


	const std::string& DrawingPrimitiveText::text() const
	{
		return mText;
	}


	HAlign DrawingPrimitiveText::halign() const
	{
		return mHalign;
	}



	DrawingPrimitiveRing::DrawingPrimitiveRing( double x, double y, double r, double w )
		: DrawingPrimitive( x, y ), mR(r), mW(w)
	{
	}


	double DrawingPrimitiveRing::r() const
	{
		return mR;
	}


	double DrawingPrimitiveRing::w() const
	{
		return mW;
	}



	DrawingPrimitiveHexagon::DrawingPrimitiveHexagon( double x, double y, double h )
		: DrawingPrimitive( x, y ), mH(h)
	{
	}


	double DrawingPrimitiveHexagon::h() const
	{
		return mH;
	}

}
