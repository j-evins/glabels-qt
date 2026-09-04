//  Merge/VCardContact.cpp
//
//  Copyright (C) 2016-2026  Jaye Evins <evins@snaught.com>
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


#include "VCardContact.hpp"

#include "RawVCard.hpp"

#include <QDebug>

#include <utility>


namespace
{
        using namespace glabels::merge;


        void addToList( QStringList& list, bool prepend, const QByteArray& value )
        {
                if ( prepend )
                {
                        list.prepend( value );
                }
                else
                {
                        list.append( value );
                }
        }


        VCardContact::NFields nExtractFields( const QByteArrayList& values )
        {
                VCardContact::NFields nFields;

                if ( values.size() >= 1 ) nFields.family     = values[0];
                if ( values.size() >= 2 ) nFields.given      = values[1];
                if ( values.size() >= 3 ) nFields.additional = values[2];
                if ( values.size() >= 4 ) nFields.prefixes   = values[3];
                if ( values.size() >= 5 ) nFields.suffixes   = values[4];

                return nFields;
        }


        void addEmail( VCardContact::EmailStruct& email, const QList<RawVCard::ParamTokens>& params, const QByteArray& value )
        {
                bool isPref = RawVCard::paramsHas( params, "TYPE", "PREF" );

                if ( RawVCard::paramsHas( params, "TYPE", "WORK" ) )
                {
                        addToList( email.work, isPref, value );
                }
                else if ( RawVCard::paramsHas( params, "TYPE", "HOME" ) )
                {
                        addToList( email.home, isPref, value );
                }
                else
                {
                        addToList( email.other, isPref, value );
                }
        }


        void addTelType( VCardContact::TelLocation& loc, const QList<RawVCard::ParamTokens>& params, bool isPref, const QByteArray& value )
        {
                if ( RawVCard::paramsHas( params, "TYPE", "CELL" ) )
                {
                        addToList( loc.cell, isPref, value );
                }
                else if ( RawVCard::paramsHas( params, "TYPE", "FAX" ) )
                {
                        addToList( loc.fax, isPref, value );
                }
                else if ( RawVCard::paramsHas( params, "TYPE", "PAGER" ) )
                {
                        addToList( loc.pager, isPref, value );
                }
                else if ( RawVCard::paramsHas( params, "TYPE", "MSG" ) )
                {
                        addToList( loc.msg, isPref, value );
                }
                else if ( RawVCard::paramsHas( params, "TYPE", "BBS" ) )
                {
                        addToList( loc.bbs, isPref, value );
                }
                else if ( RawVCard::paramsHas( params, "TYPE", "MODEM" ) )
                {
                        addToList( loc.modem, isPref, value );
                }
                else if ( RawVCard::paramsHas( params, "TYPE", "CAR" ) )
                {
                        addToList( loc.car, isPref, value );
                }
                else if ( RawVCard::paramsHas( params, "TYPE", "ISDN" ) )
                {
                        addToList( loc.isdn, isPref, value );
                }
                else if ( RawVCard::paramsHas( params, "TYPE", "VIDEO" ) )
                {
                        addToList( loc.video, isPref, value );
                }
                else if ( RawVCard::paramsHas( params, "TYPE", "PCS" ) )
                {
                        addToList( loc.pcs, isPref, value );
                }
                else if ( RawVCard::paramsHas( params, "TYPE", "MAIN" ) )
                {
                        addToList( loc.main, isPref, value );
                }
                else
                {
                        addToList( loc.voice, isPref, value );
                }
        }


        void addTel( VCardContact::TelStruct& tel, const QList<RawVCard::ParamTokens>& params, const QByteArray& value )
        {
                bool isPref = RawVCard::paramsHas( params, "TYPE", "PREF" );

                if ( RawVCard::paramsHas( params, "TYPE", "WORK" ) )
                {
                        addTelType( tel.work, params, isPref, value );
                }
                else if ( RawVCard::paramsHas( params, "TYPE", "HOME" ) )
                {
                        addTelType( tel.home, params, isPref, value );
                }
                else
                {
                        addTelType( tel.other, params, isPref, value );
                }
        }


        QString formatLatLonDms( const QByteArrayList& values )
        {
                if ( values.size() < 2 ) return "";

                bool latOk{};
                bool lonOk{};
                auto lat = values[0].toDouble( & latOk );
                auto lon = values[1].toDouble( & lonOk );

                if ( !latOk || ( lat > 90.0 ) || ( lat < -90.0 ) ||
                     !lonOk && ( lon > 180.0 ) && ( lon < -180.0 ) )
                {
                        return "";
                }

                double absLat = fabs( lat );
                int latD = trunc( absLat );
                double latMFull = 60.0*( absLat - latD );
                int latM = trunc( latMFull );
                double latS = 60.0*( latMFull - latM );

                double absLon = fabs( lon );
                int lonD = trunc( absLon );
                double lonMFull = 60.0*( absLon - lonD );
                int lonM = trunc( lonMFull );
                double lonS = 60.0*( lonMFull - lonM );

                return QString( "%1 %2 %3 %4, %5 %6 %7 %8" )
                        .arg( latD, 2 )
                        .arg( latM, 2 )
                        .arg( latS, 6, 'f', 3 )
                        .arg( (lat<0.0) ? "S" : "N" )
                        .arg( lonD, 3 )
                        .arg( lonM, 2 )
                        .arg( lonS, 6, 'f', 3 )
                        .arg( (lon<0.0) ? "W" : "E" );
        }

}



namespace glabels::merge
{
        //
        // Build contact from raw vCard
        //
        VCardContact VCardContact::fromRawVCard( const QByteArrayList& rawVCard )
        {
                VCardContact c;

                for ( auto& line : rawVCard )
                {
                        auto t = RawVCard::tokenizeLine( line );

                        if ( t.name == "FN" )
                        {
                                c.fn = t.value;
                        }
                        else if ( t.name == "N" )
                        {
                                c.n = t.value;
                                c.nFields = nExtractFields( t.values );
                        }
                        else if ( t.name == "NICKNAME" )
                        {
                                c.nickname = t.value;
                        }
                        else if ( t.name == "ORG" )
                        {
                                c.org = t.value;
                        }
                        else if ( t.name == "TITLE" )
                        {
                                c.title = t.value;
                        }
                        else if ( t.name == "ROLE" )
                        {
                                c.role = t.value;
                        }
                        else if ( t.name == "EMAIL" )
                        {
                                addEmail( c.email, t.params, t.value );
                        }
                        else if ( t.name == "MAILER" )
                        {
                                c.mailer = t.value;
                        }
                        else if ( t.name == "TEL" )
                        {
                                addTel( c.tel, t.params, t.value );
                        }
                        else if ( t.name == "UID" )
                        {
                                c.uid = t.value;
                        }
                        else if ( t.name == "NOTE" )
                        {
                                c.note = t.value;
                        }
                        else if ( t.name == "PRODID" )
                        {
                                c.prodId = t.value;
                        }
                        else if ( t.name == "SORT-STRING" )
                        {
                                c.sortString = t.value;
                        }
                        else if ( t.name == "URL" )
                        {
                                c.url = t.value;
                        }
                        else if ( t.name == "CLASS" )
                        {
                                c.classification = t.value;
                        }
                        else if ( t.name == "GEO" )
                        {
                                c.geo = t.value;
                                c.geoDms = formatLatLonDms( t.values );
                        }
                }

                return c;
        }


} // namespace glabels::merge
