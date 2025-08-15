/*  Barcode.cpp
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

#include "Barcode.h"

#include <list>

#include "DrawingPrimitives.h"


namespace glbarcode
{

	/*
	 * Barcode private data
	 */
	struct Barcode::PrivateData {

		bool                   mShowTextFlag;  /**< Display text flag */
		bool                   mChecksumFlag;  /**< Add checksum flag */

		double                 mW;             /**< Width of barcode (points) */
		double                 mH;             /**< Height of barcode (points) */

		bool                   mIsEmpty;       /**< Empty data flag */
		bool                   mIsDataValid;   /**< Valid data flag */

		std::list<DrawingPrimitive *> mPrimitives;      /**< List of drawing primitives */

	};


	Barcode::Barcode()
	{
		d = new Barcode::PrivateData;

		d->mShowTextFlag  = false;
		d->mChecksumFlag  = false;

		d->mW             = 0;
		d->mH             = 0;

		d->mIsEmpty       = true;
		d->mIsDataValid   = false;
	}


	Barcode::~Barcode()
	{
		clear(); /* Clear drawing primitives. */
		delete d;
	}


	Barcode& Barcode::setShowText( bool value )
	{
		d->mShowTextFlag = value;
		return *this;
	}


	bool Barcode::showText( ) const
	{
		return d->mShowTextFlag;
	}


	Barcode& Barcode::setChecksum( bool value )
	{
		d->mChecksumFlag = value;
		return *this;
	}


	bool Barcode::checksum( ) const
	{
		return d->mChecksumFlag;
	}


	void Barcode::render( Renderer& renderer )
	{
		renderer.render( d->mW, d->mH, d->mPrimitives );
	}


	bool Barcode::isEmpty( ) const
	{
		return d->mIsEmpty;
	}


	void Barcode::setIsEmpty( bool value )
	{
		d->mIsEmpty = value;
	}


	bool Barcode::isDataValid( ) const
	{
		return d->mIsDataValid;
	}


	void Barcode::setIsDataValid( bool value )
	{
		d->mIsDataValid = value;
	}


	double Barcode::width( ) const
	{
		return d->mW;
	}


	double Barcode::height( ) const
	{
		return d->mH;
	}


	void Barcode::setWidth( double w )
	{
		d->mW = w;
	}


	void Barcode::setHeight( double h )
	{
		d->mH = h;
	}


	void Barcode::clear( )
	{
		std::list<DrawingPrimitive*>::iterator primitive;

		for ( primitive = d->mPrimitives.begin(); primitive != d->mPrimitives.end(); primitive++ )
		{
			delete *primitive;
		}

		d->mPrimitives.clear();
	}


	void Barcode::addLine( double x, double y, double w, double h )
	{
		d->mPrimitives.push_back( new DrawingPrimitiveLine( x, y, w, h ) );
	}


	void Barcode::addBox( double x, double y, double w, double h )
	{
		d->mPrimitives.push_back( new DrawingPrimitiveBox( x, y, w, h ) );
	}


	void Barcode::addText( double x, double y, double size, const std::string& text, HAlign halign )
	{
		d->mPrimitives.push_back( new DrawingPrimitiveText( x, y, size, text, halign ) );
	}


	void Barcode::addRing( double x, double y, double r, double w )
	{
		d->mPrimitives.push_back( new DrawingPrimitiveRing( x, y, r, w ) );
	}


	void Barcode::addHexagon( double x, double y, double h )
	{
		d->mPrimitives.push_back( new DrawingPrimitiveHexagon( x, y, h ) );
	}


}
