//  PrintView.hpp
//
//  Copyright (C) 2013-2016  Jaye Evins <evins@snaught.com>
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

#ifndef PrintView_hpp
#define PrintView_hpp


#include "ui_PrintView.h"

#include "model/Model.hpp"
#include "model/PageRenderer.hpp"


namespace glabels
{

        ///
        /// Print View Widget
        ///
        class PrintView : public QWidget, public Ui_PrintView
        {
                Q_OBJECT


                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        public:
                PrintView( QWidget *parent = nullptr );
                virtual ~PrintView() = default;


                /////////////////////////////////
                // Public methods
                /////////////////////////////////
                void setModel( model::Model* model );


                /////////////////////////////////
                // Slots
                /////////////////////////////////
        private slots:
                void onAvailablePrintersChanged( QStringList printers );
                void onModelChanged();
                void updateView();
                void onFormChanged();
                void onPrintButtonClicked();
                void onSystemDialogButtonClicked();


                /////////////////////////////////
                // Private methods
                /////////////////////////////////
        private:
                void loadDestinations( const QStringList& printers );
                QString defaultPdf();
                void setDestination( const QString& printerName );


                /////////////////////////////////
                // Private Data
                /////////////////////////////////
        private:
                model::Model*       mModel;
                model::PageRenderer mRenderer;

                bool                mBlocked;

        };

}


#endif // PrintView_hpp
