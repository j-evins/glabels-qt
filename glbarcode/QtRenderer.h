/*  QtRenderer.h
 *
 *  Copyright (C) 2017  Jaye Evins <evins@snaught.com>
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

#ifndef glbarcode_QtRenderer_h
#define glbarcode_QtRenderer_h


#include "Renderer.h"

#include <QPainter>


namespace glbarcode
{

	/**
	 * @class QtRenderer QtRenderer.h glbarcode/QtRenderer.h
	 *
	 * Render to QPainter context
	 */
	class QtRenderer : public Renderer
	{
	public:
                /**
                 * Default Constructor
                 */
		QtRenderer();
		
                /**
                 * Constructor with QPainter
                 */
		QtRenderer( QPainter* painter );

                /**
                 * Destructor
                 */
		~QtRenderer() override;

                /** Get "painter" parameter
                 *
                 * @returns painter parameter
                 */
		QPainter* painter() const;

                /** Set "painter" parameter
                 *
                 * @param[in] painter pointer to QPainter 
                 *
                 * @returns reference to this QtRenderer object for parameter chaining
                 */
		QtRenderer& setPainter( QPainter* painter );
		

	private:
		/*
                 * Virtual methods implemented by QtRenderer.
                 */
		void drawBegin( double w, double h ) override;
		void drawEnd() override;
		void drawLine( double x, double y, double w, double h ) override;
		void drawBox( double x, double y, double w, double h ) override;
		void drawText( double x, double y, double size, const std::string& text, HAlign halign = H_ALIGN_CENTER ) override;
		void drawRing( double x, double y, double r, double w ) override;
		void drawHexagon( double x, double y, double h ) override;

		/**
                 * Private data
                 */
		struct PrivateData;
		PrivateData *d;
	};

}

#endif // glbarcode_QtRenderer_h
