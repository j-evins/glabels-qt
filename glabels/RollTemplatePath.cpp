//  RollTemplatePath.cpp
//
//  Copyright (C) 2014-2026  Jaye Evins <evins@snaught.com>
//
//  This file is part of gLabels-qt.
//
//  gLabels-qt is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  gLabels-qt is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
//


#include "RollTemplatePath.hpp"

#include <QDebug>


namespace glabels
{

        ///
        /// Constructor
        ///
        RollTemplatePath::RollTemplatePath( const model::Template& tmplate )
        {
                if ( !tmplate.isRoll() )
                {
                        qWarning() << "Not a \"Roll\" template type.";
                }

                model::Distance x0 = (tmplate.pageWidth() - tmplate.rollWidth()) / 2.0;
                model::Distance w = tmplate.rollWidth();
                model::Distance h = tmplate.pageHeight();
                model::Distance c = 0.07*h;
                model::Distance b = 0.03*h;

                const int nx = 18;

                // Upper break line
                moveTo( x0.pt(), -c.pt() );
                for ( int ix = 1; ix <= nx; ix++ )
                {
                        model::Distance x = ix * (w/double(nx)) + x0;
                        double          a = ix * (2*M_PI/double(nx));

                        lineTo( x.pt(), b.pt()*sin(a) - c.pt() );
                }

                // Lower break line
                lineTo( (x0+w).pt(), (h+c).pt() );
                for ( int ix = nx-1; ix >= 0; ix-- )
                {
                        model::Distance x = ix * (w/double(nx)) + x0;
                        double          a = ix * (2*M_PI/double(nx));

                        lineTo( x.pt(), b.pt()*sin(a) + h.pt() + c.pt() );
                }

                // Close path
                closeSubpath();
        }

} // namespace glabels
