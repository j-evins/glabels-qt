//  GnuBarcode.hpp
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

#ifndef barcode_GnuBarcode_hpp
#define barcode_GnuBarcode_hpp

#if HAVE_GNU_BARCODE


#include "glbarcode/Barcode1dBase.hpp"


namespace glabels::barcode::GnuBarcode
{

        /**
         * GnuBarcode::Base base class for all GNU Barcodes
         *
         * Implements glbarcode::Barcode1dBase.
         */
        class Base : public glbarcode::Barcode1dBase
        {
        protected:
                int flags;

                bool isAscii( const std::string& data ) const;

                bool isNumericLengthValid( const std::string& data,
                                           unsigned int       nMin,
                                           unsigned int       nMax ) const;

                bool isNumericLength1Valid( const std::string& data,
                                            unsigned int       nMin,
                                            unsigned int       nMax ) const;

                bool isNumericLength2Valid( const std::string& data,
                                            unsigned int       nMin,
                                            unsigned int       nMax ) const;

                void vectorize( const std::string& encodedData,
                                const std::string& displayText,
                                const std::string& cookedData,
                                double&            w,
                                double&            h ) override;
        };


        /**
         * EAN Barcode (Any)
         */
        class Ean : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * EAN-8 Barcode
         */
        class Ean8 : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * EAN-8+2 Barcode
         */
        class Ean8_2 : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * EAN-8+5 Barcode
         */
        class Ean8_5 : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * EAN-13 Barcode
         */
        class Ean13 : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * EAN-13+2 Barcode
         */
        class Ean13_2 : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * EAN-13+5 Barcode
         */
        class Ean13_5 : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * UPC Barcode (Any)
         */
        class Upc : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * UPC-A Barcode
         */
        class UpcA : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * UPC-A+2 Barcode
         */
        class UpcA_2 : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * UPC-A+5 Barcode
         */
        class UpcA_5 : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * UPC-E Barcode
         */
        class UpcE : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * UPC-E+2 Barcode
         */
        class UpcE_2 : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * UPC-E+5 Barcode
         */
        class UpcE_5 : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * ISBN Barcode
         */
        class Isbn : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * ISBN+5 Barcode
         */
        class Isbn_5 : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * Code39 Barcode
         */
        class Code39 : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * Code128 Barcode
         */
        class Code128 : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * Code128C Barcode
         */
        class Code128C : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * Code128B Barcode
         */
        class Code128B : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * I25 Barcode
         */
        class I25 : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * CBR Barcode
         */
        class Cbr : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * MSI Barcode
         */
        class Msi : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * PLS Barcode
         */
        class Pls : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };


        /**
         * Code93 Barcode
         */
        class Code93 : public Base
        {
        public:
                static Barcode* create();

        protected:
                bool validate( const std::string& rawData ) override;
                std::string encode( const std::string& cookedData ) override;
        };

}


#endif // HAVE_GNU_BARCODE

#endif // barcode_GnuBarcode_hpp
