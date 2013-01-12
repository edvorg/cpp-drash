// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "editorwindow.h"
#include "ui_editorwindow.h"

#include <QTreeWidgetItem>
#include <QStringList>
#include <QLabel>
#include <QStatusBar>
#include <QToolBar>
#include <QDebug>

#include "../scene/scene.h"
#include "../scene/figure.h"
#include "../scene/sceneobject.h"
#include "../templates/templatesystem.h"
#include "../greng/camera.h"
#include "editorapp.h"
#include "sceneeditorapp.h"

using namespace drash;

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow),
    mModeActions(parent)
{
    ui->setupUi(this);

    mSceneToolbar = new QToolBar(this);
    mObjectToolBar = new QToolBar(this);
    this->addToolBar(mSceneToolbar);
    this->addToolBar(mObjectToolBar);
    CreateActions();
    mModeActions.setExclusive(true);

    mLabelOfStatusBar = new QLabel("Editor Object");
    this->statusBar()->addWidget(mLabelOfStatusBar);

    this->setWindowTitle("DRASH Editor");

    mLayoutForScene = new QHBoxLayout();
    ui->mSpike->setLayout(mLayoutForScene);

    if (this->InitScene() == false) {
        close();
    }

    this->ui->mTreeObjects->clear();

    mWidgetForScene->hide();
    mCurrentSceneWidget = mWidgetForObjects;

    mObjectApp->SetTreeRefreshHandler([this]()
    {
        this->UpdateTreeObject(ui->mTreeObjects,mObjectApp);
    });

    mSceneToolbar->hide();
    this->startTimer(0);
}

EditorWindow::~EditorWindow()
{
    delete ui;
    mObjectApp->Release();
    delete mObjectApp;
    mSceneApp->Release();
    delete mSceneApp;
}

bool EditorWindow::InitScene()
{
    drash::CSceneParams params;
    params.mGravity.Set( 0.0f, -9.8f );

    mSceneApp = new CSceneEditorApp();
    mObjectApp = new CObjectEditorApp();

    mWidgetForObjects = new SceneWidget(this);
    mWidgetForScene = new SceneWidget(this);
    mWidgetForObjects->SetApp(mObjectApp);
    mWidgetForScene->SetApp(mSceneApp);

    mLayoutForScene->addWidget(mWidgetForObjects);
    mLayoutForScene->addWidget(mWidgetForScene);

    ui->mManageWidget->setCurrentIndex(0);
    return true;
}

void EditorWindow::timerEvent(QTimerEvent *)
{
    mTimer.Tick();

    if (mCurrentSceneWidget->GetApp() != nullptr)
    {
        mCurrentSceneWidget->GetApp()->Step(0);
    }

    mCurrentSceneWidget->updateGL();
}

void EditorWindow::CreateNewObject()
{
    QString str_name("Object");
    str_name +=QString::number(mObjectApp->GetTemplateSystem().GetSceneObjectTemplates().size()+1);
    QTreeWidgetItem *newItem = new QTreeWidgetItem(ui->mTreeObjects,QStringList(str_name));
    newItem->setSelected(true);
    mObjectApp->AddNewObjectToTemplate(str_name.toStdString());
}

void EditorWindow::AddNewFigure()
{
    if (mNewFigureAction->isChecked()){
        mObjectApp->StartBuild();
    }
}

void EditorWindow::MoveActive()
{
    if (mMoveActiveAction->isChecked()) {
        mObjectApp->ActiveMoveMode();
    }
}

void EditorWindow::MoveOfAxisActive()
{
    if (mMoveOfAxisActiveAction->isChecked()) {
        mObjectApp->ActiveMoveOfAxisMode();
    }
}

void EditorWindow::StretchActive()
{
    if (mStretchActiveAction->isChecked()) {
        mObjectApp->ActiveStretchMode();
    }
}

void EditorWindow::ZoomUp()
{
     //mCurrentApp->GetDebugRenderer().GetCamera()->Forward(10);
    //    if (mCurrentApp != nullptr){
//        CVec3f pos;

//        if (mObjectApp->GetCamera()->GetPos().IsTargetSet())
//        {
//            pos = mObjectApp->GetCamera()->GetPos().GetTarget();
//        }
//        else
//        {
//            pos = mObjectApp->GetCamera()->GetPos().Get();
//        }

//        pos.mZ += 10.0f;

//        mObjectApp->GetCamera()->GetPos().SetTarget(pos, 0.3, AnimatorBehavior::Single);
//    }
}

void EditorWindow::ZoomDown()
{
    //mCurrentApp->GetDebugRenderer().GetCamera()->Forward(-10);
//    if (mCurrentApp != nullptr){
//        CVec3f pos;

//        if (mObjectApp->GetCamera()->GetPos().IsTargetSet())
//        {
//            pos = mObjectApp->GetCamera()->GetPos().GetTarget();
//        }
//        else
//        {
//            pos = mObjectApp->GetCamera()->GetPos().Get();
//        }

//        pos.mZ -= 10.0f;

//        mObjectApp->GetCamera()->GetPos().SetTarget(pos, 0.3, AnimatorBehavior::Single);
//    }
}

void EditorWindow::CreateActions()
{
    mQuit = new QAction("Quit",this);
    mQuit->setShortcut(tr("Ctrl+Q"));
    this->addAction(mQuit);
    connect(mQuit,SIGNAL(triggered()),
            this,SLOT(close()));
    QList<QAction*> listActions;

    mNewObjectAction = new QAction("New SceneObject", this);
    mNewObjectAction->setShortcut(tr("Ctrl+N"));
    listActions << mNewObjectAction;
    connect(mNewObjectAction,SIGNAL(triggered()),
            this,SLOT(CreateNewObject()));

    mNewFigureAction = new QAction("Build new Figure", this);
    mNewFigureAction->setShortcut(tr("Ctrl+F"));
    mNewFigureAction->setCheckable(true);
    listActions << mNewFigureAction;
    connect(mNewFigureAction,SIGNAL(changed()),
            this, SLOT(AddNewFigure()));
    mModeActions.addAction(mNewFigureAction);

    mMoveActiveAction = new QAction("Move Figure", this);
    mMoveActiveAction->setCheckable(true);
    mMoveActiveAction->setShortcut(tr("Ctrl+M"));
    listActions << mMoveActiveAction;
    connect(mMoveActiveAction,SIGNAL(changed()),
            this,SLOT(MoveActive()));
    mModeActions.addAction(mMoveActiveAction);

    mMoveOfAxisActiveAction = new QAction("Move Of Axis Figure", this);
    mMoveOfAxisActiveAction->setCheckable(true);
    mMoveOfAxisActiveAction->setShortcut(tr("Ctrl+A"));
    listActions << mMoveOfAxisActiveAction;
    connect(mMoveOfAxisActiveAction,SIGNAL(changed()),
            this,SLOT(MoveOfAxisActive()));
    mModeActions.addAction(mMoveOfAxisActiveAction);

    mStretchActiveAction = new QAction("Stretch active", this);
    mStretchActiveAction->setCheckable(true);
    mStretchActiveAction->setShortcut(tr("Ctrl+W"));
    listActions << mStretchActiveAction;
    connect(mStretchActiveAction, SIGNAL(changed()),
            this,SLOT(StretchActive()));
    mModeActions.addAction(mStretchActiveAction);

    mRemoveAction = new QAction("Remove Object", this);
    mRemoveAction->setShortcut(tr("Ctrl+D"));
    listActions << mRemoveAction;
    connect(mRemoveAction,SIGNAL(triggered()),
            this, SLOT(Remove_Object()));

//    mZoomUpAction = new QAction("+", this);
//    mZoomUpAction->setShortcut(tr("Ctrl+="));
//    listActions << mZoomUpAction;
//    connect(mZoomUpAction,SIGNAL(triggered()),
//            this,SLOT(ZoomUp()));

//    mZoomDownAction = new QAction("-",this);
//    mZoomDownAction->setShortcut(tr("Ctrl+-"));
//    listActions << mZoomDownAction;
//    connect(mZoomDownAction,SIGNAL(triggered()),
//            this,SLOT(ZoomDown()));

    mObjectToolBar->addActions(listActions);


//    mSaveAction = new QAction("Save Object", this);
//    mSaveAction->setShortcut(tr("Ctrl+S"));
//    ui->toolBar->addAction(mSaveAction);
//    connect(mSaveAction, SIGNAL(triggered()),
//            this,SLOT(SaveObject()));


}

void EditorWindow::SaveObject()
{
//    mObjectApp->SaveCurrentObject();

//    UpdateTreeObject();
}

bool EditorWindow::UpdateTreeObject(QTreeWidget *_tree, drash::CApp *_app)
{
    _tree->clear();
    QList<QTreeWidgetItem*> list;
    CTemplateSystem tSys= _app->GetTemplateSystem();
    for (auto item = tSys.GetSceneObjectTemplates().begin();
         item != tSys.GetSceneObjectTemplates().end() ; item++) {
       QTreeWidgetItem *objectItem = new QTreeWidgetItem(_tree,
                                                      QStringList(QString::fromStdString(item->first)));
       _tree->addTopLevelItem(objectItem);

       const CSceneObjectGeometry &geo = *(item->second);
       const std::vector<CFigureParams> &mF = geo.mFigures;
       for (auto fig = mF.begin() ; fig != mF.end() ; fig++) {
           CFigureParams par = *fig;

           QString vecs("");
           vecs.append("[");
           for (unsigned int i = 0 ; i < par.mVertices.size() ; i++) {
               if (i != 0) vecs.append(",");
               vecs.append("(");
               QString x = QString::number(par.mVertices[i].mX);
               vecs.append(x);
               vecs.append(",");
               QString y = QString::number(par.mVertices[i].mY);
               vecs.append(y);
               vecs.append(")");
           }
           vecs.append("]");
           new QTreeWidgetItem(objectItem,QStringList(vecs));
       }
    }
    //mMoveActiveAction->setChecked(true);
    return true;
}

void EditorWindow::AddFigure()
{
    std::string nameTemplate = ui->mTreeObjects->selectedItems().at(0)->text(0).toStdString();

    mObjectApp->BuildFigure(nameTemplate);
}

void EditorWindow::on_mTreeObjects_itemSelectionChanged()
{
    QTreeWidgetItem *item = nullptr;
    if (ui->mTreeObjects->selectedItems().size() != 0) {
        item = ui->mTreeObjects->selectedItems().at(0);
    } else {
        return;
    }
    if (item->parent() == nullptr) {
        qDebug() << "Object created";
        mObjectApp->ShowObject(item->text(0).toStdString());
        //mCurrentObject = mObjectApp->GetTemplateSystem().CreateSceneObjectFromTemplate(item->text(0).toStdString(),params);
    }
}

void EditorWindow::Remove_Object()
{
    QTreeWidgetItem *item = nullptr;
    if (ui->mTreeObjects->selectedItems().size() != 0) {
        item = ui->mTreeObjects->selectedItems().at(0);
    } else {
        return;
    }
    if (item->parent() == nullptr) {
        //qDebug() << "Object created";
        mObjectApp->GetTemplateSystem().RemoveSceneObjectTemplate(item->text(0).toStdString());
        UpdateTreeObject(ui->mTreeObjects,mObjectApp);
        //mObjectApp->ShowObject(item->text(0).toStdString());
        //mCurrentObject = mObjectApp->GetTemplateSystem().CreateSceneObjectFromTemplate(item->text(0).toStdString(),params);
    }
}

void EditorWindow::on_mManageWidget_currentChanged(int index)
{
//    qDebug() << " Changed " << index ;
    if (index == 0) {
        mWidgetForScene->hide();
        mWidgetForObjects->show();
        mCurrentSceneWidget = mWidgetForObjects;
        mSceneToolbar->hide();
        mObjectToolBar->show();
    } else if (index == 1) {
        mWidgetForObjects->hide();
        mWidgetForScene->show();
        mCurrentSceneWidget = mWidgetForScene;
        mObjectApp->GetTemplateSystem().Store();
        mSceneApp->GetTemplateSystem().Load();
        UpdateTreeObject(ui->mTreeTemplates,mSceneApp);
        mObjectToolBar->hide();
        mSceneToolbar->show();
    }
}
