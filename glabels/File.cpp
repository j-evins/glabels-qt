/*  File.cpp
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

#include "File.h"

#include "MainWindow.h"
#include "SelectProductDialog.h"
#include "TemplateDesigner.h"

#include "model/FileUtil.h"
#include "model/Model.h"
#include "model/Settings.h"
#include "model/XmlLabelParser.h"
#include "model/XmlLabelCreator.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QtDebug>


namespace glabels
{

	//
	// Static data
	//
	QString File::mCwd = ".";


	///
	/// New Label Dialog
	///
	bool File::newLabel( MainWindow *window )
	{
		SelectProductDialog dialog;
		dialog.exec();

		auto tmplate = dialog.tmplate();
		if ( !tmplate.isNull() )
		{
			auto* model = new model::Model();
			model->setTmplate( tmplate );
			
			// Intelligently decide to rotate label by default
			auto frame = tmplate.frame();
			model->setRotate( frame->h() > frame->w() );

			model->clearModified();

			// Either apply to current window or open a new one
			if ( window->isEmpty() )
			{
				window->setModel( model );
			}
			else
			{
				auto *newWindow = new MainWindow();
				newWindow->setModel( model );
				newWindow->show();
			}

			return true;
		}
		else
		{
			return false;
		}
	}


	///
	/// Open File Dialog
	///
	void File::open( MainWindow *window )
	{
		// Either use the saved CWD from a previous open/save or grab it from the path of the current file
		QString cwd = mCwd;
		if ( window->model() && !window->model()->fileName().isEmpty() )
		{
			QFileInfo fileInfo( window->model()->fileName() );
			if ( fileInfo.isFile() )
			{
				cwd = fileInfo.absolutePath();
			}
		}
	
		QString fileName =
			QFileDialog::getOpenFileName( window,
			                              tr("gLabels - Open Project"),
			                              cwd,
			                              tr("glabels files (*.glabels);;All files (*)")
				);
		if ( !fileName.isEmpty() )
		{
			model::Model *model = model::XmlLabelParser::readFile( fileName );
			if ( model )
			{
				// Either apply to current window or open a new one
				if ( window->isEmpty() )
				{
					window->setModel( model );
				}
				else
				{
					auto *newWindow = new MainWindow();
					newWindow->setModel( model );
					newWindow->show();
				}
				model::Settings::addToRecentFileList( model->fileName() );

				// Save CWD
				mCwd = QFileInfo( fileName ).absolutePath();
			}
			else
			{
				model::Settings::removeFromRecentFileList( fileName );

				QMessageBox msgBox;
				msgBox.setText( tr("Unable to open \"") + fileName + tr("\".") );
				msgBox.setStandardButtons( QMessageBox::Ok );
				msgBox.setDefaultButton( QMessageBox::Ok );
				msgBox.exec();
			}
		}
	}


	///
	/// Open file
	///
	void File::open( const QString& fileName, MainWindow *window )
	{
		if ( !fileName.isEmpty() )
		{
			model::Model *model = model::XmlLabelParser::readFile( fileName );
			if ( model )
			{
				// Either apply to current window or open a new one
				if ( window->isEmpty() )
				{
					window->setModel( model );
				}
				else
				{
					auto *newWindow = new MainWindow();
					newWindow->setModel( model );
					newWindow->show();
				}
				model::Settings::addToRecentFileList( model->fileName() );

				// Save CWD
				mCwd = QFileInfo( fileName ).absolutePath();
			}
			else
			{
				model::Settings::removeFromRecentFileList( fileName );

				QMessageBox msgBox;
				msgBox.setText( tr("Unable to open \"") + fileName + tr("\".") );
				msgBox.setStandardButtons( QMessageBox::Ok );
				msgBox.setDefaultButton( QMessageBox::Ok );
				msgBox.exec();
			}
		}
	}


	///
	/// Save file
	///
	bool File::save( MainWindow *window )
	{
		if ( window->model()->fileName().isEmpty() )
		{
			return saveAs( window );
		}

		if ( !window->model()->isModified() )
		{
			return true;
		}

		model::XmlLabelCreator::writeFile( window->model(), window->model()->fileName() );
		model::Settings::addToRecentFileList( window->model()->fileName() );

		// Save CWD
		mCwd = QFileInfo( window->model()->fileName() ).absolutePath();

		return true;
	}


	///
	/// Save file as
	///
	bool File::saveAs( MainWindow *window )
	{
		// Either use the saved CWD from a previous open/save or grab it from the path
		// of the current file.
		QString cwd = mCwd;
		if ( window->model() && !window->model()->fileName().isEmpty() )
		{
			QFileInfo fileInfo( window->model()->fileName() );
			if ( fileInfo.isFile() )
			{
				cwd = fileInfo.filePath();
			}
		}
	
		QString rawFileName =
			QFileDialog::getSaveFileName( window,
			                              tr("gLabels - Save Project As"),
			                              cwd,
			                              tr("glabels files (*.glabels);;All files (*)"),
			                              nullptr,
			                              QFileDialog::DontConfirmOverwrite	);
		if ( !rawFileName.isEmpty() )
		{
			QString fileName = model::FileUtil::addExtension( rawFileName, ".glabels" );
			
			
			if ( QFileInfo::exists(fileName) )
			{
				QMessageBox msgBox( window );
				msgBox.setWindowTitle( tr("Save Label As") );
				msgBox.setIcon( QMessageBox::Warning );
				msgBox.setText( tr("%1 already exists.").arg(fileName) );
				msgBox.setInformativeText( tr("Do you want to replace it?") );
				msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
				msgBox.setDefaultButton( QMessageBox::No );

				if ( msgBox.exec() == QMessageBox::No )
				{
					return saveAs( window );
				}
			}
			
			model::XmlLabelCreator::writeFile( window->model(), fileName );
			model::Settings::addToRecentFileList( window->model()->fileName() );
		
			// Save CWD
			mCwd = QFileInfo( fileName ).absolutePath();

			return true;
		}

		return false;
	}


	///
	/// Template Designer
	///
	void File::templateDesigner( MainWindow *window )
	{
		TemplateDesigner dialog( window );
		dialog.exec();
	}


	///
	/// Close file
	///
	void File::close( MainWindow *window )
	{
		window->close();
	}


	///
	/// Exit, closing all windows
	///
	void File::exit()
	{
		foreach ( QWidget* qwidget, QApplication::topLevelWidgets() )
		{
			if ( auto* window = qobject_cast<MainWindow*>(qwidget) )
			{
				window->close();
			}
		}
	}

} // namespace glabels
