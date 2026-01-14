//  Version.cpp
//
//  Copyright (C) 2025-2026  Jaye Evins <evins@snaught.com>
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


#include "Version.hpp"

#include <QGuiApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QSysInfo>
#include <QTextStream>


namespace glabels::model
{

        QString Version::details()
        {
                QString s;
                QTextStream ts( &s );

                ts << "> GLABELS" << "\n"
                   << ">     Version: " << STRING << "\n"
                   << "> " << "\n"

                   << "> SYSTEM INFO" << "\n"
                   << ">     OS: " << QSysInfo::prettyProductName() << "\n"
                   << ">     Kernel: " << QSysInfo::kernelType() << " " << QSysInfo::kernelVersion() << "\n"
                   << ">     Build CPU Architecture: " << QSysInfo::buildCpuArchitecture() << "\n"
                   << ">     Current CPU Architecture: " << QSysInfo::currentCpuArchitecture() << "\n"
                   << ">     Qt Version: " << QLibraryInfo::version().toString() << "\n"
                   << ">     QPA Platform: " << QGuiApplication::platformName() << "\n"
                   << "> " << "\n"

                   << "> LOCALE" << "\n"
                   << ">     Name: " << QLocale::system().name();

                return s;
        }

}
