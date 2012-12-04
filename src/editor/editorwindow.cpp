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
    this->InitScene();

    this->startTimer(0);
    this->ui->mTreeObjects->clear();
    this->ui->mTreeObjects->setColumnCount(2);
//    QTreeWidget *treeWidget = new QTreeWidget();
//    treeWidget->setColumnCount(1);
//    QList<QTreeWidgetItem *> items; //= new QList<QTreeWidgetItem *>();
//    for (int i = 0; i < 10; ++i){
//        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));
//    }
//    ui->mTreeObjects->insertTopLevelItems(0, items);
//    QTreeWidgetItem* item = new QTreeWidgetItem(items[0],QStringList(QString("ABC")));
    UpdateTreeObject();
//    ui->mScene->RemoveObjects();
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
    mObjectApp->GetDebugDrawSystem().GetActiveCam()->SetZ(100);
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
}

bool EditorWindow::UpdateTreeObject()
{
    ui->mTreeObjects->clear();
    QList<QTreeWidgetItem*> list;
    CTemplateSystem tSys= mObjectApp->GetTemplateSystem();
    for (auto item = tSys.GetSceneObjectTemplates().begin();
         item != tSys.GetSceneObjectTemplates().end() ; item++) {
       QTreeWidgetItem *objectItem = new QTreeWidgetItem(ui->mTreeObjects,
                                                      QStringList(QString::fromStdString((*item)->mName)));
       ui->mTreeObjects->addTopLevelItem(objectItem);
       const CSceneObjectTemplate &ii = **item;
       const CSceneObjectGeometry &geo = ii.mGeometry;
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

    mObjectApp->BuildObject(nameTemplate);
}


void EditorWindow::on_mTreeObjects_itemDoubleClicked(QTreeWidgetItem *item, int column)
{

}

void EditorWindow::on_mNewObjectButton_clicked()
{
    QString str_name("Object");
    str_name +=QString::number(mObjectApp->GetTemplateSystem().GetSceneObjectTemplates().size()+1);
    QTreeWidgetItem *newItem = new QTreeWidgetItem(ui->mTreeObjects,QStringList(str_name));
    mObjectApp->AddNewObjectToTemplate(str_name.toStdString());
}

void EditorWindow::on_mBuildButton_clicked()
{
    this->AddFigure();
    this->UpdateTreeObject();
}

void EditorWindow::on_mNewFigureButton_clicked()
{
    //RemoveCurrentObject();
    mObjectApp->StartBuild();
}


void EditorWindow::RemoveCurrentObject() {
    if (mCurrentObject != nullptr) {
        mObjectApp->GetScene().DestroyObject(mCurrentObject);
        mCurrentObject = nullptr;
    }
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
        if (mCurrentObject != nullptr) {
            mObjectApp->GetScene().DestroyObject(mCurrentObject);
            mCurrentObject = nullptr;
        }
        CSceneObjectParams params;
        params.mPos.Set(0,0);
        qDebug() << "Object created";
        mCurrentObject = mObjectApp->GetTemplateSystem().CreateSceneObjectFromTemplate(item->text(0).toStdString(),params);
    }
}
