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


        template <class T>
        void addToList( QList<T>& list, bool prepend, const T& value )
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


        VCardContact::NFields nExtractFields( const RawVCard::LineTokens& t )
        {
                VCardContact::NFields nFields;

                nFields.raw = t.value;

                if ( t.values.size() >= 1 ) nFields.family     = t.values[0];
                if ( t.values.size() >= 2 ) nFields.given      = t.values[1];
                if ( t.values.size() >= 3 ) nFields.additional = t.values[2];
                if ( t.values.size() >= 4 ) nFields.prefixes   = t.values[3];
                if ( t.values.size() >= 5 ) nFields.suffixes   = t.values[4];

                return nFields;
        }


        void addEmail( VCardContact::EmailStruct& email, const RawVCard::LineTokens& t )
        {
                bool isPref = RawVCard::paramsHas( t.params, "TYPE", "PREF" );

                if ( RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                {
                        addToList<QString>( email.work, isPref, t.value );
                }
                else if ( RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                {
                        addToList<QString>( email.home, isPref, t.value );
                }
                else
                {
                        // Other
                        addToList<QString>( email.other, isPref, t.value );
                }
        }


        void addTelType( VCardContact::TelLocation& loc, const RawVCard::LineTokens& t )
        {
                bool isPref = RawVCard::paramsHas( t.params, "TYPE", "PREF" );

                if ( RawVCard::paramsHas( t.params, "TYPE", "VOICE" ) )
                {
                        addToList<QString>( loc.voice, isPref, t.value );
                }
                if ( RawVCard::paramsHas( t.params, "TYPE", "CELL" ) )
                {
                        addToList<QString>( loc.cell, isPref, t.value );
                }
                else if ( RawVCard::paramsHas( t.params, "TYPE", "FAX" ) )
                {
                        addToList<QString>( loc.fax, isPref, t.value );
                }
                else if ( RawVCard::paramsHas( t.params, "TYPE", "PAGER" ) )
                {
                        addToList<QString>( loc.pager, isPref, t.value );
                }
                else if ( RawVCard::paramsHas( t.params, "TYPE", "MSG" ) )
                {
                        addToList<QString>( loc.msg, isPref, t.value );
                }
                else if ( RawVCard::paramsHas( t.params, "TYPE", "BBS" ) )
                {
                        addToList<QString>( loc.bbs, isPref, t.value );
                }
                else if ( RawVCard::paramsHas( t.params, "TYPE", "MODEM" ) )
                {
                        addToList<QString>( loc.modem, isPref, t.value );
                }
                else if ( RawVCard::paramsHas( t.params, "TYPE", "CAR" ) )
                {
                        addToList<QString>( loc.car, isPref, t.value );
                }
                else if ( RawVCard::paramsHas( t.params, "TYPE", "ISDN" ) )
                {
                        addToList<QString>( loc.isdn, isPref, t.value );
                }
                else if ( RawVCard::paramsHas( t.params, "TYPE", "VIDEO" ) )
                {
                        addToList<QString>( loc.video, isPref, t.value );
                }
                else if ( RawVCard::paramsHas( t.params, "TYPE", "PCS" ) )
                {
                        addToList<QString>( loc.pcs, isPref, t.value );
                }
                else if ( RawVCard::paramsHas( t.params, "TYPE", "MAIN" ) )
                {
                        addToList<QString>( loc.main, isPref, t.value );
                }
                else
                {
                        // Default to VOICE, if none of the above present
                        addToList<QString>( loc.voice, isPref, t.value );
                }
        }


        void addTel( VCardContact::TelStruct& tel, const RawVCard::LineTokens& t )
        {
                if ( RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                {
                        addTelType( tel.work, t );
                }
                else if ( RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                {
                        addTelType( tel.home, t );
                }
                else
                {
                        // Other
                        addTelType( tel.other, t );
                }
        }


        VCardContact::AdrFields adrExtractFields( const RawVCard::LineTokens& t )
        {
                VCardContact::AdrFields adrFields;

                adrFields.raw = t.value;

                if ( t.values.size() >= 1 ) adrFields.poBox      = t.values[0];
                if ( t.values.size() >= 2 ) adrFields.extended   = t.values[1];
                if ( t.values.size() >= 3 ) adrFields.street     = t.values[2];
                if ( t.values.size() >= 4 ) adrFields.locality   = t.values[3];
                if ( t.values.size() >= 5 ) adrFields.region     = t.values[4];
                if ( t.values.size() >= 6 ) adrFields.postalCode = t.values[5];
                if ( t.values.size() >= 7 ) adrFields.country    = t.values[6];

                return adrFields;
        }


        void addAdr( VCardContact::AdrStruct& adr, const RawVCard::LineTokens& t )
        {
                bool isPref = RawVCard::paramsHas( t.params, "TYPE", "PREF" );

                if ( RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                {
                        addToList( adr.work, isPref, adrExtractFields( t ) );
                }
                else if ( RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                {
                        addToList( adr.home, isPref, adrExtractFields( t ) );
                }
                else
                {
                        // Other
                        addToList( adr.other, isPref, adrExtractFields( t ) );
                }
        }


        void addLabel( VCardContact::LabelStruct& label, const RawVCard::LineTokens& t )
        {
                bool isPref = RawVCard::paramsHas( t.params, "TYPE", "PREF" );

                if ( RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                {
                        addToList<QString>( label.work, isPref, t.value );
                }
                else if ( RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                {
                        addToList<QString>( label.home, isPref, t.value );
                }
                else
                {
                        // Other
                        addToList<QString>( label.other, isPref, t.value );
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
                                c.n = nExtractFields( t );
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
                                addEmail( c.email, t );
                        }
                        else if ( t.name == "MAILER" )
                        {
                                c.mailer = t.value;
                        }
                        else if ( t.name == "TEL" )
                        {
                                addTel( c.tel, t );
                        }
                        else if ( t.name == "ADR" )
                        {
                                addAdr( c.adr, t );
                        }
                        else if ( t.name == "LABEL" )
                        {
                                addLabel( c.label, t );
                        }
                        else if ( t.name == "BDAY" )
                        {
                                c.bday = t.value;
                        }
                        else if ( t.name == "GEO" )
                        {
                                c.geo = t.value;
                                c.geoDms = formatLatLonDms( t.values );
                        }
                        else if ( t.name == "TZ" )
                        {
                                c.tz = t.value;
                        }
                        else if ( t.name == "URL" )
                        {
                                c.url = t.value;
                        }
                        else if ( t.name == "CATEGORIES" )
                        {
                                c.categories = t.value;
                        }
                        else if ( t.name == "NOTE" )
                        {
                                c.note.append( t.value );
                        }
                        else if ( t.name == "CLASS" )
                        {
                                c.classification = t.value;
                        }
                        else if ( t.name == "UID" )
                        {
                                c.uid = t.value;
                        }
                        else if ( t.name == "SORT-STRING" )
                        {
                                c.sortString = t.value;
                        }
                        else if ( t.name == "REV" )
                        {
                                c.rev = t.value;
                        }
                        else if ( t.name == "VERSION" )
                        {
                                c.version = t.value;
                        }
                        else if ( t.name == "PRODID" )
                        {
                                c.prodId = t.value;
                        }
                }

                return c;
        }


} // namespace glabels::merge
