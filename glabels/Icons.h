/*  Icons.h
 *
 *  Copyright (C) 2013  Jaye Evins <evins@snaught.com>
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

#ifndef Icons_h
#define Icons_h


#include <QIcon>


namespace glabels
{

	///
	/// Glabels Icons
	///
	namespace Icons
	{

		class Arrow : public QIcon
		{
		public:
			Arrow()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-arrow.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-arrow.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-arrow.svg" ) );
			}
		};


		class Barcode : public QIcon
		{
		public:
			Barcode()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-barcode.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-barcode.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-barcode.svg" ) );
			}
		};


		class Box : public QIcon
		{
		public:
			Box()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-box.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-box.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-box.svg" ) );
			}
		};


		class Ellipse : public QIcon
		{
		public:
			Ellipse()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-ellipse.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-ellipse.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-ellipse.svg" ) );
			}
		};


		class Image : public QIcon
		{
		public:
			Image()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-image.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-image.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-image.svg" ) );
			}
		};


		class Line : public QIcon
		{
		public:
			Line()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-line.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-line.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-line.svg" ) );
			}
		};


		class Text : public QIcon
		{
		public:
			Text()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-text.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-text.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-text.svg" ) );
			}
		};


		class ObjectProperties : public QIcon
		{
		public:
			ObjectProperties()
			{
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-object-properties.svg" ) );
			}
		};


		class AlignLeft : public QIcon
		{
		public:
			AlignLeft()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-align-left.svg" ) );
			}
		};


		class AlignHCenter : public QIcon
		{
		public:
			AlignHCenter()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-align-hcenter.svg" ) );
			}
		};


		class AlignRight : public QIcon
		{
		public:
			AlignRight()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-align-right.svg" ) );
			}
		};


		class AlignBottom : public QIcon
		{
		public:
			AlignBottom()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-align-bottom.svg" ) );
			}
		};


		class AlignVCenter : public QIcon
		{
		public:
			AlignVCenter()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-align-vcenter.svg" ) );
			}
		};


		class AlignTop : public QIcon
		{
		public:
			AlignTop()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-align-top.svg" ) );
			}
		};


		class CenterHoriz : public QIcon
		{
		public:
			CenterHoriz()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-center-horiz.svg" ) );
			}
		};


		class CenterVert : public QIcon
		{
		public:
			CenterVert()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-center-vert.svg" ) );
			}
		};


		class FlipHoriz : public QIcon
		{
		public:
			FlipHoriz()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-flip-horiz.svg" ) );
			}
		};


		class FlipVert : public QIcon
		{
		public:
			FlipVert()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-flip-vert.svg" ) );
			}
		};


		class RotateLeft : public QIcon
		{
		public:
			RotateLeft()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-rotate-left.svg" ) );
			}
		};


		class RotateRight : public QIcon
		{
		public:
			RotateRight()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-rotate-right.svg" ) );
			}
		};


		class OrderBottom : public QIcon
		{
		public:
			OrderBottom()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-order-bottom.svg" ) );
			}
		};


		class OrderTop : public QIcon
		{
		public:
			OrderTop()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-order-top.svg" ) );
			}
		};


		class AlignTextLeft : public QIcon
		{
		public:
			AlignTextLeft()
			{
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-align-text-left.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-align-text-left.svg" ) );
			}
		};


		class AlignTextCenter : public QIcon
		{
		public:
			AlignTextCenter()
			{
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-align-text-center.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-align-text-center.svg" ) );
			}
		};


		class AlignTextRight : public QIcon
		{
		public:
			AlignTextRight()
			{
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-align-text-right.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-align-text-right.svg" ) );
			}
		};


		class AlignTextBottom : public QIcon
		{
		public:
			AlignTextBottom()
			{
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-valign-text-bottom.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-valign-text-bottom.svg" ) );
			}
		};


		class AlignTextMiddle : public QIcon
		{
		public:
			AlignTextMiddle()
			{
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-valign-text-middle.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-valign-text-middle.svg" ) );
			}
		};


		class AlignTextTop : public QIcon
		{
		public:
			AlignTextTop()
			{
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-valign-text-top.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-valign-text-top.svg" ) );
			}
		};


		class Glabels : public QIcon
		{
		public:
			Glabels()
			{
				addFile( ":icons/apps/16x16/glabels.svg" );
				addFile( ":icons/apps/22x22/glabels.svg" );
				addFile( ":icons/apps/32x32/glabels.svg" );
				addFile( ":icons/apps/48x48/glabels.svg" );
				addFile( ":icons/apps/scalable/glabels.svg" );
			}
		};


		class Edit : public QIcon
		{
		public:
			Edit()
			{
				addPixmap( QPixmap( ":icons/flat/48x48/glabels-edit.svg" ) );
			}
		};


		class EditCopy : public QIcon
		{
		public:
			EditCopy()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-edit-copy.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-edit-copy.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-edit-copy.svg" ) );
			}
		};


		class EditCut : public QIcon
		{
		public:
			EditCut()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-edit-cut.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-edit-cut.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-edit-cut.svg" ) );
			}
		};


		class EditPaste : public QIcon
		{
		public:
			EditPaste()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-edit-paste.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-edit-paste.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-edit-paste.svg" ) );
			}
		};


		class FileNew : public QIcon
		{
		public:
			FileNew()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-file-new.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-file-new.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-file-new.svg" ) );
				addPixmap( QPixmap( ":icons/flat/32x32/glabels-file-new.svg" ) );
			}
		};


		class FileOpen : public QIcon
		{
		public:
			FileOpen()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-file-open.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-file-open.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-file-open.svg" ) );
				addPixmap( QPixmap( ":icons/flat/32x32/glabels-file-open.svg" ) );
			}
		};


		class FileRecent : public QIcon
		{
		public:
			FileRecent()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-file-recent.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-file-recent.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-file-recent.svg" ) );
				addPixmap( QPixmap( ":icons/flat/32x32/glabels-file-recent.svg" ) );
			}
		};


		class FilePrint : public QIcon
		{
		public:
			FilePrint()
			{
				addPixmap( QPixmap( ":icons/flat/32x32/glabels-print.svg" ) );
				addPixmap( QPixmap( ":icons/flat/48x48/glabels-print.svg" ) );
			}
		};


		class FileSave : public QIcon
		{
		public:
			FileSave()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-file-save.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-file-save.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-file-save.svg" ) );
			}
		};


		class FileSaveAs : public QIcon
		{
		public:
			FileSaveAs()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-file-save-as.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-file-save-as.svg" ) );
				addPixmap( QPixmap( ":icons/flat/24x24/glabels-file-save-as.svg" ) );
			}
		};


		class Merge : public QIcon
		{
		public:
			Merge()
			{
				addPixmap( QPixmap( ":icons/flat/48x48/glabels-merge.svg" ) );
			}
		};


		class Properties : public QIcon
		{
		public:
			Properties()
			{
				addPixmap( QPixmap( ":icons/flat/48x48/glabels-properties.svg" ) );
			}
		};


		class Variables : public QIcon
		{
		public:
			Variables()
			{
				addPixmap( QPixmap( ":icons/flat/48x48/glabels-variables.svg" ) );
			}
		};


		class ZoomBestFit : public QIcon
		{
		public:
			ZoomBestFit()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-zoom-to-fit.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-zoom-to-fit.svg" ) );
			}
		};


		class ZoomIn : public QIcon
		{
		public:
			ZoomIn()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-zoom-in.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-zoom-in.svg" ) );
			}
		};


		class ZoomOriginal : public QIcon
		{
		public:
			ZoomOriginal()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-zoom-one-to-one.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-zoom-one-to-one.svg" ) );
			}
		};


		class ZoomOut : public QIcon
		{
		public:
			ZoomOut()
			{
				addPixmap( QPixmap( ":icons/flat/16x16/glabels-zoom-out.svg" ) );
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-zoom-out.svg" ) );
			}
		};


		class ViewGrid : public QIcon
		{
		public:
			ViewGrid()
			{
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-view-grid.svg" ) );
			}
		};


		class ViewList : public QIcon
		{
		public:
			ViewList()
			{
				addPixmap( QPixmap( ":icons/flat/22x22/glabels-view-list.svg" ) );
			}
		};

	}

}


#endif // Icons_h
