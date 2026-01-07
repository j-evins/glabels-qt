//  TemplateDesigner.hpp
//
//  Copyright (C) 2018  Jaye Evins <evins@snaught.com>
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

#ifndef TemplateDesigner_hpp
#define TemplateDesigner_hpp


#include "ui_TemplateDesignerIntroPage.h"
#include "ui_TemplateDesignerNamePage.h"
#include "ui_TemplateDesignerPageSizePage.h"
#include "ui_TemplateDesignerShapePage.h"
#include "ui_TemplateDesignerRectPage.h"
#include "ui_TemplateDesignerRoundPage.h"
#include "ui_TemplateDesignerEllipsePage.h"
#include "ui_TemplateDesignerCdPage.h"
#include "ui_TemplateDesignerPathPage.h"
#include "ui_TemplateDesignerContinuousPage.h"
#include "ui_TemplateDesignerNLayoutsPage.h"
#include "ui_TemplateDesignerOneLayoutPage.h"
#include "ui_TemplateDesignerTwoLayoutPage.h"
#include "ui_TemplateDesignerApplyPage.h"

#include "model/Template.hpp"

#include <QWizard>
#include <QWizardPage>


namespace glabels
{
        ///
        /// About Dialog Widget
        ///
        class TemplateDesigner : public QWizard
        {
                Q_OBJECT

                // My subpages are my friends :-)
                friend class TemplateDesignerIntroPage;
                friend class TemplateDesignerNamePage;
                friend class TemplateDesignerPageSizePage;
                friend class TemplateDesignerShapePage;
                friend class TemplateDesignerRectPage;
                friend class TemplateDesignerRoundPage;
                friend class TemplateDesignerEllipsePage;
                friend class TemplateDesignerCdPage;
                friend class TemplateDesignerPathPage;
                friend class TemplateDesignerContinuousPage;
                friend class TemplateDesignerNLayoutsPage;
                friend class TemplateDesignerOneLayoutPage;
                friend class TemplateDesignerTwoLayoutPage;
                friend class TemplateDesignerApplyPage;


                /////////////////////////////////
                // Life Cycle
                /////////////////////////////////
        public:
                TemplateDesigner( QWidget *parent = nullptr );


                /////////////////////////////////
                // Private methods
                /////////////////////////////////
        private:
                int nextId() const override;

                double itemWidth();
                double itemHeight();
                double itemXWaste();
                double itemYWaste();
                model::Template buildTemplate();
                void addLayouts( model::Frame& frame );
                void printTestSheet();
                void loadFromTemplate( const model::Template& tmplate );
                bool isBasedOnCopy();


                /////////////////////////////////
                // Private methods
                /////////////////////////////////
        private:
                bool mIsBasedOnCopy{false};
                bool mIsTemplatePathBased{false};
                bool mIsTemplateContinuousBased{false};
        };


        //
        // Intro Page
        //
        class TemplateDesignerIntroPage : public QWizardPage, public Ui::TemplateDesignerIntroPage
        {
                Q_OBJECT

        public:
                TemplateDesignerIntroPage( QWidget* parent = nullptr );

                bool isComplete() const override;

        private slots:
                void onCopyButtonClicked();
                void onNewButtonClicked();
        };


        //
        // Name Page
        //
        class TemplateDesignerNamePage : public QWizardPage, public Ui::TemplateDesignerNamePage
        {
                Q_OBJECT
        public:
                TemplateDesignerNamePage( QWidget* parent = nullptr );

                bool isComplete() const override;

        private slots:
                void onChanged();

        private:
                bool mCanContinue = false;
        };


        //
        // Page Size Page
        //
        class TemplateDesignerPageSizePage : public QWizardPage, public Ui::TemplateDesignerPageSizePage
        {
                Q_OBJECT
        public:
                TemplateDesignerPageSizePage( QWidget* parent = nullptr );

                void initializePage() override;
                void cleanupPage() override;

        private slots:
                void onComboChanged();
        };


        //
        // Shape Page
        //
        class TemplateDesignerShapePage : public QWizardPage, public Ui::TemplateDesignerShapePage
        {
                Q_OBJECT
        public:
                TemplateDesignerShapePage( QWidget* parent = nullptr );

                void initializePage() override;
                void cleanupPage() override;
        };


        //
        // Rect Page
        //
        class TemplateDesignerRectPage : public QWizardPage, public Ui::TemplateDesignerRectPage
        {
                Q_OBJECT
        public:
                TemplateDesignerRectPage( QWidget* parent = nullptr );

                void initializePage() override;
                void cleanupPage() override;
        };


        //
        // Round Page
        //
        class TemplateDesignerRoundPage : public QWizardPage, public Ui::TemplateDesignerRoundPage
        {
                Q_OBJECT
        public:
                TemplateDesignerRoundPage( QWidget* parent = nullptr );

                void initializePage() override;
                void cleanupPage() override;
        };


        //
        // Ellipse Page
        //
        class TemplateDesignerEllipsePage : public QWizardPage, public Ui::TemplateDesignerEllipsePage
        {
                Q_OBJECT
        public:
                TemplateDesignerEllipsePage( QWidget* parent = nullptr );

                void initializePage() override;
                void cleanupPage() override;
        };


        //
        // Cd Page
        //
        class TemplateDesignerCdPage : public QWizardPage, public Ui::TemplateDesignerCdPage
        {
                Q_OBJECT
        public:
                TemplateDesignerCdPage( QWidget* parent = nullptr );

                void initializePage() override;
                void cleanupPage() override;
        };


        //
        // Path Page
        //
        class TemplateDesignerPathPage : public QWizardPage, public Ui::TemplateDesignerPathPage
        {
                Q_OBJECT
        public:
                TemplateDesignerPathPage( QWidget* parent = nullptr );

                bool isComplete() const override;
        };


        //
        // Continuous Page
        //
        class TemplateDesignerContinuousPage : public QWizardPage, public Ui::TemplateDesignerContinuousPage
        {
                Q_OBJECT
        public:
                TemplateDesignerContinuousPage( QWidget* parent = nullptr );

                bool isComplete() const override;
        };


        //
        // NLayouts Page
        //
        class TemplateDesignerNLayoutsPage : public QWizardPage, public Ui::TemplateDesignerNLayoutsPage
        {
                Q_OBJECT
        public:
                TemplateDesignerNLayoutsPage( QWidget* parent = nullptr );

                void initializePage() override;
                void cleanupPage() override;
        };


        //
        // OneLayout Page
        //
        class TemplateDesignerOneLayoutPage : public QWizardPage, public Ui::TemplateDesignerOneLayoutPage
        {
                Q_OBJECT
        public:
                TemplateDesignerOneLayoutPage( QWidget* parent = nullptr );

                void initializePage() override;
                void cleanupPage() override;

        private slots:
                void onChanged();
                void onPrintButtonClicked();
        };


        //
        // TwoLayout Page
        //
        class TemplateDesignerTwoLayoutPage : public QWizardPage, public Ui::TemplateDesignerTwoLayoutPage
        {
                Q_OBJECT
        public:
                TemplateDesignerTwoLayoutPage( QWidget* parent = nullptr );

                void initializePage() override;
                void cleanupPage() override;

        private slots:
                void onChanged();
                void onPrintButtonClicked();
        };


        //
        // Apply Page
        //
        class TemplateDesignerApplyPage : public QWizardPage, public Ui::TemplateDesignerApplyPage
        {
                Q_OBJECT
        public:
                TemplateDesignerApplyPage( QWidget* parent = nullptr );

                bool validatePage();
        };


}


#endif // TemplateDesigner_hpp
