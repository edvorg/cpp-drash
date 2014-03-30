// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2014 Edward Knyshov, Yuriy Shatilin.

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
#include "../scene/geometrymanager.h"
#include "../greng/camera.h"
#include "editorapp.h"
#include "sceneeditorapp.h"
#include "../levelmanager/leveldesc.h"

using namespace drash;

EditorWindow::EditorWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::EditorWindow), modeActions(parent) {
    ui->setupUi(this);

    sceneToolbar = new QToolBar(this);
    objectToolBar = new QToolBar(this);
    this->addToolBar(sceneToolbar);
    this->addToolBar(objectToolBar);

    CreateActions();
    modeActions.setExclusive(true);
    connect(&modeActions, SIGNAL(selected(QAction*)), this,
            SLOT(ChangeMode(QAction*)));

    labelOfStatusBar = new QLabel("Editor Object");

    this->statusBar()->addWidget(labelOfStatusBar);

    this->setWindowTitle("DRASH Editor");

    layoutForScene = new QHBoxLayout();
    ui->spike->setLayout(layoutForScene);

    if (this->InitScene() == false) {
        close();
    }

    currentSceneWidget = widgetForObjects;

    this->ui->treeObjects->clear();

    objectApp->SetTreeRefreshHandler([this]() {
        this->UpdateTreeTemplates(ui->treeObjects, objectApp);
        moveActiveAction->setChecked(true);
    });

    sceneApp->SetTreeRefreshHandler([this]() {
        this->UpdateTreeSceneObjects();
    });

    sceneToolbar->hide();
    timer.Reset(true);

    ui->treeTemplates->setDragEnabled(true);
    //    ui->treeTemplates->setDragDropMode();
    //    emit moveActiveAction->trigger();
    //    ui->treeTemplates->setDragDropMode(QAbstractItemView::InternalMove);
    widgetForScene->hide();
    sceneApp->SetGetSelectedHandler([this]() {
        QTreeWidgetItem* item = ui->treeTemplates->currentItem();
        if (item->parent() == NULL) {
            return item->text(0).toStdString();
        } else {
            return std::string("");
        }
    });

    objectApp->SetGetSelectedHandler([this]() {
        QTreeWidgetItem* item = ui->treeObjects->currentItem();
        if (item->parent() == NULL) {
            return item->text(0).toStdString();
        } else {
            return std::string("");
        }
    });
    ui->paramsPanel->hide();

    this->startTimer(0);
}

EditorWindow::~EditorWindow() {
    delete ui;
    delete objectApp;
    delete sceneApp;
}

bool EditorWindow::InitScene() {
    drash::SceneParams params;
    params.gravity.Set(0.0f, -9.8f);

    sceneApp = new SceneEditorApp();
    objectApp = new ObjectEditorApp();

    widgetForObjects = new SceneWidget(this);
    widgetForScene = new SceneWidget(this);
    widgetForScene->SetApp(sceneApp);
    widgetForObjects->SetApp(objectApp);

    layoutForScene->addWidget(widgetForScene);
    layoutForScene->addWidget(widgetForObjects);
    widgetForScene->show();
    ui->manageWidget->setCurrentIndex(0);

    if (widgetForObjects->GetApp() == nullptr ||
        widgetForScene->GetApp() == nullptr) {
        return false;
    }

    return true;
}

void EditorWindow::timerEvent(QTimerEvent*) {
    timer.Tick();

    if (currentSceneWidget->GetApp() != nullptr) {
        currentSceneWidget->GetApp()->Step(timer.GetDeltaTime());
    } else {
        close();
    }

    if (currentSceneWidget == widgetForScene) {
        if (sceneApp->IsObjectSelected() == true) {
            ui->paramsPanel->setEnabled(true);
            if (sceneApp->IsChangedParams() == true) {
                SetObjectParams(sceneApp->GetSelectedParams());
                // ui->angleBox->setValue((double)sceneApp->GetAngleParams());
            }
        } else {
            ui->paramsPanel->setEnabled(false);
        }
    }

    currentSceneWidget->updateGL();
}

void EditorWindow::CreateNewObject() {
    QString str_name;
    int i = 0;
    do {
        str_name = "Object";
        str_name += QString::number(
            objectApp->GetGeometryManager().GetGeometries().size() + 1 + i);
        i++;
    } while (objectApp->AddNewObjectToTemplate(str_name.toStdString()) ==
             false);

    str_name += QString::number(
        objectApp->GetGeometryManager().GetGeometries().size() + 1);
    QTreeWidgetItem* newItem =
        new QTreeWidgetItem(ui->treeObjects, QStringList(str_name));
    ui->treeObjects->addTopLevelItem(newItem);
    newItem->setSelected(true);
    UpdateTreeTemplates(ui->treeObjects, objectApp);
}

void EditorWindow::AddNewFigure() {
    if (newFigureAction->isChecked()) {
        objectApp->StartBuild();
    }
}

void EditorWindow::MoveActive() {
    if (moveActiveAction->isChecked() == true) {
        objectApp->ActiveMoveMode();
    }
}

void EditorWindow::MoveOfAxisActive() {
    if (moveOfAxisActiveAction->isChecked()) {
        objectApp->ActiveMoveOfAxisMode();
    }
}

void EditorWindow::StretchActive() {
    if (stretchActiveAction->isChecked()) {
        objectApp->ActiveStretchMode();
    }
}

void EditorWindow::DeleteModeActive() {
    if (deleteModeActiveAction->isChecked() == true) {
        objectApp->ActiveDeleteMode();
    }
}

void EditorWindow::CombineFigureModeActive() {
    if (combineFiguresMode->isChecked() == true) {
        ui->treeObjects->setDragEnabled(true);
        dragActivated = true;
    } else {
        ui->treeObjects->setDragEnabled(false);
        dragActivated = false;
    }
}

void EditorWindow::ChangeMode(QAction* _action) {
    labelOfStatusBar->setText(_action->text());
    //    if (_action == combineFiguresMode && _action->isChecked()) {
    //        dragActivated = true;
    //    } else {
    //        dragActivated = false;
    //    }
}

void EditorWindow::OpenLevel() {
    //    qDebug() << "Open file";
    QString name = QFileDialog::getOpenFileName(this, "Open level file");
    if (sceneApp->LoadLevel(name.toStdString()) == true) {
        labelOfStatusBar->setText("Level load from " + name);
    } else {
        QMessageBox::critical(this, "Error", "Level not load. See log file");
    }
}

void EditorWindow::SaveLevel() {
    //    qDebug() << " Save file";
    if (sceneApp->IsSetLevel() == false) {
        QMessageBox::critical(this, "Error", "Level is not set now!!");
        return;
    }
    if (sceneApp->IsSetFileName() == false) {
        SaveLevelAs();
    } else {
        if (sceneApp->SaveLevel() == false) {
            QMessageBox::critical(this, "Error",
                                  "Level is not save. See log file");
        } else {
            labelOfStatusBar->setText(
                "Level saved in " +
                QString::fromStdString(sceneApp->GetLevelFileName()));
        }
    }
}

void EditorWindow::SaveLevelAs() {
    if (sceneApp->IsSetLevel() == false) {
        QMessageBox::critical(this, "Error", "Level is not set now!!");
        return;
    }
    QString name = QFileDialog::getSaveFileName(this, "Choose file for saving");
    if (sceneApp->SaveLevelAs(name.toStdString()) == false) {
        QMessageBox::critical(this, "Error", "Level is not save. See log file");
    } else {
        labelOfStatusBar->setText("Level saved in " + name);
    }
}

void EditorWindow::PlayLevel() { sceneApp->StartCurrentLevel(); }

void EditorWindow::PauseLevel() { sceneApp->PauseLevel(); }

void EditorWindow::StopLevel() { sceneApp->StopLevel(); }

void EditorWindow::NewLevel() {
    if (sceneApp->NewLevel() == true) {
        labelOfStatusBar->setText("Created new level");
    }
}

void EditorWindow::CreateActions() {
    quit = new QAction("Quit", this);
    quit->setShortcut(tr("Ctrl+Q"));
    this->addAction(quit);
    connect(quit, SIGNAL(triggered()), this, SLOT(close()));
    QList<QAction*> listActions;

    newObjectAction = new QAction("New SceneObject", this);
    newObjectAction->setShortcut(tr("Ctrl+N"));
    listActions << newObjectAction;
    connect(newObjectAction, SIGNAL(triggered()), this,
            SLOT(CreateNewObject()));

    newFigureAction = new QAction("Build new Figure", this);
    newFigureAction->setShortcut(tr("Ctrl+F"));
    newFigureAction->setCheckable(true);
    listActions << newFigureAction;
    connect(newFigureAction, SIGNAL(changed()), this, SLOT(AddNewFigure()));
    modeActions.addAction(newFigureAction);

    moveActiveAction = new QAction("Move Figure", this);
    moveActiveAction->setCheckable(true);
    moveActiveAction->setShortcut(tr("Ctrl+M"));
    listActions << moveActiveAction;
    connect(moveActiveAction, SIGNAL(changed()), this, SLOT(MoveActive()));
    modeActions.addAction(moveActiveAction);

    moveOfAxisActiveAction = new QAction("Move Of Axis Figure", this);
    moveOfAxisActiveAction->setCheckable(true);
    moveOfAxisActiveAction->setShortcut(tr("Ctrl+A"));
    listActions << moveOfAxisActiveAction;
    connect(moveOfAxisActiveAction, SIGNAL(changed()), this,
            SLOT(MoveOfAxisActive()));
    modeActions.addAction(moveOfAxisActiveAction);

    splitFigureActiveAction = new QAction("Split Figure", this);
    splitFigureActiveAction->setCheckable(true);
    splitFigureActiveAction->setShortcut(tr("Ctrl+E"));
    listActions << splitFigureActiveAction;
    connect(splitFigureActiveAction, SIGNAL(changed()), this,
            SLOT(SplitActive()));
    modeActions.addAction(splitFigureActiveAction);

    splitObjectActiveAction = new QAction("Split Object", this);
    splitObjectActiveAction->setCheckable(true);
    listActions << splitObjectActiveAction;
    connect(splitObjectActiveAction, SIGNAL(changed()), this,
            SLOT(SplitActive()));
    modeActions.addAction(splitObjectActiveAction);

    stretchActiveAction = new QAction("Stretch active", this);
    stretchActiveAction->setCheckable(true);
    stretchActiveAction->setShortcut(tr("Ctrl+W"));
    listActions << stretchActiveAction;
    connect(stretchActiveAction, SIGNAL(changed()), this,
            SLOT(StretchActive()));
    modeActions.addAction(stretchActiveAction);

    deleteModeActiveAction = new QAction("Delete mode", this);
    deleteModeActiveAction->setCheckable(true);
    deleteModeActiveAction->setShortcut(tr("Ctrl+D"));
    listActions << deleteModeActiveAction;
    connect(deleteModeActiveAction, SIGNAL(changed()), this,
            SLOT(DeleteModeActive()));
    modeActions.addAction(deleteModeActiveAction);

    combineFiguresMode = new QAction("Combine", this);
    combineFiguresMode->setCheckable(true);
    listActions << combineFiguresMode;
    connect(combineFiguresMode, SIGNAL(changed()), this,
            SLOT(CombineFigureModeActive()));
    modeActions.addAction(combineFiguresMode);

    removeAction = new QAction("Remove Object", this);
    removeAction->setShortcut(tr("Ctrl+R"));
    listActions << removeAction;
    connect(removeAction, SIGNAL(triggered()), this, SLOT(Remove_Object()));

    objectToolBar->addActions(listActions);

    // Actions for editor scene
    listActions.clear();

    openLevelAction = new QAction("Open Level", this);
    listActions << openLevelAction;
    connect(openLevelAction, SIGNAL(triggered()), this, SLOT(OpenLevel()));

    saveLevelAction = new QAction("Save Level", this);
    listActions << saveLevelAction;
    connect(saveLevelAction, SIGNAL(triggered()), this, SLOT(SaveLevel()));

    saveLevelAsAction = new QAction("Save As Level", this);
    listActions << saveLevelAsAction;
    connect(saveLevelAsAction, SIGNAL(triggered()), this, SLOT(SaveLevelAs()));

    playLevelAction = new QAction("Start Level", this);
    listActions << playLevelAction;
    connect(playLevelAction, SIGNAL(triggered()), this, SLOT(PlayLevel()));

    pauseLevelAction = new QAction("Pause Level", this);
    listActions << pauseLevelAction;
    connect(pauseLevelAction, SIGNAL(triggered()), this, SLOT(PauseLevel()));

    stopLevelAction = new QAction("Stop Level", this);
    listActions << stopLevelAction;
    connect(stopLevelAction, SIGNAL(triggered()), this, SLOT(StopLevel()));

    newLevelAction = new QAction("New Level", this);
    listActions.push_front(newLevelAction);
    connect(newLevelAction, SIGNAL(triggered()), this, SLOT(NewLevel()));

    sceneToolbar->addActions(listActions);
}

bool EditorWindow::UpdateTreeTemplates(QTreeWidget* _tree, drash::App* _app) {
    _tree->clear();
    QList<QTreeWidgetItem*> list;
    GeometryManager tSys = _app->GetGeometryManager();
    for (auto item = tSys.GetGeometries().begin();
         item != tSys.GetGeometries().end(); item++) {

        QTreeWidgetItem* objectItem = new QTreeWidgetItem(
            _tree, QStringList(QString::fromStdString(item->first)));
        _tree->addTopLevelItem(objectItem);

        const SceneObjectGeometry& geo = *(item->second);
        const std::vector<FigureParams>& f = geo.figures;
        objectItem->setText(1, QString::number(f.size()));
        //       for (auto fig = f.begin() ; fig != f.end() ; fig++) {
        //           FigureParams par = *fig;

        //           QString vecs("");
        //           vecs.append("[");
        //           for (unsigned int i = 0 ; i < par.vertices.size() ; i++) {
        //               if (i != 0) vecs.append(",");
        //               vecs.append("(");
        //               QString x = QString::number(par.vertices[i].x);
        //               vecs.append(x);
        //               vecs.append(",");
        //               QString y = QString::number(par.vertices[i].y);
        //               vecs.append(y);
        //               vecs.append(")");
        //           }
        //           vecs.append("]");
        //           new QTreeWidgetItem(objectItem,QStringList(vecs));
        //       }
    }
    return true;
}

void EditorWindow::UpdateTreeSceneObjects() {
    ui->treeSceneObjects->clear();
    QTreeWidget* tree = ui->treeSceneObjects;

    for (unsigned int i = 0; i < sceneApp->GetCurrentLevel()->EnumObjects();
         i++) {
        LevelObjectDesc* cur_obj_desc =
            sceneApp->GetCurrentLevel()->GetObjects()[i];

        QTreeWidgetItem* geometryItem = new QTreeWidgetItem(
            tree,
            QStringList(QString::fromStdString(cur_obj_desc->levelObjectName)));

        tree->addTopLevelItem(geometryItem);
        QString objectname = QString::fromStdString(cur_obj_desc->geometryName);
        new QTreeWidgetItem(geometryItem, QStringList(objectname));
    }
}

void EditorWindow::on_mTreeObjects_itemSelectionChanged() {
    if (dragActivated == true) {
        return;
    }
    QTreeWidgetItem* item = nullptr;
    if (ui->treeObjects->selectedItems().size() != 0) {
        item = ui->treeObjects->selectedItems().at(0);
    } else {
        return;
    }
    if (item->parent() == nullptr) {
        objectApp->ShowObject(item->text(0).toStdString());
    }
}

void EditorWindow::Remove_Object() {
    QTreeWidgetItem* item = nullptr;
    if (ui->treeObjects->selectedItems().size() != 0) {
        item = ui->treeObjects->selectedItems().at(0);
    } else {
        return;
    }
    if (item->parent() == nullptr) {
        // qDebug() << "Object created";
        objectApp->GetGeometryManager().DestroyGeometry(
            item->text(0).toStdString());
        UpdateTreeTemplates(ui->treeObjects, objectApp);
        // objectApp->ShowObject(item->text(0).toStdString());
        // currentObject =
        // objectApp->GetGeometryManager().CreateSceneObjectFromTemplate(item->text(0).toStdString(),params);
    }
}

void EditorWindow::SplitActive() {
    if (splitFigureActiveAction->isChecked() == true) {
        objectApp->ActiveSplitFigureMode();
    } else if (splitObjectActiveAction->isChecked() == true) {
        objectApp->ActiveSplitObjectMode();
    }
}

void EditorWindow::on_mManageWidget_currentChanged(int index) {
    //    qDebug() << " Changed " << index ;
    if (index == 0) {
        widgetForScene->hide();
        widgetForObjects->show();
        currentSceneWidget = widgetForObjects;
        sceneToolbar->hide();
        objectToolBar->show();

        ui->paramsPanel->hide();

    } else if (index == 1) {
        widgetForObjects->hide();
        widgetForScene->show();
        currentSceneWidget = widgetForScene;
        objectApp->GetGeometryManager().Store();
        sceneApp->GetGeometryManager().Load();
        UpdateTreeTemplates(ui->treeTemplates, sceneApp);
        objectToolBar->hide();

        sceneToolbar->show();

        ui->paramsPanel->show();
        ui->paramsPanel->setEnabled(false);

        sceneApp->ResetLevel();
    }
}

void EditorWindow::on_mTreeSceneObjects_clicked(const QModelIndex&) {

    QTreeWidgetItem* item = ui->treeSceneObjects->selectedItems().at(0);
    if (item->parent() != nullptr) {
        sceneApp->LookObject(item->parent()->text(0).toStdString(),
                             item->text(0).toStdString());
    }
}

CSceneObjectParams EditorWindow::GetObjectParams() const {
    SceneObjectParams buff;
    buff.dynamic = ui->checkBoxDynamic->isChecked();
    buff.angle = (float)ui->angleBox->value();
    buff.fixedRotation = ui->checkBoxFixedRotation->isChecked();
    return buff;
}

void EditorWindow::on_mCheckBoxDynamic_clicked() {
    sceneApp->SetDynamicParam(ui->checkBoxDynamic->isChecked());
}

void EditorWindow::on_mCheckBoxFixedRotation_clicked() {
    sceneApp->SetFixedRotationParam(ui->checkBoxFixedRotation->isChecked());
}

void EditorWindow::on_mAngleBox_valueChanged(double arg1) {
    sceneApp->SetAngleParams((float)arg1);
}

void EditorWindow::SetObjectParams(const SceneObjectParams& _params) {
    ui->angleBox->setValue(_params.angle);
    ui->checkBoxDynamic->setChecked(_params.dynamic);
    ui->checkBoxFixedRotation->setChecked(_params.fixedRotation);
}
