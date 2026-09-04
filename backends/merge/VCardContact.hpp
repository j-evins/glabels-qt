//  Merge/VCardContact.hpp
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

#ifndef merge_VCardContact_hpp
#define merge_VCardContact_hpp


#include <QByteArrayList>
#include <QIODevice>


namespace glabels::merge
{

        struct VCardContact
        {
                static VCardContact fromRawVCard( const QByteArrayList& rawVCard );

                QString fn;                // FN

                struct NFields
                {
                        QString family;           // N[0]
                        QString given;            // N[1]
                        QString additional;       // N[2] (e.g. middle name)
                        QString prefixes;         // N[3] (e.g. Ms.,Mr.Dr.)
                        QString suffixes;         // N[4] (e.g. M.D., Esq.,)
                };
                QString n;                 // N
                NFields nFields;           // Synthesized from N

                QString nickname;          // NICKNAME

                QString org;               // ORG
                QString title;             // TITLE
                QString role;              // ROLE

                struct EmailStruct
                {
                        QStringList work;     // EMAIL; TYPE=WORK
                        QStringList home;     // EMAIL; TYPE=HOME
                        QStringList other;    // EMAIL; TYPE!=WORK && TYPE!=HOME
                };
                EmailStruct email;         // EMAIL

                QString mailer;            // MAILER

                struct TelLocation
                {
                        QStringList voice;  // TEL;[TYPE=VOICE]
                        QStringList cell;   // TEL;TYPE=CELL
                        QStringList fax;    // TEL;TYPE=FAX
                        QStringList pager;  // TEL;TYPE=PAGER
                        QStringList msg;    // TEL;TYPE=MSG
                        QStringList bbs;    // TEL;TYPE=BBS
                        QStringList modem;  // TEL;TYPE=MODEM
                        QStringList car;    // TEL;TYPE=CAR
                        QStringList isdn;   // TEL;TYPE=ISDN
                        QStringList video;  // TEL;TYPE=VIDEO
                        QStringList pcs;    // TEL;TYPE=PCS
                        QStringList main;   // TEL;TYPE=MAIN
                };

                struct TelStruct
                {
                        TelLocation work;   // TEL;TYPE=WORK
                        TelLocation home;   // TEL;TYPE=HOME
                        TelLocation other;  // TEL;TYPE!=WORK&&TYPE!=HOME&&TYPE!=MAIN
                };
                TelStruct tel;             // TEL

                struct AdrFields
                {
                        QString poBox;      // ADR[0]
                        QString extended;   // ADR[1]
                        QString street;     // ADR[2]
                        QString locality;   // ADR[3]
                        QString postalCode; // ADR[4]
                        QString country;    // ADR[5]
                };

                struct AdrStruct
                {
                        QString work;           // ADR; TYPE=WORK
                        QString home;           // ADR; TYPE=HOME
                        QString other;          // ADR; TYPE!=WORK && TYPE!=HOME

                        AdrFields workFields;   // Synthesized from ADR; TYPE=WORK
                        AdrFields homeFields;   // Synthesized from ADR; TYPE=HOME
                        AdrFields otherFields;  // Synthesized from ADR; TYPE!=WORK && TYPE!=HOME
                };
                AdrStruct adr;             // ADR


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

}


#endif // merge_VCardContact_hpp
