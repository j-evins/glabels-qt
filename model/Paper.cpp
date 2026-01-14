//  Paper.cpp
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


#include "Paper.hpp"


namespace glabels::model
{

        Paper::Paper( const QString& id,
                      const QString& name,
                      Distance       width,
                      Distance       height,
                      const QString& pwgSize )
                : mId(id),
                  mName(name),
                  mWidth(width),
                  mHeight(height),
                  mPwgSize(pwgSize)
        {
                // empty
        }


        QString Paper::id() const
        {
                return mId;
        }


        QString Paper::name() const
        {
                return mName;
        }


        Distance Paper::width() const
        {
                return mWidth;
        }


        Distance Paper::height() const
        {
                return mHeight;
        }


        QString Paper::pwgSize() const
        {
                return mPwgSize;
        }


        bool Paper::isSizeIso() const
        {
                return mPwgSize.startsWith( "iso_" );
        }


        bool Paper::isSizeUs() const
        {
                return mPwgSize.startsWith( "na_" );
        }

}
