/*  FrameContinuous.h
 *
 *  Copyright (C) 2018  Jaye Evins <evins@snaught.com>
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

#ifndef model_FrameContinuous_h
#define model_FrameContinuous_h


#include "Frame.h"


namespace glabels
{
	namespace model
	{

		class FrameContinuous : public Frame
		{
			Q_DECLARE_TR_FUNCTIONS(FrameContinuous)

		public:
			FrameContinuous( Distance       w,
			                 Distance       hMin,
			                 Distance       hMax,
			                 Distance       hDefault,
			                 const QString& id = "0" );

			FrameContinuous( const FrameContinuous& other ) = default;

			std::unique_ptr<Frame> clone() const override;

			Distance w() const override;
			Distance h() const override;

			Distance hMin() const;
			Distance hMax() const;
			Distance hDefault() const;

			bool setH( Distance h ) override;

			QString sizeDescription( Units units ) const override;

			bool isSimilarTo( const Frame& other ) const override;

			const QPainterPath& path() const override;
			const QPainterPath& clipPath() const override;
			QPainterPath marginPath( Distance xSize, Distance ySize ) const override;

                        // Debugging support
			void print( QDebug& dbg ) const override;


		private:
			Distance mW;
			Distance mHMin;
			Distance mHMax;
			Distance mHDefault;
			Distance mH;

			QPainterPath mPath;
		};

	}
}


#endif // model_FrameContinuous_h
