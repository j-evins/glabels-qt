/*  Backends.cpp
 *
 *  Copyright (C) 2014  Jaye Evins <evins@snaught.com>
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

#include "Backends.h"

#include "GnuBarcode.h"
#include "QrEncode.h"
#include "Zint.h"

#include "glbarcode/Factory.h"


namespace glabels
{
	namespace barcode
	{
	
		//
		// Static data
		//
		QStringList Backends::mBackendIdList;
		QMap<QString,QString> Backends::mBackendNameMap;

		QList<Style> Backends::mStyleList;
	

		Backends::Backends()
		{
			registerStyle( "code39", "", tr("Code 39"),
			               true, true, true, true, "1234567890", true, 10 );

			registerStyle( "code39ext", "", tr("Code 39 Extended"),
			               true, true, true, true, "1234567890", true, 10 );

			registerStyle( "upc-a", "", tr("UPC-A"),
			               true, true, true, false, "12345678901", false, 11 );

			registerStyle( "ean-13", "", tr("EAN-13"),
			               true, true, true, false, "123456789012", false, 12 );

			registerStyle( "postnet", "", tr("POSTNET (any)"),
			               false, false, true, false, "12345-6789-12", false, 11 );

			registerStyle( "postnet-5", "", tr("POSTNET-5 (ZIP only)"),
			               false, false, true, false, "12345", false, 5 );

			registerStyle( "postnet-9", "", tr("POSTNET-9 (ZIP+4)"),
			               false, false, true, false, "12345-6789", false, 9 );

			registerStyle( "postnet-11", "", tr("POSTNET-11 (DPBC)"),
			               false, false, true, false, "12345-6789-12", false, 11 );

			registerStyle( "cepnet", "", tr("CEPNET"),
			               false, false, true, false, "12345-678", false, 8 );

			registerStyle( "onecode", "", tr("USPS Intelligent Mail"),
			               false, false, true, false, "12345678901234567890", false, 20 );

			registerStyle( "datamatrix", "", tr("IEC16022 (DataMatrix)"),
			               false, false, true, false, "1234567890AB", false, 12 );

#if HAVE_GNU_BARCODE
			//
			// GNU Barcode backend
			//
			registerBackend( "gnu-barcode", "GNU Barcode" );
		
			glbarcode::Factory::registerType( "gnu-barcode::ean",      GnuBarcode::Ean::create );
			glbarcode::Factory::registerType( "gnu-barcode::ean-8",    GnuBarcode::Ean8::create );
			glbarcode::Factory::registerType( "gnu-barcode::ean-8+2",  GnuBarcode::Ean8_2::create );
			glbarcode::Factory::registerType( "gnu-barcode::ean-8+5",  GnuBarcode::Ean8_5::create );
			glbarcode::Factory::registerType( "gnu-barcode::ean-13",   GnuBarcode::Ean13::create );
			glbarcode::Factory::registerType( "gnu-barcode::ean-13+2", GnuBarcode::Ean13_2::create );
			glbarcode::Factory::registerType( "gnu-barcode::ean-13+5", GnuBarcode::Ean13_5::create );
			glbarcode::Factory::registerType( "gnu-barcode::upc",      GnuBarcode::Upc::create );
			glbarcode::Factory::registerType( "gnu-barcode::upc-a",    GnuBarcode::UpcA::create );
			glbarcode::Factory::registerType( "gnu-barcode::upc-a+2",  GnuBarcode::UpcA_2::create );
			glbarcode::Factory::registerType( "gnu-barcode::upc-a+5",  GnuBarcode::UpcA_5::create );
			glbarcode::Factory::registerType( "gnu-barcode::upc-e",    GnuBarcode::UpcE::create );
			glbarcode::Factory::registerType( "gnu-barcode::upc-e+2",  GnuBarcode::UpcE_2::create );
			glbarcode::Factory::registerType( "gnu-barcode::upc-e+5",  GnuBarcode::UpcE_5::create );
			glbarcode::Factory::registerType( "gnu-barcode::isbn",     GnuBarcode::Isbn::create );
			glbarcode::Factory::registerType( "gnu-barcode::isbn+5",   GnuBarcode::Isbn_5::create );
			glbarcode::Factory::registerType( "gnu-barcode::code39",   GnuBarcode::Code39::create );
			glbarcode::Factory::registerType( "gnu-barcode::code128",  GnuBarcode::Code128::create );
			glbarcode::Factory::registerType( "gnu-barcode::code128c", GnuBarcode::Code128C::create );
			glbarcode::Factory::registerType( "gnu-barcode::code128b", GnuBarcode::Code128B::create );
			glbarcode::Factory::registerType( "gnu-barcode::i25",      GnuBarcode::I25::create );
			glbarcode::Factory::registerType( "gnu-barcode::cbr",      GnuBarcode::Cbr::create );
			glbarcode::Factory::registerType( "gnu-barcode::msi",      GnuBarcode::Msi::create );
			glbarcode::Factory::registerType( "gnu-barcode::pls",      GnuBarcode::Pls::create );
			glbarcode::Factory::registerType( "gnu-barcode::code93",   GnuBarcode::Code93::create );

			registerStyle( "ean", "gnu-barcode", tr("EAN (any)"),
			               true, true, true, false, "000000000000 00000", false, 17 );
			registerStyle( "ean-8", "gnu-barcode", tr("EAN-8"),
			               true, true, true, false, "0000000", false, 7 );
			registerStyle( "ean-8+2", "gnu-barcode", tr("EAN-8+2"),
			               true, true, true, false, "0000000 00", false, 9 );
			registerStyle( "ean-8+5", "gnu-barcode", tr("EAN-8+5"),
			               true, true, true, false, "0000000 00000", false, 12 );
			registerStyle( "ean-13", "gnu-barcode", tr("EAN-13"),
			               true, true, true, false, "000000000000", false, 12 );
			registerStyle( "ean-13+2", "gnu-barcode", tr("EAN-13+2"),
			               true, true, true, false, "000000000000 00", false, 14 );
			registerStyle( "ean-13+5", "gnu-barcode", tr("EAN-13+5"),
			               true, true, true, false, "000000000000 00000", false, 17 );
			registerStyle( "upc", "gnu-barcode", tr("UPC (UPC-A or UPC-E)"),
			               true, true, true, false, "00000000000 00000", false, 16 );
			registerStyle( "upc-a", "gnu-barcode", tr("UPC-A"),
			               true, true, true, false, "00000000000", false, 11 );
			registerStyle( "upc-a+2", "gnu-barcode", tr("UPC-A +2"),
			               true, true, true, false, "00000000000 00", false, 13 );
			registerStyle( "upc-a+5", "gnu-barcode", tr("UPC-A +5"),
			               true, true, true, false, "00000000000 00000", false, 16 );
			registerStyle( "upc-e", "gnu-barcode", tr("UPC-E"),
			               true, true, true, false, "000000", false, 6 );
			registerStyle( "upc-e+2", "gnu-barcode", tr("UPC-E +2"),
			               true, true, true, false, "000000 00", false, 8 );
			registerStyle( "upc-e+5", "gnu-barcode", tr("UPC-E +5"),
			               true, true, true, false, "000000 00000", false, 11 );
			registerStyle( "isbn", "gnu-barcode", tr("ISBN"),
			               true, true, true, true, "0-00000-000-0", false, 10 );
			registerStyle( "isbn+5", "gnu-barcode", tr("ISBN +5"),
			               true, true, true, true, "0-00000-000-0 00000", false, 15 );
			registerStyle( "code39", "gnu-barcode", tr("Code 39"),
			               true, true, true, true, "0000000000", true, 10 );
			registerStyle( "code128", "gnu-barcode", tr("Code 128"),
			               true, true, true, true, "0000000000", true, 10 );
			registerStyle( "code128c", "gnu-barcode", tr("Code 128C"),
			               true, true, true, false, "0000000000", true, 10 );
			registerStyle( "code128b", "gnu-barcode", tr("Code 128B"),
			               true, true, true, true, "0000000000", true, 10 );
			registerStyle( "i25", "gnu-barcode", tr("Interleaved 2 of 5"),
			               true, true, true, true, "0000000000", true, 10 );
			registerStyle( "cbr", "gnu-barcode", tr("Codabar"),
			               true, true, true, true, "0000000000", true, 10 );
			registerStyle( "msi", "gnu-barcode", tr("MSI"),
			               true, true, true, true, "0000000000", true, 10 );
			registerStyle( "pls", "gnu-barcode", tr("Plessey"),
			               true, true, true, true, "0000000000", true, 10 );
			registerStyle( "code93", "gnu-barcode", tr("Code 93"),
			               true, true, true, false, "0000000000", true, 10 );
#endif // HAVE_GNU_BARCODE

#if HAVE_QRENCODE
			//
			// Libqrencode backend
			//
			registerBackend( "qrencode", "QREncode" );
		
			glbarcode::Factory::registerType( "qrencode::qrcode", QrEncode::QrCode::create );

			registerStyle( "qrcode", "qrencode", tr("IEC18004 (QRCode)"),
			               false, false, true, false, "1234567890AB", false, 12 );
#endif // HAVE_QRENCODE

#if HAVE_ZINT
			//
			// Zint barcode backend
			//
			registerBackend( "zint", "Zint" );
		
			glbarcode::Factory::registerType( "zint::ausp",      Zint::AusP::create );
			glbarcode::Factory::registerType( "zint::ausrp",     Zint::AusRP::create );
			glbarcode::Factory::registerType( "zint::ausrt",     Zint::AusRT::create );
			glbarcode::Factory::registerType( "zint::ausrd",     Zint::AusRD::create );
			glbarcode::Factory::registerType( "zint::aztec",     Zint::Aztec::create );
			glbarcode::Factory::registerType( "zint::azrun",     Zint::Azrun::create );
			glbarcode::Factory::registerType( "zint::bc412",     Zint::Bc412::create );
			glbarcode::Factory::registerType( "zint::cepnet",    Zint::Cepnet::create );
			glbarcode::Factory::registerType( "zint::cbr",       Zint::Cbr::create );
			glbarcode::Factory::registerType( "zint::cblockf",   Zint::Cblockf::create );
			glbarcode::Factory::registerType( "zint::channel",   Zint::Channel::create );
			glbarcode::Factory::registerType( "zint::code1",     Zint::Code1::create );
			glbarcode::Factory::registerType( "zint::code11",    Zint::Code11::create );
			glbarcode::Factory::registerType( "zint::c16k",      Zint::C16k::create );
			glbarcode::Factory::registerType( "zint::c25m",      Zint::C25m::create );
			glbarcode::Factory::registerType( "zint::c25i",      Zint::C25i::create );
			glbarcode::Factory::registerType( "zint::c25ind",    Zint::C25ind::create );
			glbarcode::Factory::registerType( "zint::c25dl",     Zint::C25dl::create );
			glbarcode::Factory::registerType( "zint::code32",    Zint::Code32::create );
			glbarcode::Factory::registerType( "zint::code39",    Zint::Code39::create );
			glbarcode::Factory::registerType( "zint::code39e",   Zint::Code39e::create );
			glbarcode::Factory::registerType( "zint::code49",    Zint::Code49::create );
			glbarcode::Factory::registerType( "zint::code93",    Zint::Code93::create );
			glbarcode::Factory::registerType( "zint::code128",   Zint::Code128::create );
			glbarcode::Factory::registerType( "zint::code128b",  Zint::Code128b::create );
			glbarcode::Factory::registerType( "zint::daft",      Zint::Daft::create );
			glbarcode::Factory::registerType( "zint::dmtx",      Zint::Dmtx::create );
			glbarcode::Factory::registerType( "zint::dpl",       Zint::Dpl::create );
			glbarcode::Factory::registerType( "zint::dpi",       Zint::Dpi::create );
			glbarcode::Factory::registerType( "zint::dotcode",   Zint::Dotcode::create );
			glbarcode::Factory::registerType( "zint::dpd",       Zint::Dpd::create );
			glbarcode::Factory::registerType( "zint::kix",       Zint::Kix::create );
			glbarcode::Factory::registerType( "zint::ean",       Zint::Ean::create );
			glbarcode::Factory::registerType( "zint::ean14",     Zint::Ean14::create );
			glbarcode::Factory::registerType( "zint::fim",       Zint::Fim::create );
			glbarcode::Factory::registerType( "zint::flat",      Zint::Flat::create );
			glbarcode::Factory::registerType( "zint::gmtx",      Zint::Gmtx::create );
			glbarcode::Factory::registerType( "zint::gs1-128",   Zint::Gs1128::create );
			glbarcode::Factory::registerType( "zint::hanxin",    Zint::Hanxin::create );
			glbarcode::Factory::registerType( "zint::rss14",     Zint::Rss14::create );
			glbarcode::Factory::registerType( "zint::rssltd",    Zint::Rssltd::create );
			glbarcode::Factory::registerType( "zint::rssexp",    Zint::Rssexp::create );
			glbarcode::Factory::registerType( "zint::rsss",      Zint::Rsss::create );
			glbarcode::Factory::registerType( "zint::rssso",     Zint::Rssso::create );
			glbarcode::Factory::registerType( "zint::rssse",     Zint::Rssse::create );
			glbarcode::Factory::registerType( "zint::hibc128",   Zint::Hibc128::create );
			glbarcode::Factory::registerType( "zint::hibc39",    Zint::Hibc39::create );
			glbarcode::Factory::registerType( "zint::hibcdm",    Zint::Hibcdm::create );
			glbarcode::Factory::registerType( "zint::hibcqr",    Zint::Hibcqr::create );
			glbarcode::Factory::registerType( "zint::hibcpdf",   Zint::Hibcpdf::create );
			glbarcode::Factory::registerType( "zint::hibcmpdf",  Zint::Hibcmpdf::create );
			glbarcode::Factory::registerType( "zint::hibcblkf",  Zint::Hibcblkf::create );
			glbarcode::Factory::registerType( "zint::hibcaz",    Zint::Hibcaz::create );
			glbarcode::Factory::registerType( "zint::i25",       Zint::I25::create );
			glbarcode::Factory::registerType( "zint::isbn",      Zint::Isbn::create );
			glbarcode::Factory::registerType( "zint::itf14",     Zint::Itf14::create );
			glbarcode::Factory::registerType( "zint::japan",     Zint::Japan::create );
			glbarcode::Factory::registerType( "zint::korea",     Zint::Korea::create );
			glbarcode::Factory::registerType( "zint::logm",      Zint::Logm::create );
			glbarcode::Factory::registerType( "zint::maxi",      Zint::Maxi::create );
			glbarcode::Factory::registerType( "zint::mpdf",      Zint::Mpdf::create );
			glbarcode::Factory::registerType( "zint::mqr",       Zint::Mqr::create );
			glbarcode::Factory::registerType( "zint::msi",       Zint::Msi::create );
			glbarcode::Factory::registerType( "zint::nve",       Zint::Nve::create );
			glbarcode::Factory::registerType( "zint::pdf",       Zint::Pdf::create );
			glbarcode::Factory::registerType( "zint::pdft",      Zint::Pdft::create );
			glbarcode::Factory::registerType( "zint::plan",      Zint::Plan::create );
			glbarcode::Factory::registerType( "zint::postnet",   Zint::Postnet::create );
			glbarcode::Factory::registerType( "zint::pharma",    Zint::Pharma::create );
			glbarcode::Factory::registerType( "zint::pharma2",   Zint::Pharma2::create );
			glbarcode::Factory::registerType( "zint::pzn",       Zint::Pzn::create );
			glbarcode::Factory::registerType( "zint::qr",        Zint::Qr::create );
			glbarcode::Factory::registerType( "zint::rmqr",      Zint::Rmqr::create );
			glbarcode::Factory::registerType( "zint::rm4",       Zint::Rm4::create );
			glbarcode::Factory::registerType( "zint::rm4sm",     Zint::Rm4sm::create );
			glbarcode::Factory::registerType( "zint::rm2dm",     Zint::Rm2dm::create );
			glbarcode::Factory::registerType( "zint::tele",      Zint::Tele::create );
			glbarcode::Factory::registerType( "zint::telex",     Zint::Telex::create );
			glbarcode::Factory::registerType( "zint::upc-a",     Zint::UpcA::create );
			glbarcode::Factory::registerType( "zint::upc-e",     Zint::UpcE::create );
			glbarcode::Factory::registerType( "zint::upus10",    Zint::UpuS10::create );
			glbarcode::Factory::registerType( "zint::usps",      Zint::Usps::create );
			glbarcode::Factory::registerType( "zint::pls",       Zint::Pls::create );
			glbarcode::Factory::registerType( "zint::vin",       Zint::Vin::create );

			registerStyle( "ausp", "zint", tr("Australia Post Standard"),
			               false, false, true, false, "12345678901234567890123", true, 23 );

			registerStyle( "ausrp", "zint", tr("Australia Post Reply Paid"),
			               false, false, true, false, "12345678", true, 8 );

			registerStyle( "ausrt", "zint", tr("Australia Post Route Code"),
			               false, false, true, false, "12345678", true, 8 );

			registerStyle( "ausrd", "zint", tr("Australia Post Redirect"),
			               false, false, true, false, "12345678", true, 8 );

			registerStyle( "aztec", "zint", tr("Aztec Code"),
			               false, false, true, false, "1234567890", true, 10 );
          
			registerStyle( "azrun", "zint", tr("Aztec Runes"),
			               false, false, true, false, "255", true, 3 );

			registerStyle( "bc412", "zint", tr("BC412 (SEMI TI-95)"),
			               true, true, true, false, "12345678", true, 8 );

			registerStyle( "cepnet", "zint", tr("CEPNet (Brazilian Post)"),
			               false, false, true, false, "12345678", true, 8 );

			registerStyle( "channel", "zint", tr("Channel Code"),
			               true, true, false, false, "00", true, 2 );

			registerStyle( "cbr", "zint", tr("Codabar"),
			               true, true, true, true, "A00000000B", true, 10 );

			registerStyle( "cblockf", "zint", tr("Codablock-F"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "code1", "zint", tr("Code One"), 
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "code11", "zint", tr("Code 11"),
			               true, true, true, true, "0000000000", true, 10 );
          
			registerStyle( "c16k", "zint", tr("Code 16K"),
			               false, false, true, false, "0000000000", true, 10 );
          
			registerStyle( "c25m", "zint", tr("Code 2 of 5 Standard"),
			               true, true, true, true, "0000000000", true, 10 );
          
			registerStyle( "c25i", "zint", tr("Code 2 of 5 IATA"), 
			               true, true, true, true, "0000000000", true, 10 );

			registerStyle( "c25ind", "zint", tr("Code 2 of 5 Industrial"),
			               true, true, true, true, "0000000000", true, 10 );
          
			registerStyle( "c25dl", "zint", tr("Code 2 of 5 Data Logic"), 
			               true, true, true, true, "0000000000", true, 10 );

			registerStyle( "code32", "zint", tr("Code 32 (Italian Pharmacode)"), 
			               true, true, true, false, "12345678", true, 8 );

			registerStyle( "code39", "zint", tr("Code 39"),
			               true, true, true, true, "0000000000", true, 10 );
          
			registerStyle( "code39e", "zint", tr("Code 39 Extended"), 
			               true, true, true, true, "0000000000", true, 10 );

			registerStyle( "code49", "zint", tr("Code 49"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "code93", "zint", tr("Code 93"),
			               true, true, true, false, "0000000000", true, 10 );

			registerStyle( "code128", "zint", tr("Code 128"),
			               true, true, true, false, "0000000000", true, 10 );
          
			registerStyle( "code128b", "zint", tr("Code 128 (Mode C suppression)"),
			               true, true, true, false, "0000000000", true, 10 );
          
			registerStyle( "daft", "zint", tr("DAFT Code"),
			               false, false, false, false, "DAFTDAFTDAFTDAFT", true, 16 );

			registerStyle( "dmtx", "zint", tr("Data Matrix"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "dpl", "zint", tr("Deutsche Post Leitcode"),
			               true, true, true, false, "1234567890123", true, 13 );
          
			registerStyle( "dpi", "zint", tr("Deutsche Post Identcode"),
			               true, true, true, false, "12345678901", true, 11 );

			registerStyle( "dotcode", "zint", tr("DotCode"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "dpd", "zint", tr("DPD Code"),
			               true, true, true, false, "000000000000000000000000000", true, 27 );

			registerStyle( "kix", "zint", tr("Dutch Post KIX Code"),
			               false, false, false, false, "0000000000", true, 10 );

			registerStyle( "ean", "zint", tr("EAN"),
			               true, true, true, false, "123456789012", false, 12 );

			registerStyle( "ean14", "zint", tr("EAN-14"),
			               true, true, true, false, "1234567890123", false, 13 );

			registerStyle( "fim", "zint", tr("FIM (Facing ID Mark)"),
			               false, false, false, false, "A", false, 1 );

			registerStyle( "flat", "zint", tr("Flattermarken"),
			               false, false, false, false, "11111111", false, 8 );

			registerStyle( "gmtx", "zint", tr("Grid Matrix"), 
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "gs1-128", "zint", tr("GS1-128"),
			               true, true, true, false, "[01]12345678901231", false, 18 );

			registerStyle( "rss14", "zint", tr("GS1 DataBar Omnidirectional"),
			               true, true, true, false, "1234567890123", true, 13 );
          
			registerStyle( "rssltd", "zint", "GS1 DataBar Limited",
			               true, true, true, false, "1234567890123", true, 13 );
          
			registerStyle( "rssexp", "zint", "GS1 DataBar Expanded",
			               true, true, true, false, "[01]12345678901231", false, 18 );
          
			registerStyle( "rsss", "zint", tr("GS1 DataBar Stacked"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "rssso", "zint", tr("GS1 DataBar Stacked Omni."),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "rssse", "zint", tr("GS1 DataBar Expanded Stacked"),
			               false, false, true, false, "[01]12345678901231", false, 18 );

			registerStyle( "hanxin", "zint", tr("Han Xin"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "hibc128", "zint", tr("HIBC Code 128"),
			               true, true, true, false, "0000000000", true, 10 );

			registerStyle( "hibc39", "zint", tr("HIBC Code 39"),
			               true, true, true, false, "0000000000", true, 10 );

			registerStyle( "hibcdm", "zint", tr("HIBC Data Matrix"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "hibcqr", "zint", tr("HIBC QR Code"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "hibcpdf", "zint", tr("HIBC PDF417"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "hibcmpdf", "zint", tr("HIBC MicroPDF417"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "hibcblkf", "zint", tr("HIBC Codablock-F"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "hibcaz", "zint", tr("HIBC Aztec Code"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "i25", "zint", tr("Interleaved 2 of 5"),
			               true, true, true, true, "0000000000", true, 10 );

			registerStyle( "isbn", "zint", tr("ISBN"),
			               true, true, true, false, "123456789", false, 9 );

			registerStyle( "itf14", "zint", tr("ITF-14"),
			               true, true, true, false, "0000000000", true, 10 );

			registerStyle( "japan", "zint", tr("Japanese Postal"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "korea", "zint", tr("Korean Postal"),
			               true, true, true, false, "123456", false, 6 );

			registerStyle( "logm", "zint", tr("LOGMARS"),
			               true, true, true, true, "0000000000", true, 10 );

			registerStyle( "maxi", "zint", tr("Maxicode"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "mpdf", "zint", tr("MicroPDF417"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "mqr", "zint", tr("Micro QR Code"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "msi", "zint", tr("MSI Plessey"),
			               true, true, true, true, "0000000000", true, 10 );

			registerStyle( "nve", "zint", tr("NVE-18 (SSCC-18)"),
			               true, true, true, false, "12345678901234567", false, 17 );

			registerStyle( "pdf", "zint", tr("PDF417"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "pdft", "zint", tr("PDF417 Compact"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "plan", "zint", tr("PLANET"),
			               false, false, true, false, "00000000000", true, 11 );

			registerStyle( "postnet", "zint", tr("POSTNET"),
			               false, false, true, false, "00000000000", true, 11 );

			registerStyle( "pharma", "zint", tr("Pharmacode"),
			               false, false, false, false, "123456", false, 6 );

			registerStyle( "pharma2", "zint", tr("Pharmacode 2-track"),
			               false, false, false, false, "12345678", false, 8 );

			registerStyle( "pzn", "zint", tr("Pharmazentralnummer (PZN)"),
			               true, true, true, false, "1234567", false, 7 );

			registerStyle( "qr", "zint", tr("QR Code"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "rmqr", "zint", tr("rMQR (Rectangular Micro QR)"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "rm4", "zint", tr("Royal Mail 4-State Customer"),
			               false, false, true, false, "0000000000", true, 10 );

			registerStyle( "rm4sm", "zint", tr("Royal Mail 4-State Mailmark"),
			               false, false, true, false, "01000000000000000AA00AA0A", true, 25 );

			registerStyle( "rm2dm", "zint", tr("Royal Mail 2-D Mailmark"),
			               false, false, true, false, "012100123412345678AB19XY1A 0", true, 28 );

			registerStyle( "tele", "zint", tr("Telepen"),
			               true, true, true, false, "0000000000", true, 10 );

			registerStyle( "telex", "zint", tr("Telepen Numeric"),
			               true, true, true, false, "0000000000", true, 10 );

			registerStyle( "pls", "zint", tr("UK Plessey"),
			               true, true, true, false, "0000000000", true, 10 );

			registerStyle( "upc-a", "zint", tr("UPC-A"), 
			               true, true, true, false, "12345678901", false, 11 );
          
			registerStyle( "upc-e", "zint", tr("UPC-E"), 
			               true, true, true, false, "1234567", false, 7 );

			registerStyle( "upus10", "zint", tr("UPU S10"),
			               true, true, true, false, "EE876543216CA", false, 13 );

			registerStyle( "usps", "zint", tr("USPS Intelligent Mail"),
			               false, false, true, false, "12345678901234567890", true, 20 );

			registerStyle( "vin", "zint", tr("VIN (Vehicle ID Number)"),
			               true, true, true, false, "12345678701234567", false, 27 );
#endif // HAVE_ZINT

		}


		void Backends::init()
		{
			static Backends* singletonInstance = nullptr;
                
			if ( singletonInstance == nullptr )
			{
				singletonInstance = new Backends();
			}
		}


		const QStringList& Backends::backendList()
		{
			return mBackendIdList;
		}

	
		QString Backends::backendName( const QString& backendId )
		{
			return mBackendNameMap[ backendId ];
		}

	
		const QList<Style>& Backends::styleList()
		{
			return mStyleList;
		}


		const Style& Backends::defaultStyle()
		{
			return mStyleList[0];
		}


		const Style& Backends::style( const QString& backendId, const QString& StyleId )
		{
			foreach ( const Style& bcStyle, mStyleList )
			{
				if ( (bcStyle.backendId() == backendId) && (bcStyle.id() == StyleId) )
				{
					return bcStyle;
				}
			}

			return defaultStyle();
		}


		void Backends::registerBackend( const QString& backendId, const QString& backendName )
		{
			mBackendIdList.append( backendId );
			mBackendNameMap[ backendId ] = backendName;
		}


		void Backends::registerStyle( const QString& id,
		                              const QString& backendId,
		                              const QString& name,
		                              bool           canText,
		                              bool           textOptional,
		                              bool           canChecksum,
		                              bool           checksumOptional,
		                              const QString& defaultDigits,
		                              bool           canFreeForm,
		                              int            preferedN )
		{
			Style style( id, backendId, name,
			             canText, textOptional,
			             canChecksum, checksumOptional,
			             defaultDigits,
			             canFreeForm, preferedN );

			mStyleList.append( style );
		}

	} // namespace barcode
} //namespace glabels
