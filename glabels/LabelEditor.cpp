/*  LabelEditor.cpp
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

#include "LabelEditor.h"

#include "Cursors.h"
#include "UndoRedoModel.h"

#include "model/FrameCd.h"
#include "model/FrameEllipse.h"
#include "model/FrameRect.h"
#include "model/FrameRound.h"
#include "model/Model.h"
#include "model/ModelObject.h"
#include "model/ModelBarcodeObject.h"
#include "model/ModelBoxObject.h"
#include "model/ModelEllipseObject.h"
#include "model/ModelImageObject.h"
#include "model/ModelLineObject.h"
#include "model/ModelTextObject.h"
#include "model/Markup.h"
#include "model/Settings.h"

#include <QMimeData>
#include <QMouseEvent>
#include <QtMath>
#include <QtDebug>


namespace glabels
{

	//
	// Private
	//
	namespace
	{
		const int     nZoomLevels = 11;
		const double  zoomLevels[nZoomLevels] = { 8, 6, 4, 3, 2, 1.5, 1, 0.75, 0.67, 0.50, 0.33 };

		const double  ZOOM_TO_FIT_PAD = 16.0;

		const QColor  backgroundColor( 192, 192, 192 );

		const QColor  shadowColor( 64, 64, 64, 128 );
		const double  shadowOffsetPixels = 4;

		const QColor  labelColor( 255, 255, 255 );
		const QColor  labelOutlineColor( 0, 0, 0 );
		const double  labelOutlineWidthPixels = 1;

		const QColor  gridLineColor( 192, 192, 192 );
		const double  gridLineWidthPixels = 1;
		const model::Distance gridSpacing = model::Distance::pt(9); // TODO: determine from locale.

		const QColor  markupLineColor( 240, 99, 99 );
		const double  markupLineWidthPixels = 1;

		const QColor  selectRegionFillColor( 192, 192, 255, 128 );
		const QColor  selectRegionOutlineColor( 0, 0, 255, 128 );
		const double  selectRegionOutlineWidthPixels = 3;
	}


	///
	/// Constructor
	///
	LabelEditor::LabelEditor( QScrollArea* scrollArea, QWidget* parent )
		: QWidget(parent), mScrollArea(scrollArea)
	{
		mModel              = nullptr;
		mUndoRedoModel      = nullptr;

		mZoom               = 1;
		mZoomToFitFlag      = false;
		mScale              = 1;
		mMarkupVisible      = true;
		mGridVisible        = true;
		mGridSpacing        = 18;

		mState = IdleState;

		mSelectRegionVisible = false;
		mResizeObject        = nullptr;
		mResizeHandle        = nullptr;
		mResizeHonorAspect   = false;
		mCreateObjectType    = Box;
		mCreateObject        = nullptr;

		setMouseTracking( true );
		setFocusPolicy(Qt::StrongFocus);
		setAcceptDrops( true );

		connect( model::Settings::instance(), SIGNAL(changed()), this, SLOT(onSettingsChanged()) );
		onSettingsChanged();
	}


	///
	/// Zoom property
	///
	double
	LabelEditor::zoom() const
	{
		return mZoom;
	}


	///
	/// Markup visible? property
	///
	bool
	LabelEditor::markupVisible() const
	{
		return mMarkupVisible;
	}


	///
	/// Grid visible? property
	///
	bool
	LabelEditor::qridVisible() const
	{
		return mGridVisible;
	}


	///
	/// Model Parameter Setter
	///
	void
	LabelEditor::setModel( model::Model* model, UndoRedoModel* undoRedoModel )
	{
		mModel = model;
		mUndoRedoModel = undoRedoModel;

		if ( model )
		{
			zoomToFit();

			connect( model, SIGNAL(changed()), this, SLOT(update()) );
			connect( model, SIGNAL(selectionChanged()), this, SLOT(update()) );
			connect( model, SIGNAL(sizeChanged()), this, SLOT(onModelSizeChanged()) );

			update();
		}
	}


	///
	/// Grid Visibility Parameter Setter
	///
	void
	LabelEditor::setGridVisible( bool visibleFlag )
	{
		mGridVisible = visibleFlag;
		update();
	}


	///
	/// Markup Visibility Parameter Setter
	///
	void
	LabelEditor::setMarkupVisible( bool visibleFlag )
	{
		mMarkupVisible = visibleFlag;
		update();
	}


	///
	/// Zoom In "One Notch"
	///
	void
	LabelEditor::zoomIn()
	{
		// Find closest standard zoom level to our current zoom
		// Start with 2nd largest scale
		int i_min = 1;
		double dist_min = qFabs( zoomLevels[1] - mZoom );

		for ( int i = 2; i < nZoomLevels; i++ )
		{
			double dist = qFabs( zoomLevels[i] - mZoom );
			if ( dist < dist_min )
			{
				i_min = i;
				dist_min = dist;
			}
		}

		// Zoom in one notch
		setZoomReal( zoomLevels[i_min-1], false );
	}


	///
	/// Zoom Out "One Notch"
	///
	void
	LabelEditor::zoomOut()
	{
		// Find closest standard zoom level to our current zoom
		// Start with largest scale, end on 2nd smallest
		int i_min = 0;
		double dist_min = qFabs( zoomLevels[0] - mZoom );

		for ( int i = 1; i < (nZoomLevels-1); i++ )
		{
			double dist = qFabs( zoomLevels[i] - mZoom );
			if ( dist < dist_min )
			{
				i_min = i;
				dist_min = dist;
			}
		}

		// Zoom out one notch
		setZoomReal( zoomLevels[i_min+1], false );
	}


	///
	/// Zoom To 1:1 Scale
	///
	void
	LabelEditor::zoom1To1()
	{
		setZoomReal( 1.0, false );
	}


	///
	/// Zoom To Fit
	///
	void
	LabelEditor::zoomToFit()
	{
		double wPixels = mScrollArea->maximumViewportSize().width();
		double hPixels = mScrollArea->maximumViewportSize().height();
	
		double x_scale = ( wPixels - ZOOM_TO_FIT_PAD ) / mModel->w().pt();
		double y_scale = ( hPixels - ZOOM_TO_FIT_PAD ) / mModel->h().pt();
		double newZoom = qMin( x_scale, y_scale ) * model::PTS_PER_INCH / physicalDpiX();

		// Limits
		newZoom = qMin( newZoom, zoomLevels[0] );
		newZoom = qMax( newZoom, zoomLevels[nZoomLevels-1] );

		setZoomReal( newZoom, true );
	}


	///
	/// Is Zoom at Maximum?
	///
	bool
	LabelEditor::isZoomMax() const
	{
		return mZoom >= zoomLevels[0];
	}


	///
	/// Is Zoom at Minimum?
	///
	bool
	LabelEditor::isZoomMin() const
	{
		return mZoom <= zoomLevels[nZoomLevels-1];
	}


	///
	/// Set Zoom to Value
	///
	void
	LabelEditor::setZoomReal( double zoom, bool zoomToFitFlag )
	{
		mZoom          = zoom;
		mZoomToFitFlag = zoomToFitFlag;

		/* Actual scale depends on DPI of display (assume DpiX == DpiY). */
		mScale = zoom * physicalDpiX() / model::PTS_PER_INCH;

		setMinimumSize( mScale*mModel->w().pt() + ZOOM_TO_FIT_PAD,
		                mScale*mModel->h().pt() + ZOOM_TO_FIT_PAD );

		/* Adjust origin to center label in widget. */
		mX0 = (width()/mScale - mModel->w()) / 2;
		mY0 = (height()/mScale - mModel->h()) / 2;

		update();

		emit zoomChanged();
	}


	///
	/// Arrow mode (normal mode)
	///
	void
	LabelEditor::arrowMode()
	{
		setCursor( Qt::ArrowCursor );

		mState = IdleState;
	}


	///
	/// Create box mode
	///
	void
	LabelEditor::createBoxMode()
	{
		setCursor( Cursors::Box() );

		mCreateObjectType = Box;
		mState = CreateIdle;
	}


	///
	/// Create ellipse mode
	///
	void
	LabelEditor::createEllipseMode()
	{
		setCursor( Cursors::Ellipse() );

		mCreateObjectType = Ellipse;
		mState = CreateIdle;
	}


	///
	/// Create image mode
	///
	void
	LabelEditor::createImageMode()
	{
		setCursor( Cursors::Image() );

		mCreateObjectType = Image;
		mState = CreateIdle;
	}


	///
	/// Create line mode
	///
	void
	LabelEditor::createLineMode()
	{
		setCursor( Cursors::Line() );

		mCreateObjectType = Line;
		mState = CreateIdle;
	}


	///
	/// Create text mode
	///
	void
	LabelEditor::createTextMode()
	{
		setCursor( Cursors::Text() );

		mCreateObjectType = Text;
		mState = CreateIdle;
	}


	///
	/// Create barcode mode
	///
	void
	LabelEditor::createBarcodeMode()
	{
		setCursor( Cursors::Barcode() );

		mCreateObjectType = Barcode;
		mState = CreateIdle;
	}


	///
	/// Resize Event Handler
	///
	void
	LabelEditor::resizeEvent( QResizeEvent *event )
	{
		if ( mModel )
		{
			if ( mZoomToFitFlag )
			{
				zoomToFit();
			}
			else
			{
				/* Re-adjust origin to center label in widget. */
				mX0 = (width()/mScale - mModel->w()) / 2;
				mY0 = (height()/mScale - mModel->h()) / 2;

				update();
			}
		}
	}


	///
	/// Mouse Button Press Event Handler
	///
	void
	LabelEditor::mousePressEvent( QMouseEvent* event )
	{
		if ( mModel )
		{
			/*
			 * Transform to label coordinates
			 */
			QTransform transform;

			transform.scale( mScale, mScale );
			transform.translate( mX0.pt(), mY0.pt() );

			QPointF pWorld = transform.inverted().map( event->pos() );
			model::Distance xWorld = model::Distance::pt( pWorld.x() );
			model::Distance yWorld = model::Distance::pt( pWorld.y() );

		
			if ( event->button() & Qt::LeftButton )
			{
				//
				// LEFT BUTTON
				//
				switch (mState)
				{

				case IdleState:
				{
					model::ModelObject* object = nullptr;
					model::Handle* handle = nullptr;
					if ( mModel->isSelectionAtomic() &&
					     (handle = mModel->handleAt( mScale, xWorld, yWorld )) != nullptr )
					{
						//
						// Start an object resize
						//
						mResizeObject = handle->owner();
						mResizeHandle = handle;
						mResizeHonorAspect = event->modifiers() & Qt::ControlModifier;
						if ( mResizeObject->lockAspectRatio() )
						{
							mResizeHonorAspect = !mResizeHonorAspect;
						}

						mState = ArrowResize;
					}
					else if ( (object = mModel->objectAt( mScale, xWorld, yWorld )) != nullptr )
					{
						//
						// Start a Move Selection (adjusting selection if necessary)
						//
						if ( event->modifiers() & Qt::ControlModifier )
						{
							if ( object->isSelected() )
							{
								// Un-selecting a selected item
								mModel->unselectObject( object );
							}
							else
							{
								// Add to current selection
								mModel->selectObject( object );
							}
						}
						else
						{
							if ( !object->isSelected() )
							{
								// Replace current selection with this object
								mModel->unselectAll();
								mModel->selectObject( object );
							}
						}

						mMoveLastX = xWorld;
						mMoveLastY = yWorld;

						mState = ArrowMove;
					}
					else
					{
						//
						// Start a Select Region
						//
						if ( !(event->modifiers() & Qt::ControlModifier) )
						{
							mModel->unselectAll();
						}

						mSelectRegionVisible = true;
						mSelectRegion.setX1( xWorld );
						mSelectRegion.setY1( yWorld );
						mSelectRegion.setX2( xWorld );
						mSelectRegion.setY2( yWorld );

						mState = ArrowSelectRegion;
						update();
					}
				}
				break;


				case CreateIdle:
				{
					switch ( mCreateObjectType )
					{
					case Box:
						mCreateObject = new model::ModelBoxObject();
						break;
					case Ellipse:
						mCreateObject = new model::ModelEllipseObject();
						break;
					case Line: 
						mCreateObject = new model::ModelLineObject();
						break;
					case Image:
						mCreateObject = new model::ModelImageObject();
						break;
					case Text:
						mCreateObject = new model::ModelTextObject();
						break;
					case Barcode:
						mCreateObject = new model::ModelBarcodeObject();
						break;
					default:
						qDebug() << "LabelEditor::mousePressEvent: Invalid creation type. Should not happen!";
						break;
					}

					mCreateObject->setPosition( xWorld, yWorld );
					mCreateObject->setSize( 0, 0 );
					mModel->addObject( mCreateObject );

					mModel->unselectAll();
					mModel->selectObject( mCreateObject );

					mCreateX0 = xWorld;
					mCreateY0 = yWorld;

					mState = CreateDrag;
				}
				break;

				
				default:
				{
					qDebug() << "LabelEditor::mousePressEvent: Invalid state. Should not happen!";
				}
				break;

				}

			}
			else if ( event->button() & Qt::RightButton )
			{
				//
				// RIGHT BUTTON
				//
				if ( mState == IdleState )
				{
					emit contextMenuActivate( model::Point( xWorld, yWorld ) );
				}
			}
		}
	}


	///
	/// Mouse Movement Event Handler
	///
	void
	LabelEditor::mouseMoveEvent( QMouseEvent* event )
	{
		if ( mModel )
		{
			/*
			 * Transform to label coordinates
			 */
			QTransform transform;

			transform.scale( mScale, mScale );
			transform.translate( mX0.pt(), mY0.pt() );

			QPointF pWorld = transform.inverted().map( event->pos() );
			model::Distance xWorld = model::Distance::pt( pWorld.x() );
			model::Distance yWorld = model::Distance::pt( pWorld.y() );

		
			/*
			 * Emit signal regardless of mode
			 */
			emit pointerMoved( model::Point( xWorld, yWorld ) );


			/*
			 * Handle event as appropriate for state
			 */
			switch (mState)
			{

			case IdleState:
				if ( mModel->isSelectionAtomic() &&
				     mModel->handleAt( mScale, xWorld, yWorld ) )
				{
					setCursor( Qt::CrossCursor );
				}
				else if ( mModel->objectAt( mScale, xWorld, yWorld ) )
				{
					setCursor( Qt::SizeAllCursor );
				}
				else
				{
					setCursor( Qt::ArrowCursor );
				}
				break;

			case ArrowSelectRegion:
				mSelectRegion.setX2( xWorld );
				mSelectRegion.setY2( yWorld );
				update();
				break;

			case ArrowMove:
				mUndoRedoModel->checkpoint( tr("Move") );
				mModel->moveSelection( (xWorld - mMoveLastX),
				                       (yWorld - mMoveLastY) );
				mMoveLastX = xWorld;
				mMoveLastY = yWorld;
				break;

			case ArrowResize:
				mUndoRedoModel->checkpoint( tr("Resize") );
				handleResizeMotion( xWorld, yWorld );
				break;

			case CreateIdle:
				break;

			case CreateDrag:
				switch (mCreateObjectType)
				{
				case Box:
				case Ellipse:
				case Image:
				case Text:
				case Barcode:
					mCreateObject->setPosition( min( xWorld, mCreateX0 ),
					                            min( yWorld, mCreateY0 ) );
					mCreateObject->setSize( max(xWorld,mCreateX0) - min(xWorld,mCreateX0),
					                        max(yWorld,mCreateY0) - min(yWorld,mCreateY0) );
								
					break;
				case Line:
					mCreateObject->setSize( xWorld - mCreateX0, yWorld - mCreateY0 );
					break;
				default:
					qDebug() << "LabelEditor::mouseMoveEvent: Invalid creation mode. Should not happen!";
					break;
				}
				break;

			default:
				qDebug() << "LabelEditor::mouseMoveEvent: Invalid state. Should not happen!";
				break;

			}
		}
	}


	///
	/// Mouse Button Release Event Handler
	///
	void
	LabelEditor::mouseReleaseEvent( QMouseEvent* event )
	{
		if ( mModel )
		{
			/*
			 * Transform to label coordinates
			 */
			QTransform transform;

			transform.scale( mScale, mScale );
			transform.translate( mX0.pt(), mY0.pt() );

			QPointF pWorld = transform.inverted().map( event->pos() );
			model::Distance xWorld = model::Distance::pt( pWorld.x() );
			model::Distance yWorld = model::Distance::pt( pWorld.y() );

		
			if ( event->button() & Qt::LeftButton )
			{
				//
				// LEFT BUTTON Release
				//
				switch (mState)
				{

				case ArrowResize:
					mState = IdleState;
					break;

				case ArrowSelectRegion:
					mSelectRegionVisible = false;
					mSelectRegion.setX2( xWorld );
					mSelectRegion.setY2( yWorld );

					mModel->selectRegion( mSelectRegion );

					mState = IdleState;
					update();
					break;

				case CreateDrag:
					if ( (fabs(mCreateObject->w()) < 4) && (fabs(mCreateObject->h()) < 4) )
					{
						switch (mCreateObjectType)
						{
						case Text:
							mCreateObject->setSize( 72, 36 );
							break;
						case Barcode:
							mCreateObject->setSize( 72, 36 );
							break;
						case Line:
							mCreateObject->setSize( 72, 0 );
							break;
						default:
							mCreateObject->setSize( 72, 72 );
							break;
						}
					}

					setCursor( Qt::ArrowCursor );
					mState = IdleState;
					break;

				default:
					mState = IdleState;
					break;

				}

			}
		}
	}


	///
	/// Leave Event Handler
	///
	void
	LabelEditor::leaveEvent( QEvent* event )
	{
		if ( mModel )
		{
			emit pointerExited();
		}
	}


	///
	/// Handle resize motion
	///
	void
	LabelEditor::handleResizeMotion( const model::Distance& xWorld,
	                                 const model::Distance& yWorld )
	{
		QPointF p( xWorld.pt(), yWorld.pt() );
		model::Handle::Location location = mResizeHandle->location();
	
		/*
		 * Change point to object relative coordinates
		 */
		p -= QPointF( mResizeObject->x0().pt(), mResizeObject->y0().pt() );
		p = mResizeObject->matrix().inverted().map( p );

		/*
		 * Initialize origin and 2 corners in object relative coordinates.
		 */
		double x0 = 0.0;
		double y0 = 0.0;

		double x1 = 0.0;
		double y1 = 0.0;

		double x2 = mResizeObject->w().pt();
		double y2 = mResizeObject->h().pt();

		/*
		 * Calculate new size
		 */
		double w, h;
		switch ( location )
		{
		case model::Handle::NW:
			w = std::max( x2 - p.x(), 0.0 );
			h = std::max( y2 - p.y(), 0.0 );
			break;
		case model::Handle::N:
			w = x2 - x1;
			h = std::max( y2 - p.y(), 0.0 );
			break;
		case model::Handle::NE:
			w = std::max( p.x() - x1, 0.0 );
			h = std::max( y2 - p.y(), 0.0 );
			break;
		case model::Handle::E:
			w = std::max( p.x() - x1, 0.0 );
			h = y2 - y1;
			break;
		case model::Handle::SE:
			w = std::max( p.x() - x1, 0.0 );
			h = std::max( p.y() - y1, 0.0 );
			break;
		case model::Handle::S:
			w = x2 - x1;
			h = std::max( p.y() - y1, 0.0 );
			break;
		case model::Handle::SW:
			w = std::max( x2 - p.x(), 0.0 );
			h = std::max( p.y() - y1, 0.0 );
			break;
		case model::Handle::W:
			w = std::max( x2 - p.x(), 0.0 );
			h = y2 - y1;
			break;
		case model::Handle::P1:
			x1 = p.x();
			y1 = p.y();
			w  = x2 - p.x();
			h  = y2 - p.y();
			x0 = x0 + x1;
			y0 = y0 + y1;
			break;
		case model::Handle::P2:
			w  = p.x() - x1;
			h  = p.y() - y1;
			x0 = x0 + x1;
			y0 = y0 + y1;
			break;
		default:
			qDebug() << "LabelEditor::handleResizeMotion: Invalid Handle Location. Should not happen!";
		}

		/*
		 * Set size
		 */
		if ( !(location == model::Handle::P1) && !(location == model::Handle::P2) )
		{
			if ( mResizeHonorAspect )
			{
				switch ( location )
				{
				case model::Handle::E:
				case model::Handle::W:
					mResizeObject->setWHonorAspect( model::Distance::pt(w) );
					break;
				case model::Handle::N:
				case model::Handle::S:
					mResizeObject->setHHonorAspect( model::Distance::pt(h) );
					break;
				default:
					mResizeObject->setSizeHonorAspect( model::Distance::pt(w),
					                                   model::Distance::pt(h) );
					break;
				}
			}
			else
			{
				mResizeObject->setSize( model::Distance::pt(w),
				                        model::Distance::pt(h) );
			}

			/*
			 * Adjust origin, if needed.
			 */
			switch ( location )
			{
			case model::Handle::NW:
				x0 += x2 - mResizeObject->w().pt();
				y0 += y2 - mResizeObject->h().pt();
				break;
			case model::Handle::N:
			case model::Handle::NE:
				y0 += y2 - mResizeObject->h().pt();
				break;
			case model::Handle::W:
			case model::Handle::SW:
				x0 += x2 - mResizeObject->w().pt();
				break;
			default:
				break;
			}
		}
		else
		{
			mResizeObject->setSize( model::Distance::pt(w),
			                        model::Distance::pt(h) );
		}

		/*
		 * Put new origin back into world coordinates and set.
		 */
		QPointF p0( x0, y0 );
		p0 = mResizeObject->matrix().map( p0 );
		p0 += QPointF( mResizeObject->x0().pt(), mResizeObject->y0().pt() );
		mResizeObject->setPosition( model::Distance::pt(p0.x()),
		                            model::Distance::pt(p0.y()) );
	}


	///
	/// Key Press Event Handler
	void
	LabelEditor::keyPressEvent( QKeyEvent* event )
	{
		if ( mState == IdleState )
		{
			switch (event->key())
			{

			case Qt::Key_Left:
				mUndoRedoModel->checkpoint( tr("Move") );
				mModel->moveSelection( -mStepSize, model::Distance(0) );
				break;

			case Qt::Key_Up:
				mUndoRedoModel->checkpoint( tr("Move") );
				mModel->moveSelection( model::Distance(0), -mStepSize );
				break;

			case Qt::Key_Right:
				mUndoRedoModel->checkpoint( tr("Move") );
				mModel->moveSelection( mStepSize, model::Distance(0) );
				break;

			case Qt::Key_Down:
				mUndoRedoModel->checkpoint( tr("Move") );
				mModel->moveSelection( model::Distance(0), mStepSize );
				break;

			case Qt::Key_Delete:
				mUndoRedoModel->checkpoint( tr("Delete") );
				mModel->deleteSelection();
				setCursor( Qt::ArrowCursor );
				break;

			default:
				QWidget::keyPressEvent( event );
				break;
			
			}
		}
		else
		{
			QWidget::keyPressEvent( event );
		}
	}


	///
	/// Paint Event Handler
	///
	void
	LabelEditor::paintEvent( QPaintEvent* event )
	{
		if ( mModel )
		{
			QPainter painter( this );

			painter.setRenderHint( QPainter::Antialiasing, true );
			painter.setRenderHint( QPainter::TextAntialiasing, true );
			painter.setRenderHint( QPainter::SmoothPixmapTransform, true );
		
			/* Fill background before any transformations */
			painter.setBrush( QBrush( backgroundColor ) );
			painter.setPen( Qt::NoPen );
			painter.drawRect( rect() );

			/* Transform. */
			painter.scale( mScale, mScale );
			painter.translate( mX0.pt(), mY0.pt() );

			/* Now draw from the bottom layer up. */
			drawBgLayer( &painter );
			drawGridLayer( &painter );
			drawMarkupLayer( &painter );
			drawObjectsLayer( &painter );
			drawFgLayer( &painter );
			drawHighlightLayer( &painter );
			drawSelectRegionLayer( &painter );
		}
	}


	//
	// Handle drag enter event
	//
	void LabelEditor::dragEnterEvent( QDragEnterEvent *event )
	{
		if ( event->mimeData()->hasUrls()   ||
		     event->mimeData()->hasImage()  ||
		     event->mimeData()->hasText() )
		{
			event->acceptProposedAction();
		}
		else
		{
			event->ignore();
		}
	}


	//
	// Handle drag move event
	//
	void LabelEditor::dragMoveEvent( QDragMoveEvent *event )
	{
		if ( event->mimeData()->hasUrls()   ||
		     event->mimeData()->hasImage()  ||
		     event->mimeData()->hasText() )
		{
			event->acceptProposedAction();
		}
		else
		{
			event->ignore();
		}
	}


	//
	// Handle drop event
	//
	void LabelEditor::dropEvent( QDropEvent *event )
	{
		/*
		 * Transform to label coordinates
		 */
		QTransform transform;

		transform.scale( mScale, mScale );
		transform.translate( mX0.pt(), mY0.pt() );

		QPointF pWorld = transform.inverted().map( event->position() );
		auto xWorld = model::Distance::pt( pWorld.x() );
		auto yWorld = model::Distance::pt( pWorld.y() );
		auto p = model::Point( xWorld, yWorld );

		if ( event->mimeData()->hasUrls() )
		{
			mUndoRedoModel->checkpoint( tr("Drop") );
			mModel->pasteAsUrls( event->mimeData(), p );
			event->acceptProposedAction();
		}
		else if ( event->mimeData()->hasImage() )
		{
			mUndoRedoModel->checkpoint( tr("Drop") );
			mModel->pasteAsImage( event->mimeData(), p );
			event->acceptProposedAction();
		}
		else if ( event->mimeData()->hasText() )
		{
			mUndoRedoModel->checkpoint( tr("Drop") );
			mModel->pasteAsText( event->mimeData(), p );
			event->acceptProposedAction();
		}
		else
		{
			event->ignore();
		}
	}


	///
	/// Draw Background Layer
	///
	void
	LabelEditor::drawBgLayer( QPainter* painter )
	{
		/*
		 * Draw shadow
		 */
		painter->save();

		painter->setBrush( QBrush( shadowColor ) );
		painter->setPen( Qt::NoPen );

		painter->translate( shadowOffsetPixels/mScale, shadowOffsetPixels/mScale );

		if ( mModel->rotate() )
		{
			painter->rotate( -90 );
			painter->translate( -mModel->frame()->w().pt(), 0 );
		}
		painter->drawPath( mModel->frame()->path() );

		painter->restore();


		/*
		 * Draw label
		 */
		painter->save();

		painter->setBrush( QBrush( labelColor ) );
		painter->setPen( Qt::NoPen );

		if ( mModel->rotate() )
		{
			painter->rotate( -90 );
			painter->translate( -mModel->frame()->w().pt(), 0 );
		}
		painter->drawPath( mModel->frame()->path() );

		painter->restore();
	}


	///
	/// Draw Grid Layer
	///
	void
	LabelEditor::drawGridLayer( QPainter* painter )
	{
		if ( mGridVisible )
		{
			model::Distance w = mModel->frame()->w();
			model::Distance h = mModel->frame()->h();

			model::Distance x0, y0;
			if ( dynamic_cast<const model::FrameRect*>( mModel->frame() ) )
			{
				x0 = gridSpacing;
				y0 = gridSpacing;
			}
			else
			{
				/* round labels, adjust grid to line up with center of label. */
				x0 = fmod( w/2, gridSpacing );
				y0 = fmod( h/2, gridSpacing );
			}

			painter->save();
			if ( mModel->rotate() )
			{
				painter->rotate( -90 );
				painter->translate( -mModel->frame()->w().pt(), 0 );
			}

			painter->setClipPath( mModel->frame()->path() );

			QPen pen( gridLineColor, gridLineWidthPixels );
			pen.setCosmetic( true );
			painter->setPen( pen );

			for ( model::Distance x = x0; x < w; x += gridSpacing )
			{
				painter->drawLine( x.pt(), 0, x.pt(), h.pt() );
			}

			for ( model::Distance y = y0; y < h; y += gridSpacing )
			{
				painter->drawLine( 0, y.pt(), w.pt(), y.pt() );
			}

			painter->restore();
		}
	}


	///
	/// Draw Markup Layer
	///
	void
	LabelEditor::drawMarkupLayer( QPainter* painter )
	{
		if ( mMarkupVisible )
		{
			painter->save();

			QPen pen( markupLineColor, markupLineWidthPixels );
			pen.setCosmetic( true );
		
			painter->setBrush( Qt::NoBrush );
			painter->setPen( pen );

			if ( mModel->rotate() )
			{
				painter->rotate( -90 );
				painter->translate( -mModel->frame()->w().pt(), 0 );
			}

			foreach( model::Markup* markup, mModel->frame()->markups() )
			{
				painter->drawPath( markup->path( mModel->frame() ) );
			}

			painter->restore();
		}
	}


	///
	/// Draw Objects Layer
	///
	void
	LabelEditor::drawObjectsLayer( QPainter* painter )
	{
		mModel->draw( painter, true, nullptr, nullptr );
	}


	///
	/// Draw Foreground Layer
	///
	void
	LabelEditor::drawFgLayer( QPainter* painter )
	{
		/*
		 * Draw label outline
		 */
		painter->save();

		QPen pen( labelOutlineColor, labelOutlineWidthPixels );
		pen.setCosmetic( true );
		painter->setBrush( QBrush( Qt::NoBrush ) );
		painter->setPen( pen );

		if ( mModel->rotate() )
		{
			painter->rotate( -90 );
			painter->translate( -mModel->frame()->w().pt(), 0 );
		}
		painter->drawPath( mModel->frame()->path() );

		painter->restore();
	}


	///
	/// Draw Highlight Layer
	///
	void
	LabelEditor::drawHighlightLayer( QPainter* painter )
	{
		painter->save();

		foreach ( model::ModelObject* object, mModel->objectList() )
		{
			if ( object->isSelected() )
			{
				object->drawSelectionHighlight( painter, mScale );
			}
		}

		painter->restore();
	}


	///
	/// Draw Select Region Layer
	///
	void
	LabelEditor::drawSelectRegionLayer( QPainter* painter )
	{
		if ( mSelectRegionVisible )
		{
			painter->save();

			QPen pen( selectRegionOutlineColor, selectRegionOutlineWidthPixels );
			pen.setCosmetic( true );
			painter->setBrush( selectRegionFillColor );
			painter->setPen( pen );

			painter->drawRect( mSelectRegion.rect() );
		
			painter->restore();
		}

	}


	///
	/// Settings changed handler
	///
	void LabelEditor::onSettingsChanged()
	{
		model::Units units = model::Settings::units();
	
		mStepSize = model::Distance( units.resolution(), units );
	}


	///
	/// Model size changed handler
	///
	void LabelEditor::onModelSizeChanged()
	{
		if (mZoomToFitFlag)
		{
			double wPixels = mScrollArea->maximumViewportSize().width();
			double hPixels = mScrollArea->maximumViewportSize().height();
	
			double x_scale = ( wPixels - ZOOM_TO_FIT_PAD ) / mModel->w().pt();
			double y_scale = ( hPixels - ZOOM_TO_FIT_PAD ) / mModel->h().pt();
			double newZoom = qMin( x_scale, y_scale ) * model::PTS_PER_INCH / physicalDpiX();

			// Limits
			newZoom = qMin( newZoom, zoomLevels[0] );
			newZoom = qMax( newZoom, zoomLevels[nZoomLevels-1] );

			mZoom = newZoom;
		}

		/* Actual scale depends on DPI of display (assume DpiX == DpiY). */
		mScale = mZoom * physicalDpiX() / model::PTS_PER_INCH;

		setMinimumSize( mScale*mModel->w().pt() + ZOOM_TO_FIT_PAD,
		                mScale*mModel->h().pt() + ZOOM_TO_FIT_PAD );

		/* Adjust origin to center label in widget. */
		mX0 = (width()/mScale - mModel->w()) / 2;
		mY0 = (height()/mScale - mModel->h()) / 2;

		update();

		emit zoomChanged();
	}


} // namespace glabels
