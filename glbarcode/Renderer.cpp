/*  Renderer.cpp
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

#include "Renderer.h"

#include "DrawingPrimitives.h"


void glbarcode::Renderer::render( double w, double h, const std::list<DrawingPrimitive*>& primitives )
{
	drawBegin( w, h );

	std::list<DrawingPrimitive*>::const_iterator primitive;

	for ( primitive = primitives.begin(); primitive != primitives.end(); primitive++ )
	{
		if ( auto* line = dynamic_cast<DrawingPrimitiveLine*>(*primitive) )
		{
			drawLine( line->x(), line->y(), line->w(), line->h() );
		}
		else if ( auto* box = dynamic_cast<DrawingPrimitiveBox*>(*primitive) )
		{
			drawBox( box->x(), box->y(), box->w(), box->h() );
		}
		else if ( auto* text = dynamic_cast<DrawingPrimitiveText*>(*primitive) )
		{
			drawText( text->x(), text->y(), text->size(), text->text(), text->halign() );
		}
		else if ( auto* ring = dynamic_cast<DrawingPrimitiveRing*>(*primitive) )
		{
			drawRing( ring->x(), ring->y(), ring->r(), ring->w() );
		}
		else if ( auto* hex = dynamic_cast<DrawingPrimitiveHexagon*>(*primitive) )
		{
			drawHexagon( hex->x(), hex->y(), hex->h() );
		}
		else
		{
			// NOT REACHED
		}
	}

	drawEnd();
}
