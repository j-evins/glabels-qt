/*  MainWindow.h
 *
 *  Copyright (C) 2014  Jaye Evins <evins@snaught.com>
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

#ifndef MainWindow_h
#define MainWindow_h

#include <model/Model.h>

#include <QAction>
#include <QCloseEvent>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QScrollArea>
#include <QStackedWidget>
#include <QToolBar>
#include <QToolButton>


namespace glabels
{

	// Forward References
	class LabelEditor;
	class MergeView;
	class ObjectEditor;
	class PrintView;
	class PropertiesView;
	class StartupView;
	class UndoRedoModel;
	class VariablesView;


	///
	/// MainWindow Widget
	///
	class MainWindow : public QMainWindow
	{
		Q_OBJECT


		/////////////////////////////////////
		// Lifecycle
		/////////////////////////////////////
	public:
		MainWindow();
		~MainWindow() override;


		/////////////////////////////////////
		// Public Methods
		/////////////////////////////////////
	public:
		model::Model* model() const;
		void setModel( model::Model* model );
		bool isEmpty() const;


		/////////////////////////////////////
		// Events
		/////////////////////////////////////
	protected:
		void closeEvent( QCloseEvent *event ) override;


		/////////////////////////////////////
		// Slots
		/////////////////////////////////////
	private slots:
		void changePage( bool checked );

		void clipboardChanged();

		void fileNew();
		void fileOpen();
		void fileOpenRecent();
		void fileSave();
		void fileSaveAs();
		void fileShowEditorPage();
		void fileShowPropertiesPage();
		void fileShowMergePage();
		void fileShowVariablesPage();
		void fileShowPrintPage();
		void fileTemplateDesigner();
		void fileClose();
		void fileExit();

		void editUndo();
		void editRedo();
		void editCut();
		void editCopy();
		void editPaste();
		void editDelete();
		void editSelectAll();
		void editUnSelectAll();
		void editPreferences();

		void viewFileToolBar( bool );
		void viewEditorToolBar( bool );
		void viewGrid( bool );
		void viewMarkup( bool );
		void viewZoomIn();
		void viewZoomOut();
		void viewZoom1To1();
		void viewZoomToFit();

		void objectsArrowMode();
		void objectsCreateText();
		void objectsCreateBox();
		void objectsCreateLine();
		void objectsCreateEllipse();
		void objectsCreateImage();
		void objectsCreateBarcode();
		void objectsOrderRaise();
		void objectsOrderLower();
		void objectsXformRotateLeft();
		void objectsXformRotateRight();
		void objectsXformFlipHoriz();
		void objectsXformFlipVert();
		void objectsAlignLeft();
		void objectsAlignHCenter();
		void objectsAlignRight();
		void objectsAlignTop();
		void objectsAlignVCenter();
		void objectsAlignBottom();
		void objectsCenter();
		void objectsCenterHoriz();
		void objectsCenterVert();

		void helpContents();
		void helpReportBug();
		void helpAbout();

		void onContextMenuActivate();

		void onZoomChanged();
		void onPointerMoved( double, double );
		void onPointerExit();

		void onNameChanged();
		void onModifiedChanged();
		void onSelectionChanged();
		void onLabelChanged();
		void onUndoRedoChanged();

		void onSettingsChanged();


		/////////////////////////////////////
		// Internal Private Methods
		/////////////////////////////////////
	private:
		void createActions();
		void createMenus();
		void createToolBars();
		void createStatusBar();

		QWidget* createWelcomePage();
		QWidget* createEditorPage();
		QWidget* createPropertiesPage();
		QWidget* createMergePage();
		QWidget* createVariablesPage();
		QWidget* createPrintPage();

		void manageActions();

		void setTitle();

		void readSettings();
		void writeSettings();

		bool isOkToClose();


		/////////////////////////////////////
		// Private Data
		/////////////////////////////////////
	private:
		QMenu*    fileMenu;
		QMenu*    fileRecentMenu;
		QMenu*    editMenu;
		QMenu*    viewMenu;
		QMenu*    viewToolBarsMenu;
		QMenu*    objectsMenu;
		QMenu*    objectsCreateMenu;
		QMenu*    objectsOrderMenu;
		QMenu*    objectsXformMenu;
		QMenu*    objectsAlignMenu;
		QMenu*    objectsCenterMenu;
		QMenu*    helpMenu;

		QMenu*    contextMenu;
		QMenu*    contextOrderMenu;
		QMenu*    contextXformMenu;
		QMenu*    contextAlignMenu;
		QMenu*    contextCenterMenu;
		QMenu*    noSelectionContextMenu;
		
		QToolBar* fileToolBar;
		QToolBar* editorToolBar;

		model::Model*        mModel;
		UndoRedoModel*       mUndoRedoModel;

		QToolBar*            mContents;
		QToolButton*         mWelcomeButton;
		QToolButton*         mEditorButton;
		QToolButton*         mPropertiesButton;
		QToolButton*         mMergeButton;
		QToolButton*         mVariablesButton;
		QToolButton*         mPrintButton;

		QAction*             mWelcomeAction;
		QAction*             mEditorAction;
		QAction*             mPropertiesAction;
		QAction*             mMergeAction;
		QAction*             mVariablesAction;
		QAction*             mPrintAction;

		QStackedWidget*      mPages;
		StartupView*         mWelcomeView;
		QScrollArea*         mLabelEditorScrollArea;
		LabelEditor*         mLabelEditor;
		ObjectEditor*        mObjectEditor;
		PropertiesView*      mPropertiesView;
		MergeView*           mMergeView;
		VariablesView*       mVariablesView;
		PrintView*           mPrintView;

		QLabel*   zoomInfoLabel;
		QLabel*   cursorInfoLabel;

		QAction*  fileNewAction;
		QAction*  fileOpenAction;
		QAction*  fileSaveAction;
		QAction*  fileSaveAsAction;
		QAction*  fileShowEditorPageAction;
		QAction*  fileShowPropertiesPageAction;
		QAction*  fileShowMergePageAction;
		QAction*  fileShowVariablesPageAction;
		QAction*  fileShowPrintPageAction;
		QAction*  fileTemplateDesignerAction;
		QAction*  fileCloseAction;
		QAction*  fileExitAction;

		QList<QAction*>  fileRecentActionList;

		QAction*  editUndoAction;
		QAction*  editRedoAction;
		QAction*  editCutAction;
		QAction*  editCopyAction;
		QAction*  editPasteAction;
		QAction*  editDeleteAction;
		QAction*  editSelectAllAction;
		QAction*  editUnSelectAllAction;
		QAction*  editPreferencesAction;

		QAction*  viewFileToolBarAction;
		QAction*  viewEditorToolBarAction;
		QAction*  viewGridAction;
		QAction*  viewMarkupAction;
		QAction*  viewZoomInAction;
		QAction*  viewZoomOutAction;
		QAction*  viewZoom1To1Action;
		QAction*  viewZoomToFitAction;

		QAction*  objectsArrowModeAction;
		QAction*  objectsCreateTextAction;
		QAction*  objectsCreateBoxAction;
		QAction*  objectsCreateLineAction;
		QAction*  objectsCreateEllipseAction;
		QAction*  objectsCreateImageAction;
		QAction*  objectsCreateBarcodeAction;
		QAction*  objectsOrderRaiseAction;
		QAction*  objectsOrderLowerAction;
		QAction*  objectsXformRotateLeftAction;
		QAction*  objectsXformRotateRightAction;
		QAction*  objectsXformFlipHorizAction;
		QAction*  objectsXformFlipVertAction;
		QAction*  objectsAlignLeftAction;
		QAction*  objectsAlignHCenterAction;
		QAction*  objectsAlignRightAction;
		QAction*  objectsAlignTopAction;
		QAction*  objectsAlignVCenterAction;
		QAction*  objectsAlignBottomAction;
		QAction*  objectsCenterAction;
		QAction*  objectsCenterHorizAction;
		QAction*  objectsCenterVertAction;

		QAction*  helpContentsAction;
		QAction*  helpReportBugAction;
		QAction*  helpAboutAction;

		QAction*  contextCutAction;
		QAction*  contextCopyAction;
		QAction*  contextPasteAction;
		QAction*  contextDeleteAction;
	};


}


#endif // MainWindow_h
