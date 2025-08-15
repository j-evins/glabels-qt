/*  Zint.h
 *
 *  Copyright (C) 2017  Jaye Evins <evins@snaught.com>
 *
 *  This file is part of gLabels-qt.
 *
 *  gLabels-qt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  gLabels-qt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef barcode_Zint_h
#define barcode_Zint_h

#if HAVE_ZINT

#include "glbarcode/Barcode1dBase.h"


namespace glabels
{
	namespace barcode
	{
		namespace Zint
		{

			/**
			 * Zint::Base base class for all Zint barcodes
			 *
			 * Implements glbarcode::Barcode1dBase.
			 */
			class Base : public glbarcode::Barcode1dBase
			{
			protected:
				int symbology;
				int option_2;


				Base();

				bool validate( const std::string& rawData ) override;

				void vectorize( const std::string& encodedData,
				                const std::string& displayText,
				                const std::string& cookedData,
				                double&            w,
				                double&            h ) override;
			};


			/**
			 * AusP Barcode
			 */
			class AusP : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * AusRP Barcode
			 */
			class AusRP : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * AusRT Barcode
			 */
			class AusRT : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * AusRD Barcode
			 */
			class AusRD : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Aztec Barcode
			 */
			class Aztec : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Azrun Barcode
			 */
			class Azrun : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Channel Barcode
			 */
			class Channel : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Cbr Barcode
			 */
			class Cbr : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Codeblock-F Barcode
			 */
			class Cblockf : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Code1 Barcode
			 */
			class Code1 : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Code11 Barcode
			 */
			class Code11 : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * C16k Barcode
			 */
			class C16k : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * C25m Barcode
			 */
			class C25m : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * C25i Barcode
			 */
			class C25i : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Code 2 of 5 Industrial Barcode
			 */
			class C25ind : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * C25dl Barcode
			 */
			class C25dl : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Code32 Barcode
			 */
			class Code32 : public Base
			{
			public:
				static Barcode* create();
			
			protected:
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
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Code39e Barcode
			 */
			class Code39e : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Code49 Barcode
			 */
			class Code49 : public Base
			{
			public:
				static Barcode* create();
			
			protected:
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
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Code128b Barcode
			 */
			class Code128b : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Daft Barcode
			 */
			class Daft : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Dmtx Barcode
			 */
			class Dmtx : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Dpl Barcode
			 */
			class Dpl : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Dpi Barcode
			 */
			class Dpi : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * DotCode Barcode
			 */
			class Dotcode : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * DPD Code Barcode
			 */
			class Dpd : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Kix Barcode
			 */
			class Kix : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Ean Barcode
			 */
			class Ean : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * EAN-14 Barcode
			 */
			class Ean14 : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Facing Identification Mark Barcode
			 */
			class Fim : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Flattermarken Barcode
			 */
			class Flat : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Han Xin Barcode
			 */
			class Hanxin : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Hibc128 Barcode
			 */
			class Hibc128 : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Hibc39 Barcode
			 */
			class Hibc39 : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Hibcdm Barcode
			 */
			class Hibcdm : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Hibcqr Barcode
			 */
			class Hibcqr : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Hibcpdf Barcode
			 */
			class Hibcpdf : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Hibcmpdf Barcode
			 */
			class Hibcmpdf : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * HIBC Codablock-F Barcode
			 */
			class Hibcblkf : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Hibcaz Barcode
			 */
			class Hibcaz : public Base
			{
			public:
				static Barcode* create();
			
			protected:
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
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Isbn Barcode
			 */
			class Isbn : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Itf14 Barcode
			 */
			class Itf14 : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Gmtx Barcode
			 */
			class Gmtx : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Gs1128 Barcode
			 */
			class Gs1128 : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Logm Barcode
			 */
			class Logm : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Rss14 Barcode
			 */
			class Rss14 : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Rssltd Barcode
			 */
			class Rssltd : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Rssexp Barcode
			 */
			class Rssexp : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Rsss Barcode
			 */
			class Rsss : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Rssso Barcode
			 */
			class Rssso : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Rssse Barcode
			 */
			class Rssse : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Pharma Barcode
			 */
			class Pharma : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Pharma2 Barcode
			 */
			class Pharma2 : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Pzn Barcode
			 */
			class Pzn : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Tele Barcode
			 */
			class Tele : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Telex Barcode
			 */
			class Telex : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Japan Barcode
			 */
			class Japan : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Korea Barcode
			 */
			class Korea : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Maxi Barcode
			 */
			class Maxi : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Mpdf Barcode
			 */
			class Mpdf : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Msi Barcode
			 */
			class Msi : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Mqr Barcode
			 */
			class Mqr : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Nve Barcode
			 */
			class Nve : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Plan Barcode
			 */
			class Plan : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Postnet Barcode
			 */
			class Postnet : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * BC412 (SEMI T1-95) Barcode
			 */
			class Bc412 : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * CEPNet Barcode
			 */
			class Cepnet : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Pdf Barcode
			 */
			class Pdf : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Pdft Barcode
			 */
			class Pdft : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Qr Barcode
			 */
			class Qr : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * rMQR Barcode
			 */
			class Rmqr : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Rm4 Barcode
			 */
			class Rm4 : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Royal Mail 4-State Mailmark Barcode
			 */
			class Rm4sm : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Royal Mail 2-D Mailmark Barcode
			 */
			class Rm2dm : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * UpcA Barcode
			 */
			class UpcA : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * UpcE Barcode
			 */
			class UpcE : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * UPU S10 Barcode
			 */
			class UpuS10 : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Usps Barcode
			 */
			class Usps : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Pls Barcode
			 */
			class Pls : public Base
			{
			public:
				static Barcode* create();
			
			protected:
				std::string encode( const std::string& cookedData ) override;
			};


			/**
			 * Vehicle Identification Number Barcode
			 */
			class Vin : public Base
			{
			public:
				static Barcode* create();

			protected:
				std::string encode( const std::string& cookedData ) override;
			};


		}
	}
}


#endif // HAVE_ZINT

#endif // barcode_Zint_h
