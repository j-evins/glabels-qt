/*  Renderer.h
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

#ifndef glbarcode_Renderer_h
#define glbarcode_Renderer_h


#include <list>

#include "DrawingPrimitives.h"


namespace glbarcode
{

	class DrawingPrimitive; /* Forward reference to private drawing primitive class. */


	/**
	 * @class Renderer Renderer.h glbarcode/Renderer.h
	 *
	 * Base class for all renderers.
	 */
	class Renderer
	{
	protected:
		/**
		 * Default constructor.
		 */
		Renderer() = default;

	public:
		/**
		 * Destructor.
		 */
		virtual ~Renderer() = default;

		/*
		 * Non-copyable
		 */
		Renderer( const Renderer & ) = delete;
		void operator=( const Renderer & ) = delete;

		/**
		 * Render list of primitives.
		 *
		 * @param[in] w          Width of barcode bounding box (points)
		 * @param[in] h          Height of barcode bounding box (points)
		 * @param[in] primitives List of drawing primitives
		 */
		void render( double w, double h, const std::list<DrawingPrimitive*>& primitives );


	protected:
		/**
		 * Draw begin.
		 * 
		 * Required virtual method to perform rendering setup, such as opening devices and/or
		 * initializing drawing contexts.
		 *
		 * @param[in] w Width of barcode bounding box (points)
		 * @param[in] h Height of barcode bounding box (points)
		 */
		virtual void drawBegin( double w, double h ) = 0;


		/**
		 * Draw end.
		 * 
		 * Required virtual method to perform rendering cleanup, such as closing devices
		 * and/or drawing contexts.
		 */
		virtual void drawEnd() = 0;


		/**
		 * Draw line primitive.
		 * 
		 * Required virtual method to draw or render line drawing primitive.
		 *
		 * @image html figure-primitive-line.svg "Line primitive properties"
		 *
		 * @param[in] x X coordinate of line's origin (points)
		 * @param[in] y Y coordinate of line's origin (points)
		 * @param[in] w Line width (points)
		 * @param[in] h Line height (points)
		 */
		virtual void drawLine( double x, double y, double w, double h ) = 0;


		/**
		 * Draw box primitive.
		 * 
		 * Required virtual method to draw or render box drawing primitive.
		 *
		 * @image html figure-primitive-box.svg "Box primitive properties"
		 *
		 * @param[in] x X coordinate of box's origin (points)
		 * @param[in] y Y coordinate of box's origin (points)
		 * @param[in] w Width of box (points)
		 * @param[in] h Height of box (points)
		 */
		virtual void drawBox( double x, double y, double w, double h ) = 0;


		/**
		 * Draw text primitive.
		 * 
		 * Required virtual method to draw or render text drawing primitive.
		 *
		 * @image html figure-primitive-text.svg "Text primitive properties"
		 *
		 * @param[in] x    X coordinate of text's origin (points)
		 * @param[in] y    Y coordinate of text's origin (points)
		 * @param[in] size Font size of text (points)
		 * @param[in] text Text
		 */
		virtual void drawText( double x, double y, double size, const std::string& text, HAlign halign = H_ALIGN_CENTER ) = 0;


		/**
		 * Draw ring primitive.
		 * 
		 * Required virtual method to draw or render ring drawing primitive.
		 *
		 * @image html figure-primitive-ring.svg "Ring primitive properties"
		 *
		 * @param[in] x X coordinate of ring's origin (points)
		 * @param[in] y Y coordinate of ring's origin (points)
		 * @param[in] r Radius of ring (points)
		 * @param[in] w Line width of ring (points)
		 */
		virtual void drawRing( double x, double y, double r, double w ) = 0;


		/**
		 * Draw hexagon primitive.
		 * 
		 * Required virtual method to draw or render hexagon drawing primitive.
		 *
		 * @image html figure-primitive-hexagon.svg "Hexagon primitive properties"
		 *
		 * @param[in] x X coordinate of hexagon's origin (points)
		 * @param[in] y Y coordinate of hexagon's origin (points)
		 * @param[in] h Height of hexagon (points)
		 */
		virtual void drawHexagon( double x, double y, double h ) = 0;

	};

}

#endif // glbarcode_Renderer_h
