//  Constants.hpp
//
//  Copyright (C) 2016  Jaye Evins <evins@snaught.com>
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

#ifndef model_Constants_hpp
#define model_Constants_hpp


namespace glabels::model
{

        const double PTS_PER_PT    =  1.0;
        const double PTS_PER_INCH  = 72.0;
        const double PTS_PER_MM    =  2.83464566929;
        const double PTS_PER_CM    =  (10.0*PTS_PER_MM);
        const double PTS_PER_PICA  =  12.0;

        const Distance EPSILON( 0.5, Units::PT );

}


#endif // model_Constants_hpp
