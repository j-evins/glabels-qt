/*  FrameRound.h
 *
 *  Copyright (C) 2013-2016  Jaye Evins <evins@snaught.com>
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

#ifndef model_FrameRound_h
#define model_FrameRound_h


#include "Frame.h"


namespace glabels
{
	namespace model
	{

		class FrameRound : public Frame
		{
			Q_DECLARE_TR_FUNCTIONS(FrameRound)

		public:
			FrameRound( Distance       r,
			            Distance       waste,
			            const QString& id = "0" );

			FrameRound( const FrameRound &other ) = default;

			std::unique_ptr<Frame> clone() const override;

			Distance r() const;
			Distance waste() const;

			Distance w() const override;
			Distance h() const override;

			QString sizeDescription( Units units ) const override;
			bool isSimilarTo( const Frame& other ) const override;

			const QPainterPath& path() const override;
			const QPainterPath& clipPath() const override;
			QPainterPath marginPath( Distance xSize, Distance ySize ) const override;

			// Debugging support
			void print( QDebug& dbg ) const override;


		private:
			Distance mR;
			Distance mWaste;

			QPainterPath mPath;
			QPainterPath mClipPath;

		};

	}
}


#endif // model_FrameRound_h
