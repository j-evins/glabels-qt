/*  Template.h
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

#ifndef model_Template_h
#define model_Template_h


#include "Distance.h"
#include "Frame.h"
#include "Point.h"

#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QList>


namespace glabels
{
	namespace model
	{

		class Template
		{
			Q_DECLARE_TR_FUNCTIONS(Template)

		public:

			Template() = default;

			Template( const QString& brand,
			          const QString& part,
			          const QString& description,
			          const QString& paperId,
			          Distance       pageWidth,
			          Distance       pageHeight,
			          Distance       rollWidth = 0,
			          const QString& fileName = "",
			          bool           isUserDefined = false );

			Template( const Template& other );

			~Template() = default;

			Template& operator=( const Template& other );

			// From equivalent part number
			static Template fromEquiv( const QString& brand,
			                           const QString& part,
			                           const QString& equivPart );

			static QString brandPartToName( const QString& brand,
			                                const QString& part );

			bool isNull() const;

			QString brand() const;
			QString part() const;
			QString description() const;

			QString paperDescription( Units units ) const;
			QString paperId() const;
			Distance pageWidth() const;
			Distance pageHeight() const;
			Distance rollWidth() const;
			bool isSizeIso() const;
			bool isSizeUs() const;
			bool isSizeOther() const;
			bool isRoll() const;

			QString fileName() const;
			void setFileName( const QString& fileName );

			bool isUserDefined() const;
			void setIsUserDefined( bool isUserDefined );

			QString equivPart() const;
			void setEquivPart( const QString& value );

			QString productUrl() const;
			void setProductUrl( const QString& value );

			QString name() const;

			void addCategory( const QString& categoryId );
			void addFrame( const Frame& frame );

			const Frame* frame( const QString& id = "0" ) const;

			bool operator==( const Template& other ) const;

			bool hasCategory( const QString& categoryId ) const;
			bool isSimilarTo( const Template& other ) const;

			bool setH( Distance h );


		private:
			QString        mBrand;
			QString        mPart;
			QString        mDescription;

			QString        mPaperId;
			Distance       mPageWidth;
			Distance       mPageHeight;
			Distance       mRollWidth;

			bool           mIsSizeIso{ false };
			bool           mIsSizeUs{ false };
			bool           mIsRoll{ false };

			QString        mFileName;
			bool           mIsUserDefined{ false };

			QString        mEquivPart;
			QString        mName;

			QString        mProductUrl;
			QStringList    mCategoryIds;

			std::unique_ptr<Frame> mFrame;  // TODO: support multiple frames mapped by ID
		};

	}

}


// Debugging support
QDebug operator<<( QDebug dbg, const glabels::model::Template& tmplate );


#endif // model_Template_h
