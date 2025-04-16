/*  ModelImageObject.h
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

#ifndef model_ModelImageObject_h
#define model_ModelImageObject_h


#include "ModelObject.h"

#include <QSvgRenderer>


namespace glabels
{
	namespace model
	{

		///
		/// Label Model Image Object
		///
		class ModelImageObject : public ModelObject
		{
			Q_OBJECT

			///////////////////////////////////////////////////////////////
			// Lifecycle Methods
			///////////////////////////////////////////////////////////////
		public:
			ModelImageObject();

			ModelImageObject( const Distance&  x0,
			                  const Distance&  y0,
			                  const Distance&  w,
			                  const Distance&  h,
			                  bool             lockAspectRatio,
			                  const TextNode&  filenameNode,
			                  const QTransform& matrix = QTransform(),
			                  bool             shadowState = false,
			                  const Distance&  shadowX = 0,
			                  const Distance&  shadowY = 0,
			                  double           shadowOpacity = 1.0,
			                  const ColorNode& shadowColorNode = ColorNode() );

			ModelImageObject( const Distance&  x0,
			                  const Distance&  y0,
			                  const Distance&  w,
			                  const Distance&  h,
			                  bool             lockAspectRatio,
			                  const QString&   filename,
			                  const QImage&    image,
			                  const QTransform& matrix = QTransform(),
			                  bool             shadowState = false,
			                  const Distance&  shadowX = 0,
			                  const Distance&  shadowY = 0,
			                  double           shadowOpacity = 1.0,
			                  const ColorNode& shadowColorNode = ColorNode() );

			ModelImageObject( const Distance&   x0,
			                  const Distance&   y0,
			                  const Distance&   w,
			                  const Distance&   h,
			                  bool              lockAspectRatio,
			                  const QString&    filename,
			                  const QByteArray& svg,
			                  const QTransform& matrix = QTransform(),
			                  bool              shadowState = false,
			                  const Distance&   shadowX = 0,
			                  const Distance&   shadowY = 0,
			                  double            shadowOpacity = 1.0,
			                  const ColorNode&  shadowColorNode = ColorNode() );

			ModelImageObject( const ModelImageObject* object );
		
			~ModelImageObject() override;


			///////////////////////////////////////////////////////////////
			// Object duplication
			///////////////////////////////////////////////////////////////
			ModelImageObject* clone() const override;


			///////////////////////////////////////////////////////////////
			// Property Implementations
			///////////////////////////////////////////////////////////////
		public:
			//
			// Image Property: filenameNode
			//
			TextNode filenameNode() const override;
			void setFilenameNode( const TextNode& value ) override;

			//
			// Image Property: image
			//
			const QImage* image() const override;
			void setImage( const QImage& value ) override;
			void setImage( const QString& name, const QImage& value ) override;

			//
			// Image Property: svg
			//
			QByteArray svg() const override;
			void setSvg( const QString& name, const QByteArray& value ) override;

			//
			// Property: naturalSize
			//
			Size naturalSize() const override;
	

			///////////////////////////////////////////////////////////////
			// Capability Implementations
			///////////////////////////////////////////////////////////////


			///////////////////////////////////////////////////////////////
			// Drawing operations
			///////////////////////////////////////////////////////////////
		protected:
			void drawShadow( QPainter*      painter,
			                 bool           inEditor,
			                 merge::Record* record,
			                 Variables*     variables ) const override;
			
			void drawObject( QPainter*      painter,
			                 bool           inEditor,
			                 merge::Record* record,
			                 Variables*     variables ) const override;
			
			QPainterPath hoverPath( double scale ) const override;


			///////////////////////////////////////////////////////////////
			// Private
			///////////////////////////////////////////////////////////////
			void loadImage();

			bool readImageFile( const QString& fileName,
			                    QImage*&       image,
			                    QSvgRenderer*& svgRenderer,
			                    QByteArray&    svg ) const;

			QImage* createShadowImage( const QImage& image,
			                           const QColor& color ) const;
	

			///////////////////////////////////////////////////////////////
			// Private Members
			///////////////////////////////////////////////////////////////
		protected:
			TextNode       mFilenameNode;
			QImage*        mImage;
			QSvgRenderer*  mSvgRenderer;
			QByteArray     mSvg;

			static QImage* smDefaultImage;

		};

	}
}


#endif // model_ModelImageObject_h
