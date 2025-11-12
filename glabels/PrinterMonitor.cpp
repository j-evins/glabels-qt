/*  PrinterMonitor.cpp
 *
 *  Copyright (C) 2025  Jaye Evins <evins@snaught.com>
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


#include "PrinterMonitor.h"

#include <QPrinterInfo>
#include <QDebug>


namespace glabels
{

	///
	/// Static data
	///
	std::unique_ptr<PrinterMonitor> PrinterMonitor::mInstance;


	///
	/// Constructor
	///
	PrinterMonitor::PrinterMonitor()
	{
		mCurrentAvailablePrinters = QPrinterInfo::availablePrinterNames();

		mTimer.reset( new QTimer( this ) );
		connect( mTimer.get(), SIGNAL(timeout()), this, SLOT(onTimerTimeout()) );
		mTimer->start( 1000 );
	}


	///
	/// Get singleton instance
	///
	PrinterMonitor* PrinterMonitor::instance()
	{
		if ( !mInstance )
		{
			mInstance.reset( new PrinterMonitor() );
		}

		return mInstance.get();
	}


	///
	/// Get available printers
	///
	const QStringList& PrinterMonitor::availablePrinters() const
	{
		return mCurrentAvailablePrinters;
	}


	///
	/// On timer timeout
	///
	void PrinterMonitor::onTimerTimeout()
	{
		auto newAvailablePrinters = QPrinterInfo::availablePrinterNames();
		if ( newAvailablePrinters != mCurrentAvailablePrinters )
		{
			mCurrentAvailablePrinters = newAvailablePrinters;

			emit availablePrintersChanged( mCurrentAvailablePrinters );
		}
	}

	
} // namespace glabels
