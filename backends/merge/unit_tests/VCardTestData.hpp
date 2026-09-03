

#include <QByteArray>


namespace VCardTestData
{
        //
        // Set 1:  Multiple cards
        //
        QByteArray set1 =
                "BEGIN:VCARD\r\n"
                "VERSION:3.0\r\n"
                "FN:Alice Anderson\r\n"
                "N:Anderson;Alice;;;\r\n"
                "EMAIL;TYPE=INTERNET:alice@example.com\r\n"
                "TEL;TYPE=CELL;TYPE=WORK:+1-555-0101\r\n"  // Multiple paramaters vs. list of values for a single parameter
                "END:VCARD\r\n"

                "BEGIN:VCARD\r\n"
                "VERSION:3.0\r\n"
                "FN:Bob Brown\r\n"
                "N:Brown;Bob;;;\r\n"
                "EMAIL;TYPE=INTERNET:bob@example.com\r\n"
                "TEL;TYPE=CELL:+1-555-0102\r\n"
                "END:VCARD\r\n"

                "BEGIN:vCard\r\n"
                "VERSION:3.0\r\n"
                "FN:Frank Dawson\r\n"
                "ORG:Lotus Development Corporation\r\n"
                "ADR;TYPE=WORK,POSTAL,PARCEL:;;6544 Battleford Drive\r\n" // Folded onto next line
                " ;Raleigh;NC;27613-3502;U.S.A.\r\n"                      // ...folded line
                "TEL;TYPE=VOICE,MSG,WORK:+1-919-676-9515\r\n"
                "TEL;TYPE=FAX,WORK:+1-919-676-9564\r\n"
                "EMAIL;TYPE=INTERNET,PREF:Frank_Dawson@Lotus.com\r\n"
                "EMAIL;TYPE=INTERNET:fdawson@earthlink.net\r\n"
                "URL:http://home.earthlink.net/~fdawson\r\n"
                "END:vCard\r\n"

                "BEGIN:VCARD\r\n"
                "VERSION:3.0\r\n"
                "FN:Dave Davis\r\n"
                "N:Davis;Dave;;;\r\n"
                "EMAIL;TYPE=INTERNET:dave@example.com\r\n"
                "TEL;TYPE=CELL:+1-555-0104\r\n"
                "END:VCARD\r\n";

        //
        // Set 2:  Multiple cards embedded in other text
        //
        // Simulates VCards embedded in a text document (e.g. email)
        //
        QByteArray set2 =
                "...Some text before the first VCard...\r\n"
                "...\r\n"

                "BEGIN:VCARD\r\n"
                "VERSION:3.0\r\n"
                "FN:Alice Anderson\r\n"
                "N:Anderson;Alice;;;\r\n"
                "EMAIL;TYPE=INTERNET:alice@example.com\r\n"
                "TEL;TYPE=CELL;TYPE=WORK:+1-555-0101\r\n"  // Multiple paramaters vs. list of values for a single parameter
                "END:VCARD\r\n"

                "...\r\n"
                "...Some text between VCards...\r\n"
                "...\r\n"

                "BEGIN:VCARD\r\n"
                "VERSION:3.0\r\n"
                "FN:Bob Brown\r\n"
                "N:Brown;Bob;;;\r\n"
                "EMAIL;TYPE=INTERNET:bob@example.com\r\n"
                "TEL;TYPE=CELL:+1-555-0102\r\n"
                "END:VCARD\r\n"

                "BEGIN:vCard\r\n"
                "VERSION:3.0\r\n"
                "FN:Frank Dawson\r\n"
                "ORG:Lotus Development Corporation\r\n"
                "ADR;TYPE=WORK,POSTAL,PARCEL:;;6544 Battleford Drive\r\n" // Folded onto next line
                " ;Raleigh;NC;27613-3502;U.S.A.\r\n"                      // ...folded line
                "TEL;TYPE=VOICE,MSG,WORK:+1-919-676-9515\r\n"
                "TEL;TYPE=FAX,WORK:+1-919-676-9564\r\n"
                "EMAIL;TYPE=INTERNET,PREF:Frank_Dawson@Lotus.com\r\n"
                "EMAIL;TYPE=INTERNET:fdawson@earthlink.net\r\n"
                "URL:http://home.earthlink.net/~fdawson\r\n"
                "END:vCard\r\n"

                "BEGIN:VCARD\r\n"
                "VERSION:3.0\r\n"
                "FN:Dave Davis\r\n"
                "N:Davis;Dave;;;\r\n"
                "EMAIL;TYPE=INTERNET:dave@example.com\r\n"
                "TEL;TYPE=CELL:+1-555-0104\r\n"
                "END:VCARD\r\n"

                "...after last VCard...\r\n"
                "...\r\n"
                "...\r\n";
}
