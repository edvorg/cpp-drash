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
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with drash Source Code. If not, see <http://www.gnu.org/licenses/>.

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
#include "../scene/geometrymanager.h"
#include "../greng/camera.h"
#include "editorapp.h"
#include "sceneeditorapp.h"
#include "../levelmanager/leveldesc.h"

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
//    ui->mTreeTemplates->setDragDropMode();
//    emit mMoveActiveAction->trigger();
//    ui->mTreeTemplates->setDragDropMode(QAbstractItemView::InternalMove);
    mWidgetForScene->hide();
    mSceneApp->SetGetSelectedHandler([this](){
        QTreeWidgetItem * item = ui->mTreeTemplates->currentItem();
        if (item->parent() == NULL) {
            return item->text(0).toStdString();
        } else {
            return std::string("");
        }
    });

    mObjectApp->SetGetSelectedHandler([this](){
        QTreeWidgetItem * item = ui->mTreeObjects->currentItem();
        if (item->parent() == NULL) {
            return item->text(0).toStdString();
        } else {
            return std::string("");
        }
    });
    ui->mParamsPanel->hide();

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
    mTimer.Tick();

    if (mCurrentSceneWidget->GetApp() != nullptr)
    {
        mCurrentSceneWidget->GetApp()->Step(mTimer.GetDeltaTime());
    } else {
        close();
    }

    if (mCurrentSceneWidget == mWidgetForScene) {
        if (mSceneApp->IsObjectSelected() == true) {
            ui->mParamsPanel->setEnabled(true);
            if ( mSceneApp->IsChangedParams() == true ) {
                SetObjectParams(mSceneApp->GetSelectedParams());
                //ui->mAngleBox->setValue((double)mSceneApp->GetAngleParams());
            }
        } else {
            ui->mParamsPanel->setEnabled(false);
        }
    }

    mCurrentSceneWidget->updateGL();
}

void EditorWindow::CreateNewObject()
{
    QString str_name;
    int i = 0;
    do {
        str_name = "Object";
        str_name += QString::number(mObjectApp->GetGeometryManager().GetGeometries().size()+1+i);
        i++;
    } while (mObjectApp->AddNewObjectToTemplate(str_name.toStdString()) == false);

    str_name += QString::number(mObjectApp->GetGeometryManager().GetGeometries().size()+1);
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
    if (mMoveActiveAction->isChecked() == true) {
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

void EditorWindow::DeleteModeActive()
{
    if (mDeleteModeActiveAction->isChecked() == true) {
        mObjectApp->ActiveDeleteMode();
    }
}

void EditorWindow::CombineFigureModeActive()
{
    if (mCombineFiguresMode->isChecked() == true) {
        ui->mTreeObjects->setDragEnabled(true);
        mDragActivated = true;
    } else {
        ui->mTreeObjects->setDragEnabled(false);
        mDragActivated = false;
    }
}

void EditorWindow::ChangeMode(QAction *_action)
{
    mLabelOfStatusBar->setText(_action->text() );
//    if (_action == mCombineFiguresMode && _action->isChecked()) {
//        mDragActivated = true;
//    } else {
//        mDragActivated = false;
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

    mSplitFigureActiveAction = new QAction("Split Figure",this);
    mSplitFigureActiveAction->setCheckable(true);
    mSplitFigureActiveAction->setShortcut(tr("Ctrl+E"));
    listActions << mSplitFigureActiveAction;
    connect(mSplitFigureActiveAction,SIGNAL(changed()),
            this,SLOT(SplitActive()));
    mModeActions.addAction(mSplitFigureActiveAction);

    mSplitObjectActiveAction = new QAction("Split Object", this);
    mSplitObjectActiveAction->setCheckable(true);
    listActions << mSplitObjectActiveAction;
    connect(mSplitObjectActiveAction,SIGNAL(changed()),
            this,SLOT(SplitActive()));
    mModeActions.addAction(mSplitObjectActiveAction);

    mStretchActiveAction = new QAction("Stretch active", this);
    mStretchActiveAction->setCheckable(true);
    mStretchActiveAction->setShortcut(tr("Ctrl+W"));
    listActions << mStretchActiveAction;
    connect(mStretchActiveAction, SIGNAL(changed()),
            this,SLOT(StretchActive()));
    mModeActions.addAction(mStretchActiveAction);

    mDeleteModeActiveAction = new QAction("Delete mode", this);
    mDeleteModeActiveAction->setCheckable(true);
    mDeleteModeActiveAction->setShortcut(tr("Ctrl+D"));
    listActions << mDeleteModeActiveAction;
    connect(mDeleteModeActiveAction,SIGNAL(changed()),
            this,SLOT(DeleteModeActive()));
    mModeActions.addAction(mDeleteModeActiveAction);

    mCombineFiguresMode = new QAction("Combine", this);
    mCombineFiguresMode->setCheckable(true);
    listActions << mCombineFiguresMode;
    connect(mCombineFiguresMode,SIGNAL(changed()),
            this, SLOT(CombineFigureModeActive()));
    mModeActions.addAction(mCombineFiguresMode);

    mRemoveAction = new QAction("Remove Object", this);
    mRemoveAction->setShortcut(tr("Ctrl+R"));
    listActions << mRemoveAction;
    connect(mRemoveAction,SIGNAL(triggered()),
            this, SLOT(Remove_Object()));


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

}


bool EditorWindow::UpdateTreeTemplates(QTreeWidget *_tree, drash::CApp *_app)
{
    _tree->clear();
    QList<QTreeWidgetItem*> list;
    CGeometryManager tSys= _app->GetGeometryManager();
    for (auto item = tSys.GetGeometries().begin();
        item != tSys.GetGeometries().end() ; item++) {

        QTreeWidgetItem *objectItem = new QTreeWidgetItem(_tree,
                                                      QStringList(QString::fromStdString(item->first)));
        _tree->addTopLevelItem(objectItem);

        const CSceneObjectGeometry &geo = *(item->second);
        const std::vector<CFigureParams> &mF = geo.mFigures;
        objectItem->setText(1,QString::number(mF.size()));
//       for (auto fig = mF.begin() ; fig != mF.end() ; fig++) {
//           CFigureParams par = *fig;

//           QString vecs("");
//           vecs.append("[");
//           for (unsigned int i = 0 ; i < par.mVertices.size() ; i++) {
//               if (i != 0) vecs.append(",");
//               vecs.append("(");
//               QString x = QString::number(par.mVertices[i].mX);
//               vecs.append(x);
//               vecs.append(",");
//               QString y = QString::number(par.mVertices[i].mY);
//               vecs.append(y);
//               vecs.append(")");
//           }
//           vecs.append("]");
//           new QTreeWidgetItem(objectItem,QStringList(vecs));
//       }
    }
    return true;
}

void EditorWindow::UpdateTreeSceneObjects()
{
    ui->mTreeSceneObjects->clear();
    QTreeWidget * tree = ui->mTreeSceneObjects;

    for (unsigned int i = 0; i < mSceneApp->GetCurrentLevel()->EnumObjects(); i++) {
        CLevelObjectDesc * cur_obj_desc = mSceneApp->GetCurrentLevel()->GetObjects()[i];

        QTreeWidgetItem *geometryItem = new QTreeWidgetItem(tree,
                                        QStringList(QString::fromStdString(cur_obj_desc->mLevelObjectName)));

        tree->addTopLevelItem(geometryItem);
        QString objectname = QString::fromStdString(cur_obj_desc->mGeometryName);
        new QTreeWidgetItem(geometryItem,QStringList(objectname));
    }
}

void EditorWindow::on_mTreeObjects_itemSelectionChanged()
{
    if (mDragActivated == true) {
        return;
    }
    QTreeWidgetItem *item = nullptr;
    if (ui->mTreeObjects->selectedItems().size() != 0) {
        item = ui->mTreeObjects->selectedItems().at(0);
    } else {
        return;
    }
    if (item->parent() == nullptr) {
        mObjectApp->ShowObject(item->text(0).toStdString());
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
        mObjectApp->GetGeometryManager().DestroyGeometry(item->text(0).toStdString());
        UpdateTreeTemplates(ui->mTreeObjects,mObjectApp);
        //mObjectApp->ShowObject(item->text(0).toStdString());
        //mCurrentObject = mObjectApp->GetGeometryManager().CreateSceneObjectFromTemplate(item->text(0).toStdString(),params);
    }
}

void EditorWindow::SplitActive()
{
    if (mSplitFigureActiveAction->isChecked() == true) {
        mObjectApp->ActiveSplitFigureMode();
    } else if (mSplitObjectActiveAction->isChecked() == true ) {
        mObjectApp->ActiveSplitObjectMode();
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

        ui->mParamsPanel->hide();

    } else if (index == 1) {
        mWidgetForObjects->hide();
        mWidgetForScene->show();
        mCurrentSceneWidget = mWidgetForScene;
        mObjectApp->GetGeometryManager().Store();
        mSceneApp->GetGeometryManager().Load();
        UpdateTreeTemplates(ui->mTreeTemplates,mSceneApp);
        mObjectToolBar->hide();

        mSceneToolbar->show();

        ui->mParamsPanel->show();
        ui->mParamsPanel->setEnabled(false);

        mSceneApp->ResetLevel();
    }
}

void EditorWindow::on_mTreeSceneObjects_clicked(const QModelIndex &)
{

    QTreeWidgetItem * item = ui->mTreeSceneObjects->selectedItems().at(0);
    if (item->parent() != nullptr) {
        mSceneApp->LookObject(item->parent()->text(0).toStdString(),
                              item->text(0).toStdString());
    }
}


CSceneObjectParams EditorWindow::GetObjectParams() const
{
    CSceneObjectParams buff;
    buff.mDynamic = ui->mCheckBoxDynamic->isChecked();
    buff.mAngle = (float)ui->mAngleBox->value();
    buff.mFixedRotation = ui->mCheckBoxFixedRotation->isChecked();
    return buff;
}

void EditorWindow::on_mCheckBoxDynamic_clicked()
{
    mSceneApp->SetDynamicParam(ui->mCheckBoxDynamic->isChecked());
}

void EditorWindow::on_mCheckBoxFixedRotation_clicked()
{
    mSceneApp->SetFixedRotationParam(ui->mCheckBoxFixedRotation->isChecked());
}

void EditorWindow::on_mAngleBox_valueChanged(double arg1)
{
    mSceneApp->SetAngleParams((float) arg1);
}


void EditorWindow::SetObjectParams(const CSceneObjectParams &_params)
{
    ui->mAngleBox->setValue(_params.mAngle);
    ui->mCheckBoxDynamic->setChecked(_params.mDynamic);
    ui->mCheckBoxFixedRotation->setChecked(_params.mFixedRotation);
}
