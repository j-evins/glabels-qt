/*  Cursors.cpp
 *
 *  Copyright (C) 2013  Jaye Evins <evins@snaught.com>
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

#include "Cursors.h"

#include <QPixmap>


namespace glabels
{

	Cursors::Crosshair::Crosshair()
		: QCursor( QPixmap(":cursors/32x32/cursor_crosshair.png"), 15, 15 )
	{
		// empty
	}


	Cursors::Move::Move()
		: QCursor( QPixmap(":cursors/32x32/cursor_move.png"), 15, 15 )
	{
		// empty
	}


	Cursors::Barcode::Barcode()
		: QCursor( QPixmap(":cursors/32x32/cursor_barcode.png"), 7, 7 )
	{
		// empty
	}


	Cursors::Box::Box()
		: QCursor( QPixmap(":cursors/32x32/cursor_box.png"), 7, 7 )
	{
		// empty
	}


	Cursors::Ellipse::Ellipse()
		: QCursor( QPixmap(":cursors/32x32/cursor_ellipse.png"), 7, 7 )
	{
		// empty
	}


	Cursors::Image::Image()
		: QCursor( QPixmap(":cursors/32x32/cursor_image.png"), 7, 7 )
	{
		// empty
	}


	Cursors::Line::Line()
		: QCursor( QPixmap(":cursors/32x32/cursor_line.png"), 7, 7 )
	{
		// empty
	}


	Cursors::Text::Text()
		: QCursor( QPixmap(":cursors/32x32/cursor_text.png"), 7, 7 )
	{
		// empty
	}

} // namespace glabels
