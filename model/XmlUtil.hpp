//  XmlUtil.hpp
//
//  Copyright (C) 2013-2026  Jaye Evins <evins@snaught.com>
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

#ifndef model_XmlUtil_hpp
#define model_XmlUtil_hpp


#include "Distance.hpp"

#include <QDomElement>
#include <QFont>
#include <QPainterPath>
#include <QString>
#include <Qt>
#include <QTextOption>

#include <cstdint>


namespace glabels::model
{

        class XmlUtil
        {
        public:
                XmlUtil() = delete;

                static Units units();
                static void setUnits( Units units );

                static QString  getStringAttr( const QDomElement& node,
                                               const QString&     name,
                                               const QString&     default_value );

                static double   getDoubleAttr( const QDomElement& node,
                                               const QString&     name,
                                               double             default_value );

                static bool     getBoolAttr( const QDomElement& node,
                                             const QString&     name,
                                             bool               default_value );

                static int      getIntAttr( const QDomElement& node,
                                            const QString&     name,
                                            int                default_value );

                static uint32_t getUIntAttr( const QDomElement& node,
                                             const QString&     name,
                                             uint32_t           default_value );

                static QString  getI18nAttr( const QDomElement& node,
                                             const QString&     name,
                                             const QString&     default_value );

                static Distance getLengthAttr( const QDomElement& node,
                                               const QString&     name,
                                               Distance           default_value );

                static QFont::Weight getWeightAttr( const QDomElement& node,
                                                    const QString&     name,
                                                    QFont::Weight      default_value );

                static Qt::Alignment getAlignmentAttr( const QDomElement& node,
                                                       const QString&     name,
                                                       Qt::Alignment      default_value );

                static QTextOption::WrapMode getWrapModeAttr( const QDomElement&    node,
                                                              const QString&        name,
                                                              QTextOption::WrapMode default_value );

                static Units getUnitsAttr(  const QDomElement& node,
                                            const QString&     name,
                                            Units              default_value );

                static QPainterPath getPathDataAttr( const QDomElement& node,
                                                     const QString&     name,
                                                     Units              units );


                static void     setStringAttr( QDomElement&   node,
                                               const QString& name,
                                               const QString& value );

                static void     setDoubleAttr( QDomElement&   node,
                                               const QString& name,
                                               double         value );

                static void     setBoolAttr( QDomElement&   node,
                                             const QString& name,
                                             bool           value );

                static void     setIntAttr( QDomElement&   node,
                                            const QString& name,
                                            int            value );

                static void     setUIntAttr( QDomElement&   node,
                                             const QString& name,
                                             uint32_t       value );

                static void     setLengthAttr( QDomElement&    node,
                                               const QString&  name,
                                               Distance        value );

                static void     setWeightAttr( QDomElement&    node,
                                               const QString&  name,
                                               QFont::Weight   value );

                static void     setAlignmentAttr( QDomElement&    node,
                                                  const QString&  name,
                                                  Qt::Alignment   value );

                static void     setWrapModeAttr( QDomElement&          node,
                                                 const QString&        name,
                                                 QTextOption::WrapMode value );

                static void     setUnitsAttr(  QDomElement&   node,
                                               const QString& name,
                                               Units          value );

                static void     setPathDataAttr( QDomElement&        node,
                                                 const QString&      name,
                                                 const QPainterPath& value,
                                                 Units               units );

        private:
                static Units mUnits;

        };

}


#endif // model_XmlUtil_hpp
