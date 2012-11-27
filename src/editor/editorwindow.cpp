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
#include <string>

#include <QTreeWidgetItem>
#include <QStringList>
#include "../subsystem/templatesystem.h"
//#include ""
using namespace drash;

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow)
{
    ui->setupUi(this);
    CreateActions();
    this->setWindowTitle("DRASH Editor");
    this->InitScene();

    this->startTimer(0);
    this->ui->mTreeObjects->clear();
//    QTreeWidget *treeWidget = new QTreeWidget();
//    treeWidget->setColumnCount(1);
//    QList<QTreeWidgetItem *> items; //= new QList<QTreeWidgetItem *>();
//    for (int i = 0; i < 10; ++i){
//        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));
//    }
//    ui->mTreeObjects->insertTopLevelItems(0, items);
//    QTreeWidgetItem* item = new QTreeWidgetItem(items[0],QStringList(QString("ABC")));
    auto t = mApp->GetTemplateSystem().CreateSceneObjectTemplate("name1");
    if (t != nullptr)
    {
        t->mGeometry.mFigures.resize(1);
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-2, -1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(2, -1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(1, 1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-1, 1));
    }
    t = mApp->GetTemplateSystem().CreateSceneObjectTemplate("name2");
    if (t != nullptr)
    {
        t->mGeometry.mFigures.resize(1);
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(2, 1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-2, 1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-1, -1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(1, -1));
    }
    t = mApp->GetTemplateSystem().CreateSceneObjectTemplate("ground");
    if (t != nullptr)
    {
        t->mGeometry.mFigures.resize(1);
        t->mGeometry.mFigures[0].mDepth = 10;
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(50, 5));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-50, 5));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-50, -5));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(50, -5));
    }
    UpdateTreeObject();
}



EditorWindow::~EditorWindow()
{
    delete ui;
}

bool EditorWindow::InitScene()
{
    drash::CSceneParams params;
    params.mGravity.Set( 0.0f, -9.8f );
    mApp = drash::StartApp("test1");
    if (mApp == NULL) {
        return false;
    }

    if (mApp->Init() == false) {
        return false;
    }

    ui->mScene->SetTestApp(mApp);
    return true;
}

void EditorWindow::timerEvent(QTimerEvent *)
{
    mTimer.Tick();

    if (mApp != nullptr)
    {
        mApp->Step(0);
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
    CTemplateSystem tSys= mApp->GetTemplateSystem();
    for (auto item = tSys.GetTemplates().begin();
         item != tSys.GetTemplates().end() ; item++) {
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
           for (int i = 0 ; i < par.mVertices.size() ; i++) {
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
           QTreeWidgetItem *itemFig = new QTreeWidgetItem(objectItem,QStringList(vecs));
       }
    }

    return true;
}


void EditorWindow::on_pushButton_clicked()
{
    this->ui->mScene->StartBuildObject();
}

void EditorWindow::on_pushButton_2_clicked()
{
    ui->mScene->BuildNow();
}
