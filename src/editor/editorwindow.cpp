// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

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
#include <QDebug>

#include "../scene.h"
#include "../sceneobjects/figure.h"
#include "../sceneobjects/sceneobject.h"
#include "../subsystem/templatesystem.h"
#include "../misc/vec2.h"
#include "../debugdrawsystem/camera.h"
#include "editorapp.h"

#include <vector>
#include <string>

using namespace drash;

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow),
    mModeActions(parent)
{
    ui->setupUi(this);
    CreateActions();
    mModeActions.setExclusive(true);
    mLabelOfStatusBar = new QLabel("Editor Object");
    this->statusBar()->addWidget(mLabelOfStatusBar);

    this->setWindowTitle("DRASH Editor");

    if (this->InitScene()) {
        close();
    }

    this->startTimer(0);
    this->ui->mTreeObjects->clear();
    //this->ui->mTreeObjects->setColumnCount(2);
    UpdateTreeObject();

    mObjectApp->SetTreeRefreshHandler([this]()
    {
        this->UpdateTreeObject();
    });

}

EditorWindow::~EditorWindow()
{
    delete ui;
    mObjectApp->Release();
    delete mObjectApp;
}

bool EditorWindow::InitScene()
{
    drash::CSceneParams params;
    params.mGravity.Set( 0.0f, -9.8f );
    mObjectApp = new CObjectEditorApp();
    if (mObjectApp == nullptr) {
        return false;
    }

    if (mObjectApp->Init() == false) {
        return false;
    }
//    mObjectApp->GetDebugDrawSystem().GetActiveCam()->SetOrtho(true);
//    mObjectApp->GetDebugDrawSystem().GetActiveCam()->GetOrthoWidth().Set(120);
    mObjectApp->GetDebugDrawSystem().GetActiveCam()->GetPos().Set(CVec3f(0, 50, 100));
    mObjectApp->GetDebugDrawSystem().GetActiveCam()->GetRotation().Set(CVec3f(-M_PI / 6, 0, 0));
    ui->mScene->SetTestApp(mObjectApp);
    mCurrentApp = mObjectApp;
    return true;
}

void EditorWindow::timerEvent(QTimerEvent *)
{
    mTimer.Tick();

    if (mObjectApp != nullptr)
    {
        mObjectApp->Step(0);
    }

    ui->mScene->updateGL();
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

void EditorWindow::StretchActive()
{
    if (mStretchActiveAction->isChecked()) {
        mObjectApp->ActiveStretchMode();
    }
}

void EditorWindow::ZoomUp()
{
    if (mCurrentApp != nullptr){
        qDebug() << "Zoom up!";
        CVec3f pos = mCurrentApp->GetDebugDrawSystem().GetActiveCam()->GetPos().GetTarget();
        pos.mZ += 10.0f;
        mCurrentApp->GetDebugDrawSystem().GetActiveCam()->GetPos().SetTarget(pos, 0.3, AnimationBehaviorSingle);
    }
}

void EditorWindow::ZoomDown()
{
    if (mCurrentApp != nullptr){
        CVec3f pos = mCurrentApp->GetDebugDrawSystem().GetActiveCam()->GetPos().GetTarget();
        pos.mZ -= 10.0f;
        mCurrentApp->GetDebugDrawSystem().GetActiveCam()->GetPos().SetTarget(pos, 0.3, AnimationBehaviorSingle);
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

    mStretchActiveAction = new QAction("Stretch active", this);
    mStretchActiveAction->setCheckable(true);
    mStretchActiveAction->setShortcut(tr("Ctrl+W"));
    listActions << mStretchActiveAction;
    connect(mStretchActiveAction, SIGNAL(changed()),
            this,SLOT(StretchActive()));
    mModeActions.addAction(mStretchActiveAction);

    mZoomUpAction = new QAction("+", this);
    mZoomUpAction->setShortcut(tr("Ctrl+="));
    listActions << mZoomUpAction;
    connect(mZoomUpAction,SIGNAL(triggered()),
            this,SLOT(ZoomUp()));

    mZoomDownAction = new QAction("-",this);
    mZoomDownAction->setShortcut(tr("Ctrl+-"));
    listActions << mZoomDownAction;
    connect(mZoomDownAction,SIGNAL(triggered()),
            this,SLOT(ZoomDown()));

    ui->toolBar->addActions(listActions);

    mRemoveAction = new QAction("Remove Object", this);
    mRemoveAction->setShortcut(tr("Ctrl+D"));
    ui->toolBar->addAction(mRemoveAction);
    connect(mRemoveAction,SIGNAL(triggered()),
            this, SLOT(Remove_Object()));

//    mSaveAction = new QAction("Save Object", this);
//    mSaveAction->setShortcut(tr("Ctrl+S"));
//    ui->toolBar->addAction(mSaveAction);
//    connect(mSaveAction, SIGNAL(triggered()),
//            this,SLOT(SaveObject()));


}

void EditorWindow::SaveObject()
{
    mObjectApp->SaveCurrentObject();

    UpdateTreeObject();
}

bool EditorWindow::UpdateTreeObject()
{
    ui->mTreeObjects->clear();
    QList<QTreeWidgetItem*> list;
    CTemplateSystem tSys= mObjectApp->GetTemplateSystem();
    for (auto item = tSys.GetSceneObjectTemplates().begin();
         item != tSys.GetSceneObjectTemplates().end() ; item++) {
       QTreeWidgetItem *objectItem = new QTreeWidgetItem(ui->mTreeObjects,
                                                      QStringList(QString::fromStdString(item->first)));
       ui->mTreeObjects->addTopLevelItem(objectItem);
//       const CSceneObjectGeometry &ii = *(item->second);
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
//           QTreeWidgetItem *itemFig =
           new QTreeWidgetItem(objectItem,QStringList(vecs));
       }
    }
    mMoveActiveAction->setChecked(true);
    return true;
}

void EditorWindow::AddFigure()
{
    std::string nameTemplate = ui->mTreeObjects->selectedItems().at(0)->text(0).toStdString();

    mObjectApp->BuildFigure(nameTemplate);
}


void EditorWindow::on_mTreeObjects_itemDoubleClicked(QTreeWidgetItem *item, int column)
{

}

void EditorWindow::on_mTreeObjects_itemClicked(QTreeWidgetItem *item, int column)
{

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
        UpdateTreeObject();
        //mObjectApp->ShowObject(item->text(0).toStdString());
        //mCurrentObject = mObjectApp->GetTemplateSystem().CreateSceneObjectFromTemplate(item->text(0).toStdString(),params);
    }
}
