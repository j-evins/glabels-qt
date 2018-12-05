/*  FramePath.h
 *
 *  Copyright (C) 2018  Jim Evins <evins@snaught.com>
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

#ifndef model_FramePath_h
#define model_FramePath_h


#include "Frame.h"


namespace glabels
{
	namespace model
	{

		class FramePath : public Frame
		{
			Q_DECLARE_TR_FUNCTIONS(FramePath)

		public:
			FramePath( const QPainterPath& path,
			           const Distance&     xWaste,
			           const Distance&     yWaste,
			           const Units&        originalUnits,
			           const QString&      id = "0" );

			FramePath( const FramePath& other ) = default;

			Frame* dup() const override;

			Distance xWaste() const;
			Distance yWaste() const;

			Units        originalUnits() const;

			Distance w() const override;
			Distance h() const override;

			QString sizeDescription( const Units& units ) const override;

			bool isSimilarTo( Frame* other ) const override;

			const QPainterPath& path() const override;
			const QPainterPath& clipPath() const override;
			QPainterPath marginPath( const Distance& xSize,
			                         const Distance& ySize ) const override;


		private:
			Distance mW;
			Distance mH;
			Distance mXWaste;
			Distance mYWaste;

			QPainterPath mPath;
			QPainterPath mClipPath;

			Units mOriginalUnits;
		};

	}
}


// Debugging support
QDebug operator<<( QDebug dbg, const glabels::model::FramePath& frame );


#endif // model_FramePath_h
