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
#include <QFileDialog>
#include <QMessageBox>

#include "../scene/scene.h"
#include "../scene/figure.h"
#include "../scene/sceneobject.h"
#include "../templates/templatesystem.h"
#include "../greng/camera.h"
#include "editorapp.h"
#include "sceneeditorapp.h"
#include "../levelmanager/level.h"

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
    connect(&mModeActions,SIGNAL(selected(QAction*)),
            this,SLOT(ChangeMode(QAction*)));

    mLabelOfStatusBar = new QLabel("Editor Object");
    this->statusBar()->addWidget(mLabelOfStatusBar);

    this->setWindowTitle("DRASH Editor");

    mLayoutForScene = new QHBoxLayout();
    ui->mSpike->setLayout(mLayoutForScene);

    if (this->InitScene() == false) {
        close();
    }

    mCurrentSceneWidget = mWidgetForObjects;

    this->ui->mTreeObjects->clear();

    mObjectApp->SetTreeRefreshHandler([this]()
    {
        this->UpdateTreeTemplates(ui->mTreeObjects,mObjectApp);
        mMoveActiveAction->setChecked(true);
    });

    mSceneApp->SetTreeRefreshHandler([this]()
    {
        this->UpdateTreeSceneObjects();
    });

    mSceneToolbar->hide();
    mTimer.Reset(true);

    ui->mTreeTemplates->setDragEnabled(true);
//    emit mMoveActiveAction->trigger();
//    ui->mTreeTemplates->setDragDropMode(QAbstractItemView::InternalMove);
    mWidgetForScene->hide();
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
    mWidgetForScene->SetApp(mSceneApp);
    mWidgetForObjects->SetApp(mObjectApp);

    mLayoutForScene->addWidget(mWidgetForScene);
    mLayoutForScene->addWidget(mWidgetForObjects);
    mWidgetForScene->show();
    ui->mManageWidget->setCurrentIndex(0);
    if (mWidgetForObjects->GetApp() == nullptr ||
        mWidgetForScene->GetApp() == nullptr) {
        return false;
    }

    return true;
}

void EditorWindow::timerEvent(QTimerEvent *)
{
//    if (fuck == false) {
//        mWidgetForScene->hide();
//        fuck = true;
//    }
    mTimer.Tick();

    if (mCurrentSceneWidget->GetApp() != nullptr)
    {
        mCurrentSceneWidget->GetApp()->Step(mTimer.GetDeltaTime());
    } else {
        close();
    }

    mCurrentSceneWidget->updateGL();
}

void EditorWindow::CreateNewObject()
{
    QString str_name;
    int i = 0;
    do {
        str_name = "Object";
        str_name += QString::number(mObjectApp->GetTemplateSystem().GetSceneObjectTemplates().size()+1+i);
        i++;
    } while (mObjectApp->AddNewObjectToTemplate(str_name.toStdString()) == false);

    str_name += QString::number(mObjectApp->GetTemplateSystem().GetSceneObjectTemplates().size()+1);
    QTreeWidgetItem *newItem = new QTreeWidgetItem(ui->mTreeObjects,QStringList(str_name));
    ui->mTreeObjects->addTopLevelItem(newItem);
    newItem->setSelected(true);
    UpdateTreeTemplates(ui->mTreeObjects, mObjectApp);
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

void EditorWindow::ChangeMode(QAction *_action)
{
    mLabelOfStatusBar->setText(_action->text());
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

void EditorWindow::OpenLevel()
{
//    qDebug() << "Open file";
    QString name = QFileDialog::getOpenFileName(this,"Open level file");
    if (mSceneApp->LoadLevel(name.toStdString()) == true) {
        mLabelOfStatusBar->setText("Level load from " + name);
    } else {
        QMessageBox::critical(this,"Error", "Level not load. See log file");
    }
}

void EditorWindow::SaveLevel()
{
//    qDebug() << " Save file";
    if (mSceneApp->IsSetLevel() == false) {
        QMessageBox::critical(this,"Error", "Level is not set now!!");
        return;
    }
    if (mSceneApp->IsSetFileName() == false) {
        SaveLevelAs();
    } else {
        if (mSceneApp->SaveLevel() == false) {
            QMessageBox::critical(this,"Error", "Level is not save. See log file");
        } else {
            mLabelOfStatusBar->setText("Level saved in " +
                                       QString::fromStdString(mSceneApp->GetLevelFileName()));
        }
    }
}

void EditorWindow::SaveLevelAs()
{
    if (mSceneApp->IsSetLevel() == false) {
        QMessageBox::critical(this,"Error", "Level is not set now!!");
        return;
    }
    QString name = QFileDialog::getSaveFileName(this,"Choose file for saving");
    if (mSceneApp->SaveLevelAs(name.toStdString()) == false) {
        QMessageBox::critical(this,"Error", "Level is not save. See log file");
    } else {
        mLabelOfStatusBar->setText("Level saved in " + name);
    }
}

void EditorWindow::PlayLevel()
{
    mSceneApp->StartCurrentLevel();
}

void EditorWindow::PauseLevel()
{
    mSceneApp->PauseLevel();
}

void EditorWindow::StopLevel()
{
    mSceneApp->StopLevel();
}

void EditorWindow::NewLevel()
{
    if (mSceneApp->NewLevel() == true) {
        mLabelOfStatusBar->setText("Created new level");
    }
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

    // Actions for editor scene
    listActions.clear();

    mOpenLevelAction = new QAction("Open Level", this);
    listActions << mOpenLevelAction;
    connect(mOpenLevelAction,SIGNAL(triggered()),
            this, SLOT(OpenLevel()));

    mSaveLevelAction = new QAction("Save Level", this);
    listActions << mSaveLevelAction;
    connect(mSaveLevelAction,SIGNAL(triggered()),
            this,SLOT(SaveLevel()));

    mSaveLevelAsAction = new QAction("Save As Level", this);
    listActions << mSaveLevelAsAction;
    connect(mSaveLevelAsAction, SIGNAL(triggered()) ,
            this,SLOT(SaveLevelAs()));

    mPlayLevelAction = new QAction("Start Level", this);
    listActions << mPlayLevelAction;
    connect(mPlayLevelAction, SIGNAL(triggered()),
            this,SLOT(PlayLevel()));

    mPauseLevelAction = new QAction("Pause Level", this);
    listActions << mPauseLevelAction;
    connect(mPauseLevelAction, SIGNAL(triggered()) ,
            this, SLOT(PauseLevel()));

    mStopLevelAction = new QAction("Stop Level", this);
    listActions << mStopLevelAction;
    connect(mStopLevelAction,SIGNAL(triggered()),
            this,SLOT(StopLevel()));

    mNewLevelAction = new QAction("New Level", this);
    listActions.push_front(mNewLevelAction);
    connect(mNewLevelAction, SIGNAL(triggered()),
            this, SLOT(NewLevel()));

    mSceneToolbar->addActions(listActions);
//    mSaveAction = new QAction("Save Object", this);
//    mSaveAction->setShortcut(tr("Ctrl+S"));
//    ui->toolBar->addAction(mSaveAction);
//    connect(mSaveAction, SIGNAL(triggered()),
//            this,SLOT(SaveObject()));

}


bool EditorWindow::UpdateTreeTemplates(QTreeWidget *_tree, drash::CApp *_app)
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
    return true;
}

void EditorWindow::UpdateTreeSceneObjects()
{
    ui->mTreeSceneObjects->clear();
    QTreeWidget * tree = ui->mTreeSceneObjects;
    for (const auto & headitem : mSceneApp->GetCurrentLevel()->GetObjects()) {
        QTreeWidgetItem *templateItem = new QTreeWidgetItem(tree,
                                        QStringList(QString::fromStdString(headitem.first)));

        tree->addTopLevelItem(templateItem);
        for (const auto &item : headitem.second) {
            QString objectname = QString::fromStdString(item.first);
            new QTreeWidgetItem(templateItem,QStringList(objectname));
        }
    }
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
        UpdateTreeTemplates(ui->mTreeObjects,mObjectApp);
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
        UpdateTreeTemplates(ui->mTreeTemplates,mSceneApp);
        mObjectToolBar->hide();
        mSceneToolbar->show();
        mSceneApp->ResetLevel();
    }
}

void EditorWindow::on_mTreeTemplates_doubleClicked(const QModelIndex &index)
{
    std::string nameobject = ui->mTreeTemplates->currentItem()->text(0).toStdString();
    mSceneApp->AddObject(nameobject);
}

void EditorWindow::on_mTreeSceneObjects_clicked(const QModelIndex &index)
{
    QTreeWidgetItem * item = ui->mTreeSceneObjects->selectedItems().at(0);
    if (item->parent() != nullptr) {
        mSceneApp->LookObject(item->parent()->text(0).toStdString(),
                              item->text(0).toStdString());
    }
}
