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

#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include "../diag/timer.h"
#include "../qt/scenewidget.h"
#include <QActionGroup>
#include <QHBoxLayout>
class QTreeWidgetItem;
class QLabel;

namespace drash {
    class ObjectEditorApp;
    class App;
    class SceneEditorApp;
    class SceneObjectParams;
}

namespace Ui {
    class EditorWindow;
}

class QStatusBar;
class QTreeWidget;
class QModelIndex;

class EditorWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit EditorWindow(QWidget* parent = 0);
    ~EditorWindow();

private
slots:

    void on_mTreeObjects_itemSelectionChanged();

private:
    Ui::EditorWindow* ui;
    bool InitScene();
    drash::ObjectEditorApp* objectApp = nullptr;
    drash::SceneEditorApp* sceneApp = nullptr;
    drash::App* currentApp = nullptr;

    virtual void timerEvent(QTimerEvent*) override;

    SceneWidget* widgetForScene = nullptr;
    SceneWidget* widgetForObjects = nullptr;
    SceneWidget* currentSceneWidget = nullptr;
    // drash::SceneObject * currentObject = nullptr;

    bool dragActivated = false;

    // Actions
private:
    // for editor object
    QAction* quit;
    QAction* removeAction;
    QAction* moveActiveAction;
    QAction* stretchActiveAction;
    QAction* moveOfAxisActiveAction;
    QAction* newFigureAction;
    QAction* newObjectAction;
    QAction* deleteModeActiveAction;
    QAction* splitFigureActiveAction;
    QAction* splitObjectActiveAction;
    QActionGroup modeActions;
    QAction* combineFiguresMode;

    // for editor scene
    QAction* openLevelAction;
    QAction* saveLevelAction;
    QAction* saveLevelAsAction;
    QAction* playLevelAction;
    QAction* pauseLevelAction;
    QAction* stopLevelAction;
    QAction* newLevelAction;

    // for other
    // Slots for Actions
private
slots:
    // for edtitor object
    void CreateNewObject();
    void AddNewFigure();
    void MoveActive();
    void MoveOfAxisActive();
    void StretchActive();
    void Remove_Object();
    void SplitActive();
    void DeleteModeActive();
    void CombineFigureModeActive();

    void ChangeMode(QAction* _action);

    // for editor scene
    void OpenLevel();
    void SaveLevel();
    void SaveLevelAs();
    void PlayLevel();
    void PauseLevel();
    void StopLevel();

    void NewLevel();
    void on_mManageWidget_currentChanged(int index);

    void on_mTreeSceneObjects_clicked(const QModelIndex&);

    void on_mCheckBoxDynamic_clicked();

    void on_mCheckBoxFixedRotation_clicked();

    void on_mAngleBox_valueChanged(double arg1);

private:
    // GuiObjects
    QLabel* labelOfStatusBar;

    QToolBar* objectToolBar;
    QToolBar* sceneToolbar;

private:
    void CreateActions();

    bool UpdateTreeTemplates(QTreeWidget* _tree, drash::App* _app);

    void UpdateTreeSceneObjects();

    drash::Timer timer;

    void AddFigure();
    void SetObjectParams(const drash::SceneObjectParams& _params);
    drash::SceneObjectParams GetObjectParams() const;

private:
    QHBoxLayout* layoutForScene;

public:
};

#endif // EDITORWINDOW_H
