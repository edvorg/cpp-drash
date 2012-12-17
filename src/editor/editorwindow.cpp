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
#include "../cscene.h"
#include "../subsystem/templatesystem.h"
#include "../misc/cvec2.h"

#include <vector>
#include <string>
#include <QTreeWidgetItem>
#include <QStringList>

using namespace drash;

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow)
{
    ui->setupUi(this);
    CreateActions();

    mLabelOfStatusBar = new QLabel("Editor Object");
    this->statusBar()->addWidget(mLabelOfStatusBar);

    this->setWindowTitle("DRASH Editor");

    if (this->InitScene()) {
        close();
    }

    this->startTimer(0);
    this->ui->mTreeObjects->clear();
    this->ui->mTreeObjects->setColumnCount(2);
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
    if (mObjectApp == NULL) {
        return false;
    }

    if (mObjectApp->Init() == false) {
        return false;
    }
    mObjectApp->GetDebugDrawSystem().GetActiveCam()->GetPos().Set(CVec3f(0, 0, 100));
    ui->mScene->SetTestApp(mObjectApp);
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

void EditorWindow::CreateActions()
{
    mQuit = new QAction("Quit",this);
    mQuit->setShortcut(tr("Ctrl+Q"));
    this->addAction(mQuit);
    connect(mQuit,SIGNAL(triggered()),
            this,SLOT(close()));

    mRemoveAction = new QAction("Remove Object", this);
    mRemoveAction->setShortcut(tr("Ctrl+D"));
    ui->toolBar->addAction(mRemoveAction);
    connect(mRemoveAction,SIGNAL(triggered()),
            this, SLOT(Remove_Object()));

    mSaveAction = new QAction("Save Object", this);
    mSaveAction->setShortcut(tr("Ctrl+S"));
    ui->toolBar->addAction(mSaveAction);
    connect(mSaveAction, SIGNAL(triggered()),
            this,SLOT(SaveObject()));
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
               QString x = QString::number(par.mVertices[i].x);
               vecs.append(x);
               vecs.append(",");
               QString y = QString::number(par.mVertices[i].y);
               vecs.append(y);
               vecs.append(")");
           }
           vecs.append("]");
//           QTreeWidgetItem *itemFig =
           new QTreeWidgetItem(objectItem,QStringList(vecs));
       }
    }

    return true;
}

void EditorWindow::AddFigure()
{
    std::string nameTemplate = ui->mTreeObjects->selectedItems().at(0)->text(0).toStdString();

    mObjectApp->BuildFigure(nameTemplate);
    //this->UpdateTreeObject();
}


void EditorWindow::on_mTreeObjects_itemDoubleClicked(QTreeWidgetItem *item, int column)
{

}

void EditorWindow::on_mNewObjectButton_clicked()
{
    QString str_name("Object");
    str_name +=QString::number(mObjectApp->GetTemplateSystem().GetSceneObjectTemplates().size()+1);
    QTreeWidgetItem *newItem = new QTreeWidgetItem(ui->mTreeObjects,QStringList(str_name));
    newItem->setSelected(true);
//    ui->mTreeObjects->SetS
    mObjectApp->AddNewObjectToTemplate(str_name.toStdString());

}

void EditorWindow::on_mBuildButton_clicked()
{
    this->AddFigure();
}

void EditorWindow::on_mNewFigureButton_clicked()
{
    mObjectApp->StartBuild();
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
    if (item->parent() == NULL) {
        qDebug() << "Object created";
        mObjectApp->ShowObject(item->text(0).toStdString());
        //mCurrentObject = mObjectApp->GetTemplateSystem().CreateSceneObjectFromTemplate(item->text(0).toStdString(),params);
    }
}

void EditorWindow::on_ActiveMoveButton_clicked()
{
    mObjectApp->ActiveMoveMode();
}

void EditorWindow::Remove_Object()
{
    QTreeWidgetItem *item = nullptr;
    if (ui->mTreeObjects->selectedItems().size() != 0) {
        item = ui->mTreeObjects->selectedItems().at(0);
    } else {
        return;
    }
    if (item->parent() == NULL) {
        //qDebug() << "Object created";
        mObjectApp->GetTemplateSystem().RemoveSceneObjectTemplate(item->text(0).toStdString());
        UpdateTreeObject();
        //mObjectApp->ShowObject(item->text(0).toStdString());
        //mCurrentObject = mObjectApp->GetTemplateSystem().CreateSceneObjectFromTemplate(item->text(0).toStdString(),params);
    }
}
