/*  DrawingPrimitives.h
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

#ifndef glbarcode_DrawingPrimitives_h
#define glbarcode_DrawingPrimitives_h


#include "Enums.h"

#include <string>


namespace glbarcode
{

	/**
	 * @class DrawingPrimitive DrawingPrimitives.h glbarcode/DrawingPrimitives.h
	 *
	 * Base class for all drawing primitives
	 */
	class DrawingPrimitive
	{
	protected:
		/**
		 * Constructor
		 *
		 * @param[in] x X coordinate of primitive's origin (points)
		 * @param[in] y Y coordinate of primitive's origin (points)
		 */
		DrawingPrimitive( double x, double y );

	public:
		/**
		 * Destructor
		 */
		virtual ~DrawingPrimitive() = default;

		/*
		 * Non-copyable
		 */
		DrawingPrimitive( const DrawingPrimitive & ) = delete;
		void operator=( const DrawingPrimitive & ) = delete;

		/**
		 * Get X coordinate of primitive's origin (points).
		 */
		double x() const;

		/**
		 * Get Y coordinate of primitive's origin (points).
		 */
		double y() const;

	private:
		double mX;    /**< X coordinate of primitive's origin (points). */
		double mY;    /**< Y coordinate of primitive's origin (points). */
	};



	/**
	 * @class DrawingPrimitiveLine DrawingPrimitives.h glbarcode/DrawingPrimitives.h
	 *
	 * A solid vertical line drawing primitive.
	 *
	 * @image html figure-primitive-line.svg "Line primitive properties"
	 *
	 */
	class DrawingPrimitiveLine : public DrawingPrimitive
	{
	public:
		/**
		 * Line constructor
		 *
		 * @param[in] x X coordinate of line's origin (points)
		 * @param[in] y Y coordinate of line's origin (points)
		 * @param[in] w Line width (points)
		 * @param[in] h Line height (points)
		 */
		DrawingPrimitiveLine( double x, double y, double w, double h );

		/**
		 * Get line width (points).
		 */
		double w() const;

		/**
		 * Get line height (points).
		 */
		double h() const;

	private:
		double  mW;    /**< Line width (points). */
		double  mH;    /**< Line length (points). */
	};



	/**
	 * @class DrawingPrimitiveBox DrawingPrimitives.h glbarcode/DrawingPrimitives.h
	 *
	 * A solid box drawing primitive.
	 *
	 * @image html figure-primitive-box.svg "Box primitive properties"
	 *
	 */
	class DrawingPrimitiveBox : public DrawingPrimitive
	{
	public:
		/**
		 * Box constructor
		 *
		 * @param[in] x X coordinate of box's origin (points)
		 * @param[in] y Y coordinate of box's origin (points)
		 * @param[in] w Width of box (points)
		 * @param[in] h Height of box (points)
		 */
		DrawingPrimitiveBox( double x, double y, double w, double h );

		/**
		 * Get box width (points).
		 */
		double w() const;

		/**
		 * Get box height (points).
		 */
		double h() const;

	private:
		double  mW;    /**< Width of box (points). */
		double  mH;    /**< Height of box (points). */
	};



	/**
	 * @class DrawingPrimitiveText DrawingPrimitives.h glbarcode/DrawingPrimitives.h
	 *
	 * A character string drawing primitive.
	 *
	 * @image html figure-primitive-text.svg "Text primitive properties"
	 *
	 */
	class DrawingPrimitiveText : public DrawingPrimitive
	{
	public:
		/**
		 * Text constructor
		 *
		 * @param[in] x      X coordinate of text's origin (points)
		 * @param[in] y      Y coordinate of text's origin (points)
		 * @param[in] size   Font size of text (points)
		 * @param[in] text   Text
		 * @param[in] halign Horizontal text alignment
		 */
		DrawingPrimitiveText( double x, double y, double size, const std::string& text, HAlign halign = H_ALIGN_CENTER );

		/**
		 * Get font size (points).
		 */
		double size() const;

		/**
		 * Get text.
		 */
		const std::string& text() const;

		/**
		 * Get horizontal alignment.
		 */
		HAlign halign() const;

	private:
		double       mSize;    /**< Font size of text (points). */
		std::string  mText;    /**< Text. */
		HAlign       mHalign;  /**< Horizontal alignment. */
	};



	/**
	 * @class DrawingPrimitiveRing DrawingPrimitives.h glbarcode/DrawingPrimitives.h
	 *
	 * A ring (an open circle) drawing primitive.
	 *
	 * @image html figure-primitive-ring.svg "Ring primitive properties"
	 *
	 */
	class DrawingPrimitiveRing : public DrawingPrimitive
	{
	public:
		/**
		 * Ring constructor
		 *
		 * @param[in] x X coordinate of ring's origin (points)
		 * @param[in] y Y coordinate of ring's origin (points)
		 * @param[in] r Radius of ring (points)
		 * @param[in] w Line width of ring (points)
		 */
		DrawingPrimitiveRing( double x, double y, double r, double w );

		/**
		 * Get radius of ring (points).
		 */
		double r() const;

		/**
		 * Get line width (points).
		 */
		double w() const;

	private:
		double  mR;   /**< Radius of ring (points). */
		double  mW;   /**< Line width of ring (points). */
	};



	/**
	 * @class DrawingPrimitiveHexagon DrawingPrimitives.h glbarcode/DrawingPrimitives.h
	 *
	 * A solid regular hexagon (oriented with vertexes at top and bottom) drawing primitive.
	 *
	 * @image html figure-primitive-hexagon.svg "Hexagon primitive properties"
	 *
	 */
	class DrawingPrimitiveHexagon : public DrawingPrimitive
	{
	public:
		/**
		 * Hexagon constructor
		 *
		 * @param[in] x X coordinate of hexagon's origin (points)
		 * @param[in] y Y coordinate of hexagon's origin (points)
		 * @param[in] h Height of hexagon (points)
		 */
		DrawingPrimitiveHexagon( double x, double y, double h );

		/**
		 * Get Hexagon height (points).
		 */
		double h() const;

	private:
		double  mH;    /**< Height of hexagon (points). */
	};

}


#endif // glbarcode_DrawingPrimitives_h
