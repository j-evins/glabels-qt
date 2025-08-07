/*  LabelEditor.h
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

#ifndef LabelEditor_h
#define LabelEditor_h


#include "model/Handles.h"
#include "model/Model.h"
#include "model/ModelObject.h"
#include "model/Region.h"

#include <QPainter>
#include <QScrollArea>
#include <QWidget>


namespace glabels
{

	// Forward References
	class UndoRedoModel;


	///
	/// LabelEditor Widget
	///
	class LabelEditor : public QWidget
	{
		Q_OBJECT

		/////////////////////////////////////
		// Lifecycle
		/////////////////////////////////////
	public:
		LabelEditor( QScrollArea* scrollArea, QWidget* parent = nullptr );


		/////////////////////////////////////
		// Signals
		/////////////////////////////////////
	signals:
		void contextMenuActivate( model::Point p );
		void zoomChanged();
		void pointerMoved( model::Point p );
		void pointerExited();
		void modeChanged();


		/////////////////////////////////////
		// Parameters
		/////////////////////////////////////
	public:
		double zoom() const;
		bool markupVisible() const;
		bool qridVisible() const;


		/////////////////////////////////////
		// Model
		/////////////////////////////////////
	public:
		void setModel( model::Model* model, UndoRedoModel* undoRedoModel );


		/////////////////////////////////////
		// Visibility operations
		/////////////////////////////////////
	public:
		void setGridVisible( bool visibleFlag );
		void setMarkupVisible( bool visibleFlag );


		/////////////////////////////////////
		// Zoom operations
		/////////////////////////////////////
	public:
		void zoomIn();
		void zoomOut();
		void zoom1To1();
		void zoomToFit();
		bool isZoomMax() const;
		bool isZoomMin() const;
	private:
		void setZoomReal( double zoom, bool zoomToFitFlag );


		/////////////////////////////////////
		// Mode operations
		/////////////////////////////////////
	public:
		void arrowMode();
		void createBoxMode();
		void createEllipseMode();
		void createLineMode();
		void createImageMode();
		void createTextMode();
		void createBarcodeMode();


		/////////////////////////////////////
		// Event handlers
		/////////////////////////////////////
	protected:
		void resizeEvent( QResizeEvent* event ) override;
		void mousePressEvent( QMouseEvent* event ) override;
		void mouseMoveEvent( QMouseEvent* event ) override;
		void mouseReleaseEvent( QMouseEvent* event ) override;
		void leaveEvent( QEvent* event ) override;
		void keyPressEvent( QKeyEvent* event ) override;
		void paintEvent( QPaintEvent* event ) override;
		void dragEnterEvent( QDragEnterEvent *event ) override;
		void dragMoveEvent( QDragMoveEvent *event ) override;
		void dropEvent( QDropEvent *event ) override;


		/////////////////////////////////////
		// Private methods
		/////////////////////////////////////
	private:
		void handleResizeMotion( const model::Distance& xWorld,
		                         const model::Distance& yWorld );

		void drawBgLayer( QPainter* painter );
		void drawGridLayer( QPainter* painter );
		void drawMarkupLayer( QPainter* painter );
		void drawObjectsLayer( QPainter* painter );
		void drawFgLayer( QPainter* painter );
		void drawHighlightLayer( QPainter* painter );
		void drawSelectRegionLayer( QPainter* painter );


		/////////////////////////////////////
		// Private slots
		/////////////////////////////////////
	private slots:
		void onSettingsChanged();
		void onModelSizeChanged();


		/////////////////////////////////////
		// Private data
		/////////////////////////////////////
	private:
		enum State {
			IdleState,
			ArrowSelectRegion,
			ArrowMove,
			ArrowResize,
			CreateIdle,
			CreateDrag
		};

		enum CreateType {
			Box,
			Ellipse,
			Line,
			Image,
			Text,
			Barcode
		};

		QScrollArea*         mScrollArea;

		model::Model*        mModel;
		UndoRedoModel*       mUndoRedoModel;

		double               mZoom;
		bool                 mZoomToFitFlag;
		double               mScale;
		model::Distance      mX0;
		model::Distance      mY0;

		bool                 mMarkupVisible;
		bool                 mGridVisible;

		double               mGridSpacing;
		model::Distance      mStepSize;

		State                mState;

		/* ArrowSelectRegion state */
		bool                 mSelectRegionVisible;
		model::Region        mSelectRegion;

		/* ArrowMove state */
		model::Distance      mMoveLastX;
		model::Distance      mMoveLastY;

		/* ArrowResize state */
		model::ModelObject*  mResizeObject;
		model::Handle*       mResizeHandle;
		bool                 mResizeHonorAspect;

		/* CreateDrag state */
		CreateType           mCreateObjectType;
		model::ModelObject*  mCreateObject;
		model::Distance      mCreateX0;
		model::Distance      mCreateY0;


	};

}


#endif // LabelEditor_h
