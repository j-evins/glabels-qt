//  Merge/VCardSimpleParser.cpp
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


#include "VCardSimpleParser.hpp"

#include "RawVCard.hpp"

#include <QDebug>

#include <utility>


namespace
{

        //
        // Structured contact
        //
        struct Contact
        {
                static Contact fromRawVCard( const QByteArrayList& rawVCard );

                QString fn;                // FN

                QString n;                 // N
                QString nGiven;            // synthesized from N
                QString nFamily;           // synthesized from N
                QString nAdditional;       // synthesized from N (e.g. middle name)
                QString nPrefixes;         // synthesized from N (e.g. Ms.,Mr.Dr.)
                QString nSuffixes;         // synthesized from N (e.g. M.D., Esq.,)

                QString nickname;          // NICKNAME

                QString org;               // ORG
                QString title;             // TITLE
                QString role;              // ROLE

                QStringList emailWork;     // EMAIL;TYPE=WORK
                QStringList emailHome;     // EMAIL;TYPE=HOME
                QStringList email;         // EMAIL;TYPE!=WORK&&TYPE!=HOME

                QString mailer;            // MAILER

                QStringList telWork;       // TEL;TYPE=WORK;[TYPE=VOICE]
                QStringList telWorkCell;   // TEL;TYPE=WORK;TYPE=CELL
                QStringList telWorkFax;    // TEL;TYPE=WORK;TYPE=FAX
                QStringList telWorkPager;  // TEL;TYPE=WORK;TYPE=PAGER
                QStringList telWorkMsg;    // TEL;TYPE=WORK;TYPE=MSG
                QStringList telWorkBbs;    // TEL;TYPE=WORK;TYPE=BBS
                QStringList telWorkModem;  // TEL;TYPE=WORK;TYPE=MODEM
                QStringList telWorkCar;    // TEL;TYPE=WORK;TYPE=CAR
                QStringList telWorkIsdn;   // TEL;TYPE=WORK;TYPE=ISDN
                QStringList telWorkVideo;  // TEL;TYPE=WORK;TYPE=VIDEO
                QStringList telWorkPcs;    // TEL;TYPE=WORK;TYPE=PCS

                QStringList telHome;       // TEL;TYPE=HOME;[TYPE=VOICE]
                QStringList telHomeCell;   // TEL;TYPE=HOME;TYPE=CELL
                QStringList telHomeFax;    // TEL;TYPE=HOME;TYPE=FAX
                QStringList telHomePager;  // TEL;TYPE=HOME;TYPE=PAGER
                QStringList telHomeMsg;    // TEL;TYPE=HOME;TYPE=MSG
                QStringList telHomeBbs;    // TEL;TYPE=HOME;TYPE=BBS
                QStringList telHomeModem;  // TEL;TYPE=HOME;TYPE=MODEM
                QStringList telHomeCar;    // TEL;TYPE=HOME;TYPE=CAR
                QStringList telHomeIsdn;   // TEL;TYPE=HOME;TYPE=ISDN
                QStringList telHomeVideo;  // TEL;TYPE=HOME;TYPE=VIDEO
                QStringList telHomePcs;    // TEL;TYPE=HOME;TYPE=PCS

                QStringList tel;           // TEL;TYPE=WORK;[TYPE=VOICE]
                QStringList telCell;       // TEL;TYPE=WORK;TYPE=CELL
                QStringList telFax;        // TEL;TYPE=WORK;TYPE=FAX
                QStringList telPager;      // TEL;TYPE=WORK;TYPE=PAGER
                QStringList telMsg;        // TEL;TYPE=WORK;TYPE=MSG
                QStringList telBbs;        // TEL;TYPE=WORK;TYPE=BBS
                QStringList telModem;      // TEL;TYPE=WORK;TYPE=MODEM
                QStringList telCar;        // TEL;TYPE=WORK;TYPE=CAR
                QStringList telIsdn;       // TEL;TYPE=WORK;TYPE=ISDN
                QStringList telVideo;      // TEL;TYPE=WORK;TYPE=VIDEO
                QStringList telPcs;        // TEL;TYPE=WORK;TYPE=PCS

                QStringList telMain;       // TEL;TYPE=MAIN

                QString adr;               // ADR
                QString adrHome;           // ADR;TYPE=HOME
                QString adrWork;           // ADR;TYPE=WORK


                QString classification;    // CLASS

                QString uid;               // UID
                QString sortString;        // SORT-STRING
                QString prodId;            // PRODID



                QString addressLabelHome;  // LABEL;TYPE=HOME or ADR;TYPE=HOME
                QString addressLabelWork;  // LABEL;TYPE=WORK or ADR;TYPE=WORK
                QString addressLabelOther; // LABEL;TYPE!=HOME;TYPE!=WORK or ADR;TYPE!=HOME;TYPE!=WORK

                QString url;               // URL

                QString categories;        // CATEGORIES

                QString note;              // NOTE

                QString rev;               // REV

                QString geo;               // GEO
                QString geoDms;            // Formated GEO as LatLon as DMS

                QString tz;                // TZ

        };


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


        //
        // Key-value pair
        //
        struct KeyValuePair
        {
                QString key;
                QString value;
        };


        KeyValuePair parseTokens( const glabels::merge::RawVCard::LineTokens& tokens )
        {
                //
                // Types not parsed into key-value pairs
                //
                //    - BEGIN
                //    - END
                //    - VERSION
                //    - NAME
                //    - PROFILE
                //    - SOURCE
                //    - PHOTO
                //    - LOGO
                //    - SOUND
                //    - KEY
                //    - AGENT

                //
                // Simple text types, no/ignore parameters, single text value
                //
                if ( ( tokens.name == "FN" )          ||
                     ( tokens.name == "MAILER" )      ||
                     ( tokens.name == "TITLE" )       ||
                     ( tokens.name == "ROLE" )        ||
                     ( tokens.name == "NOTE" )        ||
                     ( tokens.name == "PRODID" )      ||
                     ( tokens.name == "SORT-STRING" ) ||
                     ( tokens.name == "N" )           ||
                     ( tokens.name == "NICKNAME" )    ||
                     ( tokens.name == "UID" )         ||
                     ( tokens.name == "URL" )         ||
                     ( tokens.name == "TZ" )          ||
                     ( tokens.name == "ORG" )         ||
                     ( tokens.name == "BDAY" )        ||
                     ( tokens.name == "GEO" )         ||
                     ( tokens.name == "CATEGORIES" )  ||
                     ( tokens.name == "REV" )         ||
                     ( tokens.name == "CLASS" )       ||
                     ( tokens.name.startsWith( "X-" ) ) )
                {
                        return { tokens.name, tokens.value };
                }

                //
                // Email/Adress/Lable
                //
                if ( ( tokens.name == "EMAIL" ) ||
                     ( tokens.name == "TEL" )   ||
                     ( tokens.name == "ADR" )   ||
                     ( tokens.name == "LABEL" ) )
                {
                        QString qName = tokens.name;
                        for ( auto& paramTokens : tokens.params )
                        {
                                if ( paramTokens.name == "TYPE" )
                                {
                                        for ( auto& value : paramTokens.values )
                                        {
                                                qName += "_" + value.toUpper();
                                        }
                                }
                        }

                        return { qName, tokens.value };
                }

                return { "", "" };
        }


        Contact Contact::fromRawVCard( const QByteArrayList& rawVCard )
        {
                Contact c;

                for ( auto& line : rawVCard )
                {
                        auto t = glabels::merge::RawVCard::tokenizeLine( line );

                        if ( t.name == "FN" )
                        {
                                c.fn = t.value;
                                continue;
                        }
                        if ( t.name == "N" )
                        {
                                c.n = t.value;
                                if ( t.values.size() >= 1 ) c.nFamily     = t.values[0];
                                if ( t.values.size() >= 2 ) c.nGiven      = t.values[1];
                                if ( t.values.size() >= 3 ) c.nAdditional = t.values[2];
                                if ( t.values.size() >= 4 ) c.nPrefixes   = t.values[3];
                                if ( t.values.size() >= 5 ) c.nSuffixes   = t.values[4];
                                continue;
                        }
                        if ( t.name == "NICKNAME" )
                        {
                                c.nickname = t.value;
                                continue;
                        }
                        if ( t.name == "ORG" )
                        {
                                c.org = t.value;
                                continue;
                        }
                        if ( t.name == "TITLE" )
                        {
                                c.title = t.value;
                                continue;
                        }
                        if ( t.name == "ROLE" )
                        {
                                c.role = t.value;
                                continue;
                        }
                        if ( t.name == "EMAIL" )
                        {
                                bool isPref = glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "PREF" );

                                if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                                {
                                        addToList( c.emailWork, isPref, t.value );
                                }
                                else if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                                {
                                        addToList( c.emailHome, isPref, t.value );
                                }
                                else
                                {
                                        addToList( c.email, isPref, t.value );
                                }
                                continue;
                        }
                        if ( t.name == "MAILER" )
                        {
                                c.mailer = t.value;
                                continue;
                        }
                        if ( t.name == "TEL" )
                        {
                                bool isPref = glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "PREF" );

                                if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "CELL" ) )
                                {
                                        if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                                        {
                                                addToList( c.telWorkCell, isPref, t.value );
                                        }
                                        else if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                                        {
                                                addToList( c.telHomeCell, isPref, t.value );
                                        }
                                        else
                                        {
                                                addToList( c.telCell, isPref, t.value );
                                        }
                                        continue;
                                }
                                if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "FAX" ) )
                                {
                                        if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                                        {
                                                addToList( c.telWorkFax, isPref, t.value );
                                        }
                                        else if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                                        {
                                                addToList( c.telHomeFax, isPref, t.value );
                                        }
                                        else
                                        {
                                                addToList( c.telFax, isPref, t.value );
                                        }
                                        continue;
                                }
                                if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "PAGER" ) )
                                {
                                        if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                                        {
                                                addToList( c.telWorkPager, isPref, t.value );
                                        }
                                        else if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                                        {
                                                addToList( c.telHomePager, isPref, t.value );
                                        }
                                        else
                                        {
                                                addToList( c.telPager, isPref, t.value );
                                        }
                                        continue;
                                }
                                if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "MSG" ) )
                                {
                                        if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                                        {
                                                addToList( c.telWorkMsg, isPref, t.value );
                                        }
                                        else if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                                        {
                                                addToList( c.telHomeMsg, isPref, t.value );
                                        }
                                        else
                                        {
                                                addToList( c.telMsg, isPref, t.value );
                                        }
                                        continue;
                                }
                                if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "BBS" ) )
                                {
                                        if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                                        {
                                                addToList( c.telWorkBbs, isPref, t.value );
                                        }
                                        else if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                                        {
                                                addToList( c.telHomeBbs, isPref, t.value );
                                        }
                                        else
                                        {
                                                addToList( c.telBbs, isPref, t.value );
                                        }
                                        continue;
                                }
                                if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "MODEM" ) )
                                {
                                        if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                                        {
                                                addToList( c.telWorkModem, isPref, t.value );
                                        }
                                        else if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                                        {
                                                addToList( c.telHomeModem, isPref, t.value );
                                        }
                                        else
                                        {
                                                addToList( c.telModem, isPref, t.value );
                                        }
                                        continue;
                                }
                                if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "CAR" ) )
                                {
                                        if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                                        {
                                                addToList( c.telWorkCar, isPref, t.value );
                                        }
                                        else if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                                        {
                                                addToList( c.telHomeCar, isPref, t.value );
                                        }
                                        else
                                        {
                                                addToList( c.telCar, isPref, t.value );
                                        }
                                        continue;
                                }
                                if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "ISDN" ) )
                                {
                                        if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                                        {
                                                addToList( c.telWorkIsdn, isPref, t.value );
                                        }
                                        else if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                                        {
                                                addToList( c.telHomeIsdn, isPref, t.value );
                                        }
                                        else
                                        {
                                                addToList( c.telIsdn, isPref, t.value );
                                        }
                                        continue;
                                }
                                if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "VIDEO" ) )
                                {
                                        if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                                        {
                                                addToList( c.telWorkVideo, isPref, t.value );
                                        }
                                        else if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                                        {
                                                addToList( c.telHomeVideo, isPref, t.value );
                                        }
                                        else
                                        {
                                                addToList( c.telVideo, isPref, t.value );
                                        }
                                        continue;
                                }
                                if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "PCS" ) )
                                {
                                        if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                                        {
                                                addToList( c.telWorkPcs, isPref, t.value );
                                        }
                                        else if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                                        {
                                                addToList( c.telHomePcs, isPref, t.value );
                                        }
                                        else
                                        {
                                                addToList( c.telPcs, isPref, t.value );
                                        }
                                        continue;
                                }
                                if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "MAIN" ) )
                                {
                                        addToList( c.telMain, isPref, t.value );
                                        continue;
                                }
                                // VOICE
                                {
                                        if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "WORK" ) )
                                        {
                                                addToList( c.telWork, isPref, t.value );
                                        }
                                        else if ( glabels::merge::RawVCard::paramsHas( t.params, "TYPE", "HOME" ) )
                                        {
                                                addToList( c.telHome, isPref, t.value );
                                        }
                                        else
                                        {
                                                addToList( c.tel, isPref, t.value );
                                        }
                                        continue;
                                }
                        }
                        if ( t.name == "UID" )
                        {
                                c.uid = t.value;
                                continue;
                        }
                        if ( t.name == "NOTE" )
                        {
                                c.note = t.value;
                                continue;
                        }
                        if ( t.name == "PRODID" )
                        {
                                c.prodId = t.value;
                                continue;
                        }
                        if ( t.name == "SORT-STRING" )
                        {
                                c.sortString = t.value;
                                continue;
                        }
                        if ( t.name == "URL" )
                        {
                                c.url = t.value;
                                continue;
                        }
                        if ( t.name == "CLASS" )
                        {
                                c.classification = t.value;
                                continue;
                        }
                        if ( t.name == "GEO" )
                        {
                                c.geo = t.value;
                                c.geoDms = formatLatLonDms( t.values );
                                continue;
                        }
                }

                return c;
        }

}


namespace glabels::merge
{

        ///
        /// Parse raw vCard
        ///
        Record VCardSimpleParser::parseRawVCard( const QByteArrayList& buffer )
        {
                Record record;

                qDebug() << "";
                qDebug() << "#################################################";

                int i = 0;
                for ( auto& line : buffer )
                {
                        auto tokens = RawVCard::tokenizeLine( line );

#if 0
                        qDebug() << "----------------------------------------";

                        qDebug() << "valid = " << tokens.valid;
                        qDebug() << "group= " << tokens.group;
                        qDebug() << "name = " << tokens.name;
                        for ( auto& paramTokens : tokens.params )
                        {
                                qDebug() << "param = " << paramTokens.name;
                                qDebug() << "    paramValues = " << paramTokens.values;
                        }
                        qDebug() << "values = " << tokens.values;

                        qDebug() << "----------------------------------------";
#endif

                        auto pair = parseTokens( tokens );
                        if ( !pair.key.isEmpty() )
                        {
                                qDebug() << i << ". " << pair.key << " = " << pair.value;
                        }
                        else
                        {
                                qDebug() << i << ".";
                        }
                        i++;

                }

                return record;
        }

}
