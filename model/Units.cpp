//  Units.cpp
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


#include "Units.hpp"

#include <QDebug>
#include <QTextStream>


namespace glabels::model
{

        Units::Units( Units::Enum enumValue )
                : mEnumValue(enumValue)
        {
                switch (enumValue)
                {
                case PT:
                case IN:
                case MM:
                case CM:
                case PC:
                        /* Catch all valid enum values. */
                        break;
                default:
                        /* Catch invalid enum values, reset to PT. */
                        qWarning() << "Bad Units::Enum value = " << enumValue << ".";
                        mEnumValue = PT;
                        break;
                }
        }


        Units::Units( const QString& idString )
        {
                if ( idString == "pt" )
                {
                        mEnumValue = PT;
                }
                else if ( idString == "in" )
                {
                        mEnumValue = IN;
                }
                else if ( idString == "mm" )
                {
                        mEnumValue = MM;
                }
                else if ( idString == "cm" )
                {
                        mEnumValue = CM;
                }
                else if ( idString == "pc" )
                {
                        mEnumValue = PC;
                }
                else
                {
                        mEnumValue = PT;
                }
        }


        Units Units::pt()
        {
                return Units(PT);
        }


        Units Units::in()
        {
                return Units(IN);
        }


        Units Units::mm()
        {
                return Units(MM);
        }


        Units Units::cm()
        {
                return Units(CM);
        }


        Units Units::pc()
        {
                return Units(PC);
        }


        Units::Enum Units::toEnum() const
        {
                return mEnumValue;
        }


        QString Units::toIdString() const
        {
                QString idString;

                switch (mEnumValue)
                {
                case Units::PT:
                        idString = "pt";
                        break;
                case Units::IN:
                        idString = "in";
                        break;
                case Units::MM:
                        idString = "mm";
                        break;
                case Units::CM:
                        idString = "cm";
                        break;
                case Units::PC:
                        idString = "pc";
                        break;
                default:
                        idString = "pt";
                }

                return idString;
        }


        QString Units::toTrName() const
        {
                QString nameString;

                switch (mEnumValue)
                {
                case Units::PT:
                        nameString = tr("points");
                        break;
                case Units::IN:
                        nameString = tr("inches");
                        break;
                case Units::MM:
                        nameString = tr("mm");
                        break;
                case Units::CM:
                        nameString = tr("cm");
                        break;
                case Units::PC:
                        nameString = tr("picas");
                        break;
                default:
                        nameString = tr("points");
                }

                return nameString;
        }


        double Units::resolution() const
        {
                double value;

                switch (mEnumValue)
                {
                case Units::PT:
                        value = 0.01;
                        break;
                case Units::IN:
                        value = 0.001;
                        break;
                case Units::MM:
                        value = 0.01;
                        break;
                case Units::CM:
                        value = 0.001;
                        break;
                case Units::PC:
                        value = 0.01;
                        break;
                default:
                        value = 0.01;
                        break;
                }

                return value;
        }


        int Units::resolutionDigits() const
        {
                int digits;

                switch (mEnumValue)
                {
                case Units::PT:
                        digits = 2;
                        break;
                case Units::IN:
                        digits = 3;
                        break;
                case Units::MM:
                        digits = 2;
                        break;
                case Units::CM:
                        digits = 3;
                        break;
                case Units::PC:
                        digits = 2;
                        break;
                default:
                        digits = 2;
                        break;
                }

                return digits;
        }


        bool Units::isIdValid( const QString& idString )
        {
                bool retValue = false;

                if ( idString == "pt" )
                {
                        retValue = true;
                }
                else if ( idString == "in" )
                {
                        retValue = true;
                }
                else if ( idString == "mm" )
                {
                        retValue = true;
                }
                else if ( idString == "cm" )
                {
                        retValue = true;
                }
                else if ( idString == "pc" )
                {
                        retValue = true;
                }

                return retValue;
        }


}
