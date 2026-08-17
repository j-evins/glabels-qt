//  AppearanceModel.hpp
//
//  Copyright (C) 2026  Jaye Evins <evins@snaught.com>
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

#ifndef AppearanceModel_hpp
#define AppearanceModel_hpp


#include "model/Settings.hpp"

#include <QObject>

#include <memory>


namespace glabels
{

        ///
        /// Settings Singleton Class
        ///
        class AppearanceModel : public QObject
        {
                Q_OBJECT

        public:
                enum Style { STYLE_LIGHT, STYLE_DARK };


                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        private:
                AppearanceModel();

        public:
                static void init();
                static AppearanceModel* instance();


                /////////////////////////////////
                // Signals
                /////////////////////////////////
        signals:
                void changed();


                /////////////////////////////////
                // Accessors
                /////////////////////////////////
        public:
                static Style style();


                /////////////////////////////////
                // Private methods
                /////////////////////////////////
        private:
                static void setMode( model::Settings::AppearanceMode mode );

                static void setLightMode();
                static void setDarkMode();


                /////////////////////////////////
                // Slots
                /////////////////////////////////
        private slots:
                void onSettingsChanged();


                /////////////////////////////////
                // pseudo-Slot
                /////////////////////////////////
#if QT_VERSION >= QT_VERSION_CHECK(6,8,0)
        private:
	        void onSystemColorSchemeChanged( Qt::ColorScheme scheme );
#endif


        private:
                static std::unique_ptr<AppearanceModel> mInstance;
                static model::Settings::AppearanceMode mMode;

        };

}


#endif // AppearanceModel_hpp
