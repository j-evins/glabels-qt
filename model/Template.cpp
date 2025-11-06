/*  Template.cpp
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


#include "Template.h"

#include "Db.h"
#include "FrameContinuous.h"
#include "StrUtil.h"

#include <QtDebug>


namespace glabels
{
	namespace model
	{

		Template::Template( const QString& brand,
		                    const QString& part,
		                    const QString& description,
		                    const QString& paperId,
		                    Distance       pageWidth,
		                    Distance       pageHeight,
		                    Distance       rollWidth,
		                    const QString& fileName,
		                    bool           isUserDefined )
			: mBrand(brand),
			  mPart(part),
			  mDescription(description),
			  mPaperId(paperId),
			  mPageWidth(pageWidth),
			  mPageHeight(pageHeight),
			  mRollWidth(rollWidth),
			  mFileName(fileName),
			  mIsUserDefined(isUserDefined)
		{
			mName = brandPartToName( brand, part );

			if ( Db::isPaperIdKnown( paperId ) )
			{
				auto paper = Db::lookupPaperFromId( paperId );
				mIsSizeIso = paper.isSizeIso();
				mIsSizeUs  = paper.isSizeUs();
			}

			mIsRoll = (paperId == "roll");
		}


		Template::Template( const Template& other )
		{
			mBrand         = other.mBrand;
			mPart          = other.mPart;
			mDescription   = other.mDescription;
			mPaperId       = other.mPaperId;
			mPageWidth     = other.mPageWidth;
			mPageHeight    = other.mPageHeight;
			mRollWidth     = other.mRollWidth;
			mIsSizeIso     = other.mIsSizeIso;
			mIsSizeUs      = other.mIsSizeUs;
			mIsRoll        = other.mIsRoll;
			mEquivPart     = other.mEquivPart;
			mName          = other.mName;
			mProductUrl    = other.mProductUrl;
			mFileName      = other.mFileName;
			mIsUserDefined = other.mIsUserDefined;

			if ( other.mFrame )
			{
				mFrame = other.mFrame->clone();
			}

			for ( auto& categoryId : other.mCategoryIds )
			{
				addCategory( categoryId );
			}
		}


		Template& Template::operator=( const Template& other )
		{
			if ( this != &other )
			{
				mBrand         = other.mBrand;
				mPart          = other.mPart;
				mDescription   = other.mDescription;
				mPaperId       = other.mPaperId;
				mPageWidth     = other.mPageWidth;
				mPageHeight    = other.mPageHeight;
				mRollWidth     = other.mRollWidth;
				mIsSizeIso     = other.mIsSizeIso;
				mIsSizeUs      = other.mIsSizeUs;
				mIsRoll        = other.mIsRoll;
				mEquivPart     = other.mEquivPart;
				mName          = other.mName;
				mProductUrl    = other.mProductUrl;
				mFileName      = other.mFileName;
				mIsUserDefined = other.mIsUserDefined;

				if ( other.mFrame )
				{
					mFrame = other.mFrame->clone();
				}

				mCategoryIds.clear();
				for ( auto& categoryId : other.mCategoryIds )
				{
					addCategory( categoryId );
				}
			}

			return *this;
		}


		// From equivalent part number
		Template Template::fromEquiv( const QString& brand,
		                              const QString& part,
		                              const QString& equivPart )
		{
			if ( Db::isTemplateKnown( brand, equivPart ) )
			{
				auto tmplate = Db::lookupTemplateFromBrandPart( brand, equivPart );

				tmplate.mPart      = part;
				tmplate.mEquivPart = equivPart;

				tmplate.mName = brandPartToName( brand, part );

				return tmplate;
			}
			else
			{
				qWarning() << "Error: cannot create equivalent template for "
				           << brand << ", " << equivPart
				           << ". Forward references not supported.";
				return Template();
			}
		}


		QString Template::brandPartToName( const QString& brand,
		                                   const QString& part )
		{
			return QString( "%1 %2" ).arg( brand ).arg( part );
		}


		bool Template::isNull() const
		{
			return mBrand.isEmpty() || mPart.isEmpty();
		}

	
		QString Template::brand() const
		{
			return mBrand;
		}

	
		QString Template::part() const
		{
			return mPart;
		}

	
		QString Template::description() const
		{
			return mDescription;
		}
	

		QString Template::paperDescription( Units units ) const
		{
			if ( mPaperId == "other" )
			{
				if ( units.toEnum() == Units::IN )
				{
					QString wStr = StrUtil::formatFraction( mPageWidth.in() );
					QString hStr = StrUtil::formatFraction( mPageHeight.in() );

					return QString("%1 x %2 %3").arg(wStr).arg(hStr).arg(units.toTrName());
				}
				else
				{
					return QString("%1 x %2 %3")
						.arg(mPageWidth.inUnits(units), 0, 'g', 5)
						.arg(mPageHeight.inUnits(units), 0, 'g', 5)
						.arg(units.toTrName());
				}
			}

			return  Db::lookupPaperNameFromId( mPaperId );
		}


		QString Template::paperId() const
		{
			return mPaperId;
		}

	
		Distance Template::pageWidth() const
		{
			return mPageWidth;
		}

	
		Distance Template::pageHeight() const
		{
			// Adjust height if continuous tape
			if ( const auto* frameContinuous = dynamic_cast<const model::FrameContinuous*>( mFrame.get() ) )
			{
				return frameContinuous->h();
			}
			else
			{
				return mPageHeight;
			}
		}

	
		Distance Template::rollWidth() const
		{
			return mRollWidth;
		}

	
		bool Template::isSizeIso() const
		{
			return mIsSizeIso;
		}

	
		bool Template::isSizeUs() const
		{
			return mIsSizeUs;
		}

	
		bool Template::isSizeOther() const
		{
			return !mIsSizeIso && !mIsSizeUs;
		}
	

		bool Template::isRoll() const
		{
			return mIsRoll;
		}
	

		QString Template::fileName() const
		{
			return mFileName;
		}

	
		void Template::setFileName( const QString& value )
		{
			mFileName = value;
		}
	

		bool Template::isUserDefined() const
		{
			return mIsUserDefined;
		}
	

		void Template::setIsUserDefined( bool isUserDefined )
		{
			mIsUserDefined = isUserDefined;
		}
	

		QString Template::equivPart() const
		{
			return mEquivPart;
		}

	
		void Template::setEquivPart( const QString& value )
		{
			mEquivPart = value;
		}
	

		QString Template::productUrl() const
		{
			return mProductUrl;
		}

	
		void Template::setProductUrl( const QString& value )
		{
			mProductUrl = value;
		}
	

		QString Template::name() const
		{
			return mName;
		}
	

		const Frame* Template::frame( const QString& id ) const
		{
			return mFrame.get();
		}


		void Template::addCategory( const QString& categoryId )
		{
			mCategoryIds << categoryId;
		}


		void Template::addFrame( const Frame& frame )
		{
			mFrame = frame.clone();
		}


		bool Template::operator==( const Template& other ) const
		{
			return (mBrand == other.mBrand) && (mPart == other.mPart);
		}


		bool Template::hasCategory( const QString& categoryId ) const
		{
			for ( auto& testCategoryId : mCategoryIds )
			{
				if ( categoryId == testCategoryId )
				{
					return true;
				}
			}

			return false;
		}


		bool Template::isSimilarTo( const Template& other ) const
		{
			// Does page size match?
			if ( (mPaperId    != other.mPaperId)    ||
			     (mPageWidth  != other.mPageWidth ) ||
			     (mPageHeight != other.mPageHeight ) )
			{
				return false;
			}

			// Are frames similar
			auto& frame1 = mFrame;
			auto& frame2 = other.mFrame;
			if ( !frame1->isSimilarTo( *frame2 ) )
			{
				return false;
			}

			// Are they layed out similarly?
			for ( auto& layout1 : frame1->layouts() )
			{
				bool matchFound = false;
				for ( auto& layout2 : frame2->layouts() )
				{
					if ( layout1.isSimilarTo( layout2 ) )
					{
						matchFound = true;
						break;
					}
				}
				if ( !matchFound )
				{
					return false;
				}
			}

			return true;
		}


		bool Template::setH( Distance h )
		{
			if ( mFrame )
			{
				return mFrame->setH( h );
			}
			else
			{
				return false;
			}
		}

	}
}


QDebug operator<<( QDebug dbg, const glabels::model::Template& tmplate )
{
	QDebugStateSaver saver(dbg);

	dbg.nospace() << "Template{ "
	              << tmplate.brand() << "," << tmplate.part() << "," << tmplate.description() << ","
	              << tmplate.paperId() << ","
	              << tmplate.pageWidth() << ","
	              << tmplate.pageHeight() << ","
	              << tmplate.rollWidth() << ","
	              << tmplate.isSizeIso() << ","
	              << tmplate.isSizeUs() << ","
	              << tmplate.isSizeOther() << ","
	              << tmplate.isRoll() << ","
	              << *tmplate.frame() << ","
	              << " }";
	return dbg;
}
