//  GnuBarcode.cpp
//
//  Copyright (C) 2017  Jaye Evins <evins@snaught.com>
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

#if HAVE_GNU_BARCODE


#include "GnuBarcode.hpp"

#include <QDebug>

#include <cctype>

#include <barcode.h>


namespace
{
        const double INK_BLEED = 0.1; /* Shrink bars to account for ink bleed. */
        const double FONT_SCALE = 0.75; /* Shrink font slightly, otherwise too crowded. */
}


namespace glabels::barcode::GnuBarcode
{

        bool Base::isAscii( const std::string& data ) const
        {
                for (char c : data)
                {
                        if ( (c & 0x80) != 0 )
                        {
                                return false;
                        }
                }

                return true;
        }


        bool Base::isNumericLengthValid( const std::string& data,
                                         unsigned int       nMin,
                                         unsigned int       nMax ) const
        {
                unsigned int n = 0;

                for (char c : data)
                {
                        if ( isdigit(c) )
                        {
                                n++;
                        }
                }

                return (n >= nMin) && (n <= nMax);
        }


        bool Base::isNumericLength1Valid( const std::string& data,
                                          unsigned int       nMin,
                                          unsigned int       nMax ) const
        {
                unsigned int n = 0;

                for ( unsigned int i = 0; !isspace(data[i]) && (i < data.size()); i++ )
                {
                        if ( isdigit(data[i]) )
                        {
                                n++;
                        }
                        else if ( !isspace(data[i]) )
                        {
                                return false;
                        }
                }

                return (n >= nMin) && (n <= nMax);
        }


        bool Base::isNumericLength2Valid( const std::string& data,
                                          unsigned int       nMin,
                                          unsigned int       nMax ) const
        {
                unsigned int i;
                unsigned int n = 0;

                for ( i = 0; !isspace(data[i]) && (i < data.size()); i++ )
                {
                        /* Skip over 1st string */
                }

                for ( i++ /* skip space */ ; i < data.size(); i++ )
                {
                        if ( isdigit(data[i]) )
                        {
                                n++;
                        }
                        else
                        {
                                return false;
                        }
                }

                return (n >= nMin) && (n <= nMax);
        }


        void Base::vectorize( const std::string& encodedData,
                              const std::string& displayText,
                              const std::string& cookedData,
                              double&            w,
                              double&            h )
        {
                /*
                 * First encode using GNU Barcode library.
                 */
                Barcode_Item* bci = Barcode_Create( (char*)cookedData.c_str() );

                bci->scalef = 0;
                bci->width  = (int)w;
                bci->height = (int)h;

                bci->flags = flags;
                if ( !showText() )
                {
                        bci->flags |= BARCODE_NO_ASCII;
                }
                if ( !checksum() )
                {
                        bci->flags |= BARCODE_NO_CHECKSUM;
                }

                Barcode_Encode( bci, flags );

                if ( (bci->partial == nullptr) || (bci->textinfo == nullptr) )
                {
                        Barcode_Delete( bci );
                        setIsDataValid( false );
                        return;
                }


                /*
                 * Now do the actual vectorization.
                 *
                 * This code is based on the postscript renderer (ps.c) from the GNU barcode library:
                 *
                 *     Copyright (C) 1999 Alessaandro Rubini (rubini@gnu.org)
                 *     Copyright (C) 1999 Prosa Srl. (prosa@prosa.it)
                 */
                if (bci->width > (2*bci->margin))
                {
                        bci->width -= 2*bci->margin;
                }
                if (bci->height > (2*bci->margin))
                {
                        bci->height -= 2*bci->margin;
                }

                /* First calculate barlen */
                int barlen = bci->partial[0] - '0';
                for ( int i = 1; bci->partial[i] != 0; i++ )
                {
                        if ( isdigit(bci->partial[i]) )
                        {
                                barlen += bci->partial[i] - '0';
                        }
                        else
                        {
                                if ( (bci->partial[i] != '+') && (bci->partial[i] != '-') )
                                {
                                        barlen += bci->partial[i] - 'a' + 1;
                                }
                        }
                }

                /* The scale factor depends on bar length */
                double scalef = 1;
                if ( bci->scalef == 0 )
                {
                        if ( bci->width == 0 )
                        {
                                bci->width = barlen; /* default */
                        }
                        scalef = bci->scalef = (double)bci->width / (double)barlen;
                        if ( scalef < 0.5 )
                        {
                                scalef = 0.5;
                        }
                }

                /* The width defaults to "just enough" */
                bci->width = (int)( barlen * scalef + 1 );

                /* But it can be too small, in this case enlarge and center the area */
                if ( bci->width < (int)(barlen * scalef) )
                {
                        auto wid = int( barlen * scalef + 1);
                        bci->xoff -= (wid - bci->width)/2 ;
                        bci->width = wid;
                        /* Can't extend too far on the left */
                        if (bci->xoff < 0)
                        {
                                bci->width += -bci->xoff;
                                bci->xoff = 0;
                        }
                }

                /* The height defaults to 80 points (rescaled) */
                if ( bci->height == 0 )
                {
                        bci->height = (int)( 80 * scalef );
                }

                /* If too small (5 + text), reduce the scale factor and center */
                int i = 5 + 10 * ( ((bci->flags & BARCODE_NO_ASCII)==0) ? 1 : 0 );
                if ( bci->height < (int)(i * scalef) )
                {
                        bci->height = (int)( i * scalef );
                }

                /* Now traverse the code string and create a list of lines */
                char mode = '-'; /* text below bars */
                double x = bci->margin + (bci->partial[0] - '0') * scalef;
                i = 1;
                for ( int ip = 1; bci->partial[ip] != 0; ip++, i++)
                {
                        /* special cases: '+' and '-' */
                        if ( bci->partial[ip] == '+' || bci->partial[ip] == '-' )
                        {
                                mode = bci->partial[ip];        /* don't count it */
                                i++;
                                continue;
                        }
                        /* j is the width of this bar/space */
                        int j;
                        if ( isdigit(bci->partial[ip]) )
                        {
                                j = bci->partial[ip] - '0';
                        }
                        else
                        {
                                j = bci->partial[ip] - 'a' + 1;
                        }
                        if ( (i % 2) != 0 )
                        {
                                /* bar */
                                double x0 = x;
                                double y0 = bci->margin;
                                double yr = bci->height;
                                if ( (bci->flags & BARCODE_NO_ASCII) == 0 )
                                {
                                        /* leave space for text */
                                        if (mode == '-')
                                        {
                                                /* text below bars: 10 or 5 points */
                                                yr -= (isdigit(bci->partial[ip]) ? 10 : 5) * scalef;
                                        }
                                        else
                                        {
                                                /* '+' */
                                                /* above bars: 10 or 0 from bottom,
                                                   and 10 from top */
                                                y0 += 10 * scalef;
                                                yr -= (isdigit(bci->partial[ip]) ? 20 : 10) * scalef;
                                        }
                                }
                                addBox( x0, y0, (j * scalef) - INK_BLEED, yr );
                        }
                        x += j * scalef;

                }

                /* Now the text */
                mode = '-';                /* reinstantiate default */
                if ( (bci->flags & BARCODE_NO_ASCII) == 0 )
                {
                        int len = strlen( bci->textinfo );
                        for ( int i = 0; i < len; i++ )
                        {
                                if ( bci->textinfo[i] == ' ' )
                                {
                                        continue;
                                }
                                if ( (bci->textinfo[i] == '+') || (bci->textinfo[i] == '-') )
                                {
                                        mode = bci->textinfo[i];
                                        continue;
                                }
                                double f1, f2;
                                char   c;
                                if ( sscanf( &bci->textinfo[i], "%lf:%lf:%c", &f1, &f2, &c) != 3 )
                                {
                                        qDebug() << "Impossible data:" << QString(&bci->textinfo[i]);
                                        continue;
                                }
                                f2 *= FONT_SCALE;
                                double x0 = f1*scalef + bci->margin + 0.4*f2*scalef;
                                double y0;
                                if (mode == '-')
                                {
                                        y0 = bci->margin + bci->height - 8*scalef + 0.75*f2*scalef;
                                }
                                else
                                {
                                        y0 = bci->margin + 0.75*f2*scalef;
                                }
                                addText( x0, y0, f2*scalef, QString(c).toStdString() );

                                /* skip past the substring we just read. */
                                while ( (bci->textinfo[i] != ' ') && (bci->textinfo[i] != 0) )
                                {
                                        i++;
                                }
                        }
                }

                /* Fill in other info */
                w = bci->width  + 2.0*bci->margin;
                h = bci->height + 2.0*bci->margin;

                /* Cleanup */
                Barcode_Delete( bci );
        }


        //////////////////////////////////////////////////////
        // EAN Barcode (Any)
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Ean::create()
        {
                return new Ean();
        }


        bool Ean::validate( const std::string& rawData )
        {
                return (      isNumericLengthValid(  rawData,  7,  8 )
                              ||   isNumericLengthValid(  rawData, 12, 13 )
                              || ( isNumericLength1Valid( rawData,  7,  8 ) && isNumericLength2Valid( rawData, 2, 2 ) )
                              || ( isNumericLength1Valid( rawData,  7,  8 ) && isNumericLength2Valid( rawData, 5, 5 ) )
                              || ( isNumericLength1Valid( rawData, 12, 13 ) && isNumericLength2Valid( rawData, 2, 2 ) )
                              || ( isNumericLength1Valid( rawData, 12, 13 ) && isNumericLength2Valid( rawData, 5, 5 ) ) );
        }


        std::string Ean::encode( const std::string& cookedData )
        {
                flags = BARCODE_EAN;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // EAN-8 Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Ean8::create()
        {
                return new Ean8();
        }


        bool Ean8::validate( const std::string& rawData )
        {
                return isNumericLengthValid(  rawData,  7,  8 );
        }


        std::string Ean8::encode( const std::string& cookedData )
        {
                flags = BARCODE_EAN;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // EAN-8+2 Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Ean8_2::create()
        {
                return new Ean8_2();
        }


        bool Ean8_2::validate( const std::string& rawData )
        {
                return isNumericLength1Valid( rawData,  7,  8 ) && isNumericLength2Valid( rawData, 2, 2 );
        }


        std::string Ean8_2::encode( const std::string& cookedData )
        {
                flags = BARCODE_EAN;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // EAN-8+5 Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Ean8_5::create()
        {
                return new Ean8_5();
        }


        bool Ean8_5::validate( const std::string& rawData )
        {
                return isNumericLength1Valid( rawData,  7,  8 ) && isNumericLength2Valid( rawData, 5, 5 );
        }


        std::string Ean8_5::encode( const std::string& cookedData )
        {
                flags = BARCODE_EAN;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // EAN-13 Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Ean13::create()
        {
                return new Ean13();
        }


        bool Ean13::validate( const std::string& rawData )
        {
                return isNumericLengthValid(  rawData, 12, 13 );
        }


        std::string Ean13::encode( const std::string& cookedData )
        {
                flags = BARCODE_EAN;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // EAN-13+2 Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Ean13_2::create()
        {
                return new Ean13_2();
        }


        bool Ean13_2::validate( const std::string& rawData )
        {
                return isNumericLength1Valid( rawData, 12, 13 ) && isNumericLength2Valid( rawData, 2, 2 );
        }


        std::string Ean13_2::encode( const std::string& cookedData )
        {
                flags = BARCODE_EAN;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // EAN-13+5 Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Ean13_5::create()
        {
                return new Ean13_5();
        }


        bool Ean13_5::validate( const std::string& rawData )
        {
                return isNumericLength1Valid( rawData, 12, 13 ) && isNumericLength2Valid( rawData, 5, 5 );
        }


        std::string Ean13_5::encode( const std::string& cookedData )
        {
                flags = BARCODE_EAN;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // UPC Barcode (Any)
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Upc::create()
        {
                return new Upc();
        }


        bool Upc::validate( const std::string& rawData )
        {
                return      isNumericLengthValid( rawData, 6, 8 )
                        ||  isNumericLengthValid( rawData, 11, 12 )
                        || (isNumericLength1Valid( rawData, 6, 8 ) && isNumericLength2Valid( rawData, 2, 2 ))
                        || (isNumericLength1Valid( rawData, 6, 8 ) && isNumericLength2Valid( rawData, 5, 5 ))
                        || (isNumericLength1Valid( rawData, 11, 12 ) && isNumericLength2Valid( rawData, 2, 2 ))
                        || (isNumericLength1Valid( rawData, 11, 12 ) && isNumericLength2Valid( rawData, 5, 5 ));
        }


        std::string Upc::encode( const std::string& cookedData )
        {
                flags = BARCODE_UPC;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // UPC-A Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* UpcA::create()
        {
                return new UpcA();
        }


        bool UpcA::validate( const std::string& rawData )
        {
                return isNumericLengthValid( rawData, 11, 12 );
        }


        std::string UpcA::encode( const std::string& cookedData )
        {
                flags = BARCODE_UPC;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // UPC-A+2 Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* UpcA_2::create()
        {
                return new UpcA_2();
        }


        bool UpcA_2::validate( const std::string& rawData )
        {
                return isNumericLength1Valid( rawData, 11, 12 ) && isNumericLength2Valid( rawData, 2, 2 );
        }


        std::string UpcA_2::encode( const std::string& cookedData )
        {
                flags = BARCODE_UPC;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // UPC-A+5 Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* UpcA_5::create()
        {
                return new UpcA_5();
        }


        bool UpcA_5::validate( const std::string& rawData )
        {
                return isNumericLength1Valid( rawData, 11, 12 ) && isNumericLength2Valid( rawData, 5, 5 );
        }


        std::string UpcA_5::encode( const std::string& cookedData )
        {
                flags = BARCODE_UPC;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // UPC-E Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* UpcE::create()
        {
                return new UpcE();
        }


        bool UpcE::validate( const std::string& rawData )
        {
                return isNumericLengthValid( rawData, 6, 8 );
        }


        std::string UpcE::encode( const std::string& cookedData )
        {
                flags = BARCODE_UPC;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // UPC-E+2 Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* UpcE_2::create()
        {
                return new UpcE_2();
        }


        bool UpcE_2::validate( const std::string& rawData )
        {
                return isNumericLength1Valid( rawData, 6, 8 ) && isNumericLength2Valid( rawData, 2, 2 );
        }


        std::string UpcE_2::encode( const std::string& cookedData )
        {
                flags = BARCODE_UPC;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // UPC-E+5 Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* UpcE_5::create()
        {
                return new UpcE_5();
        }


        bool UpcE_5::validate( const std::string& rawData )
        {
                return isNumericLength1Valid( rawData, 6, 8 ) && isNumericLength2Valid( rawData, 5, 5 );
        }


        std::string UpcE_5::encode( const std::string& cookedData )
        {
                flags = BARCODE_UPC;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // ISBN Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Isbn::create()
        {
                return new Isbn();
        }


        bool Isbn::validate( const std::string& rawData )
        {
                return isNumericLengthValid( rawData, 9, 10 );
        }


        std::string Isbn::encode( const std::string& cookedData )
        {
                flags = BARCODE_ISBN;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // ISBN+5 Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Isbn_5::create()
        {
                return new Isbn_5();
        }


        bool Isbn_5::validate( const std::string& rawData )
        {
                return isNumericLength1Valid( rawData, 9, 10 ) && isNumericLength2Valid( rawData, 5, 5 );
        }


        std::string Isbn_5::encode( const std::string& cookedData )
        {
                flags = BARCODE_ISBN;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // Code39 Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Code39::create()
        {
                return new Code39();
        }


        bool Code39::validate( const std::string& rawData )
        {
                return isAscii( rawData );
        }


        std::string Code39::encode( const std::string& cookedData )
        {
                flags = BARCODE_39;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // Code128 Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Code128::create()
        {
                return new Code128();
        }


        bool Code128::validate( const std::string& rawData )
        {
                return isAscii( rawData );
        }


        std::string Code128::encode( const std::string& cookedData )
        {
                flags = BARCODE_128;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // Code128C Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Code128C::create()
        {
                return new Code128C();
        }


        bool Code128C::validate( const std::string& rawData )
        {
                return isAscii( rawData );
        }


        std::string Code128C::encode( const std::string& cookedData )
        {
                flags = BARCODE_128C;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // Code128B Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Code128B::create()
        {
                return new Code128B();
        }


        bool Code128B::validate( const std::string& rawData )
        {
                return isAscii( rawData );
        }


        std::string Code128B::encode( const std::string& cookedData )
        {
                flags = BARCODE_128B;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // I25 Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* I25::create()
        {
                return new I25();
        }


        bool I25::validate( const std::string& rawData )
        {
                return isAscii( rawData );
        }


        std::string I25::encode( const std::string& cookedData )
        {
                flags = BARCODE_I25;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // CBR Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Cbr::create()
        {
                return new Cbr();
        }


        bool Cbr::validate( const std::string& rawData )
        {
                return isAscii( rawData );
        }


        std::string Cbr::encode( const std::string& cookedData )
        {
                flags = BARCODE_CBR;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // MSI Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Msi::create()
        {
                return new Msi();
        }


        bool Msi::validate( const std::string& rawData )
        {
                return isAscii( rawData );
        }


        std::string Msi::encode( const std::string& cookedData )
        {
                flags = BARCODE_MSI;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // PLS Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Pls::create()
        {
                return new Pls();
        }


        bool Pls::validate( const std::string& rawData )
        {
                return isAscii( rawData );
        }


        std::string Pls::encode( const std::string& cookedData )
        {
                flags = BARCODE_PLS;
                return ""; // Actual encoding is done in vectorize
        }


        //////////////////////////////////////////////////////
        // Code93 Barcode
        //////////////////////////////////////////////////////
        glbarcode::Barcode* Code93::create()
        {
                return new Code93();
        }


        bool Code93::validate( const std::string& rawData )
        {
                return isAscii( rawData );
        }


        std::string Code93::encode( const std::string& cookedData )
        {
                flags = BARCODE_93;
                return ""; // Actual encoding is done in vectorize
        }


}


#endif // HAVE_GNU_BARCODE
