/*  PrinterMonitor.h
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

#ifndef PrinterMonitor_h
#define PrinterMonitor_h


#include <QObject>
#include <QStringList>
#include <QTimer>

#include <memory>


namespace glabels
{

	///
	/// Printer Monitor
	///
	class PrinterMonitor : public QObject
	{
		Q_OBJECT

		/////////////////////////////////
		// Life Cycle
		/////////////////////////////////
	private:
		PrinterMonitor();

	public:
		static PrinterMonitor* instance();


		/////////////////////////////////
		// Public methods
		/////////////////////////////////
	public:
		const QStringList& availablePrinters() const;


		/////////////////////////////////
		// Slots
		/////////////////////////////////
	private slots:
		void onTimerTimeout();
			

		/////////////////////////////////
		// Signals
		/////////////////////////////////
	signals:
		void availablePrintersChanged( const QStringList& availablePrinters );


		/////////////////////////////////
		// Private Members
		/////////////////////////////////
	private:
		static std::unique_ptr<PrinterMonitor> mInstance;

		std::unique_ptr<QTimer> mTimer;
		QStringList mCurrentAvailablePrinters;

	};

}


#endif // PrinterMonitor_h
