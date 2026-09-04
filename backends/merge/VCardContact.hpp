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

        // The following struct attempts to impose a generic implementation of a vCard contact
        // that could come from many different programs, each with its own conventions on how
        // to organize and possible collapse this data.
        //
        // Assumption 1: There will be no more than one instance of the following lines in
        //               single vCard:
        //
        //                   FN, N, NICKNAME, ORG, TITLE, ROLE, MAILER, CLASS, UID,
        //                   SORT-STRING, PRODID, URL, CATEGORIES, NOTE, REV, GEO,
        //                   and TZ.
        //
        // Assumption 2: There may be any number of the following lines in single vCard:
        //
        //                   EMAIL, TEL, ADR, and LABEL
        //
        // Assumption 3: The EMAIL, TEL, ADR, and LABEL lines will be broken out into
        //               a fixed heirarchical set of structures, based on TYPE parameters.
        //               The top-level break-out of these lines is
        //
        //                   TYPE=WORK -> .work
        //                   TYPE=HOME -> .home
        //                   default   -> .other
        //
        //               If both TYPE=WORK and TYPE=HOME are present, WORK will take
        //               precidence.
        //
        //               The second level break-out of the TEL line, will be one of
        //
        //                   CELL, FAX, PAGER, MSG, BBS, MODEM, CAR, ISDN, VIDEO, PCS,
        //                   MAIN, and VOICE (voice is the default if none of these
        //                   are present.)
        //
        //               E.g. tel.work.cell or tel.home.voice
        //
        // Assumption 4: Since after breaking out heirarchically, there may be more than one
        //               line with the same classification present, a QList of the interpreted
        //               line value will be kept.
        //
        //               The TYPE=PREF parameter will trigger the line being prepended to
        //               the list, rather than being appended.
        //
        // Assumption 5: For structural lines N and ADR, separate fields are created for the
        //               raw value, and individual fields for each subfield.
        //
        // Assumption 6: Ignoring groups.  These are usually used to tie application specific
        //               meta data to lines, using X-* lines.
        //
        // Assumption 7: Ignoring application specific, X-*, lines.
        //

        struct VCardContact
        {
                static VCardContact fromRawVCard( const QByteArrayList& rawVCard );

                QString fn;                // FN

                struct NFields
                {
                        QString raw;              // N
                        QString family;           // N[0]
                        QString given;            // N[1]
                        QString additional;       // N[2] (e.g. middle name)
                        QString prefixes;         // N[3] (e.g. Ms.,Mr.Dr.)
                        QString suffixes;         // N[4] (e.g. M.D., Esq.,)
                };
                NFields n;                 // Synthesized from N

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
                        QString raw;        // ADR
                        QString poBox;      // ADR[0]
                        QString extended;   // ADR[1]
                        QString street;     // ADR[2]
                        QString locality;   // ADR[3]
                        QString region;     // ADR[4]
                        QString postalCode; // ADR[5]
                        QString country;    // ADR[6]
                };

                struct AdrStruct
                {
                        QList<AdrFields> work;   // ADR;TYPE=WORK
                        QList<AdrFields> home;   // ADR;TYPE=HOME
                        QList<AdrFields> other;  // ADR;TYPE=OTHER
                };
                AdrStruct adr;             // ADR

                struct LabelStruct
                {
                        QStringList work;   // LABEL;TYPE=WORK
                        QStringList home;   // LABEL;TYPE=HOME
                        QStringList other;  // LABEL;TYPE=OTHER
                };
                LabelStruct label;         // LABEL

                QString bday;              // BDAY

                QString geo;               // GEO
                QString geoDms;            // Formated GEO as LatLon as DMS

                QString tz;                // TZ

                QString url;               // URL

                QString categories;        // CATEGORIES

                QStringList note;          // NOTE

                QString classification;    // CLASS
                QString uid;               // UID
                QString sortString;        // SORT-STRING
                QString rev;               // REV
                QString version;           // VERSION
                QString prodId;            // PRODID
        };

}


#endif // merge_VCardContact_hpp
