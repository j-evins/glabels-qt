//  ColorPaletteDialog.cpp
//
//  Copyright (C) 2014-2016  Jaye Evins <evins@snaught.com>
//
//  This file is part of gLabels-qt.
//
//  gLabels-qt is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  gLabels-qt is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with gLabels-qt.  If not, see <http://www.gnu.org/licenses/>.
//


#include "ColorPaletteDialog.hpp"

#include <QColorDialog>
#include <QDebug>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStandardItemModel>
#include <QVBoxLayout>


namespace glabels
{

        //
        // Static data
        //
        ColorPaletteDialog::ColorTableEntry ColorPaletteDialog::mColorTable[] = {

                { "#ef2929", QT_TRANSLATE_NOOP("Color name", "Light Scarlet Red") },
                { "#fcaf3e", QT_TRANSLATE_NOOP("Color name", "Light Orange")      },
                { "#fce94f", QT_TRANSLATE_NOOP("Color name", "Light Butter")      },
                { "#8ae234", QT_TRANSLATE_NOOP("Color name", "Light Chameleon")   },
                { "#729fcf", QT_TRANSLATE_NOOP("Color name", "Light Sky Blue")    },
                { "#ad7fa8", QT_TRANSLATE_NOOP("Color name", "Light Plum")        },
                { "#e9b96e", QT_TRANSLATE_NOOP("Color name", "Light Chocolate")   },
                { "#888a85", QT_TRANSLATE_NOOP("Color name", "Light Aluminum 1")  },
                { "#eeeeec", QT_TRANSLATE_NOOP("Color name", "Light Aluminum 2")  },

                { "#cc0000", QT_TRANSLATE_NOOP("Color name", "Scarlet Red")       },
                { "#f57900", QT_TRANSLATE_NOOP("Color name", "Orange")            },
                { "#edd400", QT_TRANSLATE_NOOP("Color name", "Butter")            },
                { "#73d216", QT_TRANSLATE_NOOP("Color name", "Chameleon")         },
                { "#3465a4", QT_TRANSLATE_NOOP("Color name", "Sky Blue")          },
                { "#75507b", QT_TRANSLATE_NOOP("Color name", "Plum")              },
                { "#c17d11", QT_TRANSLATE_NOOP("Color name", "Chocolate")         },
                { "#555753", QT_TRANSLATE_NOOP("Color name", "Aluminum 1")        },
                { "#d3d7cf", QT_TRANSLATE_NOOP("Color name", "Aluminum 2")        },

                { "#a40000", QT_TRANSLATE_NOOP("Color name", "Dark Scarlet Red")  },
                { "#ce5c00", QT_TRANSLATE_NOOP("Color name", "Dark Orange")       },
                { "#c4a000", QT_TRANSLATE_NOOP("Color name", "Dark Butter")       },
                { "#4e9a06", QT_TRANSLATE_NOOP("Color name", "Dark Chameleon")    },
                { "#204a87", QT_TRANSLATE_NOOP("Color name", "Dark Sky Blue")     },
                { "#5c3566", QT_TRANSLATE_NOOP("Color name", "Dark Plum")         },
                { "#8f5902", QT_TRANSLATE_NOOP("Color name", "Dark Chocolate")    },
                { "#2e3436", QT_TRANSLATE_NOOP("Color name", "Dark Aluminum 1")   },
                { "#babdb6", QT_TRANSLATE_NOOP("Color name", "Dark Aluminum 2")   },

                { "#000000", QT_TRANSLATE_NOOP("Color name", "Black")             },
                { "#2e3436", QT_TRANSLATE_NOOP("Color name", "Very Dark Gray")    },
                { "#555753", QT_TRANSLATE_NOOP("Color name", "Darker Gray")       },
                { "#888a85", QT_TRANSLATE_NOOP("Color name", "Dark Gray")         },
                { "#babdb6", QT_TRANSLATE_NOOP("Color name", "Medium Gray")       },
                { "#d3d7cf", QT_TRANSLATE_NOOP("Color name", "Light Gray")        },
                { "#eeeeec", QT_TRANSLATE_NOOP("Color name", "Lighter Gray")      },
                { "#f3f3f3", QT_TRANSLATE_NOOP("Color name", "Very Light Gray")   },
                { "#ffffff", QT_TRANSLATE_NOOP("Color name", "White")             }

        };


        ColorPaletteDialog::ColorPaletteDialog( const QString& defaultLabel,
                                                const QColor&  defaultColor,
                                                const QColor&  color,
                                                bool           showUseFieldButton,
                                                QWidget*       parent )
                : QDialog( parent )
        {
                mColorHistory = ColorHistory::instance();
                connect( mColorHistory, SIGNAL(changed()), this, SLOT(onColorHistoryChanged()) );

                mDefaultColor = defaultColor;
                mColorNode = model::ColorNode( color );

                setStyleSheet( ".glabels--ColorPaletteDialog {background: white; border: 1px solid black}" );
                setWindowFlags( Qt::Popup | Qt::FramelessWindowHint );

                auto* vLayout = new QVBoxLayout();
                vLayout->setContentsMargins( 0, 0, 0, 0 );
                vLayout->setSpacing( 0 );

                //
                // Construct Standard Colors Grid
                //
                auto* standardColorsGroup = new QGroupBox( tr("Standard Colors") );
                standardColorsGroup->setAlignment( Qt::AlignHCenter );
                vLayout->addWidget( standardColorsGroup );

                auto* mainPaletteLayout = new QGridLayout();
                mainPaletteLayout->setSpacing( 0 );
                for ( int iRow = 0; iRow < PALETTE_ROWS; iRow++ )
                {
                        for ( int iCol = 0; iCol < PALETTE_COLS; iCol++ )
                        {
                                int i = iRow*PALETTE_COLS + iCol;

                                ColorPaletteItem* item = new ColorPaletteItem( i,
                                                                               QColor( mColorTable[i].colorSpec ),
                                                                               tr(mColorTable[i].trname) );
                                connect( item, SIGNAL(activated(int)),
                                         this, SLOT(onPaletteItemActivated(int)) );

                                mainPaletteLayout->addWidget( item, iRow, iCol );
                        }
                }
                standardColorsGroup->setLayout( mainPaletteLayout );

                //
                // Construct Recent Colors Grid
                //
                auto* recentColorsGroup = new QGroupBox( tr("Recent Colors") );
                recentColorsGroup->setAlignment( Qt::AlignHCenter );
                vLayout->addWidget( recentColorsGroup );

                auto* customPaletteLayout = new QHBoxLayout();
                customPaletteLayout->setSpacing( 0 );
                for ( int iCol = 0; iCol < PALETTE_COLS; iCol++ )
                {
                        mHistoryItem[iCol] = new ColorPaletteItem( iCol, QColor(0,0,0,0), "" );
                        mHistoryItem[iCol]->setEnabled( false );
                        connect( mHistoryItem[iCol], SIGNAL(activated(int)),
                                 this, SLOT(onHistoryItemActivated(int)) );

                        customPaletteLayout->addWidget( mHistoryItem[iCol] );
                }
                recentColorsGroup->setLayout( customPaletteLayout );


                //
                // Construct Default (e.g. "No Fill") Button
                //
                auto* defaultColorButton = new QPushButton( defaultLabel );
                defaultColorButton->setAutoDefault( false );
                defaultColorButton->setDefault( false );
                connect( defaultColorButton, SIGNAL(clicked()), this, SLOT(onDefaultButtonClicked()) );
                vLayout->addWidget( defaultColorButton );

                //
                // Construct Custom Color Button
                //
                auto* customColorButton = new QPushButton( tr("Custom color...") );
                customColorButton->setAutoDefault( false );
                customColorButton->setDefault( false );
                connect( customColorButton, SIGNAL(clicked()), this, SLOT(onCustomColorButtonClicked()) );
                vLayout->addWidget( customColorButton );

                //
                // Construct "Use field" Button
                //
                if ( showUseFieldButton )
                {
                        mFieldButton = new FieldButton();
                        mFieldButton->setText( tr("Use substitution field") );
                        mFieldButton->setAutoDefault( false );
                        mFieldButton->setDefault( false );
                        connect( mFieldButton, SIGNAL(keySelected(QString)), this, SLOT(onKeySelected(QString)) );
                        vLayout->addWidget( mFieldButton );
                }
                else
                {
                        mFieldButton = nullptr;
                }

                setLayout( vLayout );

                loadCustomColorHistory();
        }


        void ColorPaletteDialog::setColorNode( const model::ColorNode& colorNode )
        {
                mColorNode = colorNode;
        }


        void ColorPaletteDialog::setKeys( const merge::Merge*     merge,
                                          const model::Variables& variables )
        {
                if (mFieldButton)
                {
                        mFieldButton->setKeys( merge, variables );
                }
        }


        void ColorPaletteDialog::onPaletteItemActivated( int id )
        {
                model::ColorNode newColorNode;
                newColorNode.setField( false );
                newColorNode.setColor( QColor( mColorTable[id].colorSpec ) );
                newColorNode.setKey( "" );

                if ( newColorNode != mColorNode )
                {
                        mColorNode = newColorNode;

                        mColorHistory->addColor( mColorNode.color(), mColorTable[id].trname );

                        emit colorChanged( mColorNode, false );
                        accept();
                }
        }


        void ColorPaletteDialog::onHistoryItemActivated( int id )
        {
                mColorNode.setField( false );
                mColorNode.setColor( mColorHistory->getColors()[id] );
                mColorNode.setKey( "" );

                emit colorChanged( mColorNode, false );
                accept();
        }


        void ColorPaletteDialog::onDefaultButtonClicked()
        {
                mColorNode.setField( false );
                mColorNode.setColor( mDefaultColor );
                mColorNode.setKey( "" );

                emit colorChanged( mColorNode, true );
                accept();
        }


        void ColorPaletteDialog::onCustomColorButtonClicked()
        {
                QColorDialog dlg( mColorNode.color(), this );
                dlg.setWindowTitle( tr("Custom Color") );

                if ( dlg.exec() )
                {
                        model::ColorNode newColorNode;

                        newColorNode.setField( false );
                        newColorNode.setColor( dlg.currentColor() );
                        newColorNode.setKey( "" );

                        if ( newColorNode != mColorNode )
                        {
                                mColorNode = newColorNode;

                                // TRANSLATORS
                                //:   %1 = color specification in hex. String must not contain a colon (:).
                                mColorHistory->addColor( mColorNode.color(),
                                                         QString(tr("Custom Color %1")).arg(mColorNode.color().name()) );

                                emit colorChanged( mColorNode, false );
                                accept();
                        }
                }
        }


        void ColorPaletteDialog::onColorHistoryChanged()
        {
                loadCustomColorHistory();
        }


        void ColorPaletteDialog::loadCustomColorHistory()
        {
                QStringList nameList = mColorHistory->getNames();
                QList<QColor> colorList = mColorHistory->getColors();

                int id = 0;
                for ( QColor color : colorList )
                {
                        mHistoryItem[id]->setColor( id, color, nameList[id] );
                        mHistoryItem[id]->setEnabled( true );
                        id++;
                }

                while ( id < PALETTE_ROWS )
                {
                        mHistoryItem[id]->setEnabled( false );
                        id++;
                }
        }


        void ColorPaletteDialog::onKeySelected( QString key )
        {
                mColorNode.setField( true );
                mColorNode.setColor( QColor( 0xee, 0xee, 0xec ) );
                mColorNode.setKey( key );

                emit colorChanged( mColorNode, false );
                accept();
        }

} // namespace glabels
