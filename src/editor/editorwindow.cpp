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
    QTreeWidget *treeWidget = new QTreeWidget();
    treeWidget->setColumnCount(1);
    QList<QTreeWidgetItem *> items;
    for (int i = 0; i < 10; ++i)
        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("item: %1").arg(i))));
    ui->mTreeObjects->insertTopLevelItems(0, items);
    QTreeWidgetItem item;
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


void EditorWindow::on_pushButton_clicked()
{
    this->ui->mScene->StartBuildObject();
}

void EditorWindow::on_pushButton_2_clicked()
{
    ui->mScene->BuildNow();
}
