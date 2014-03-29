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
    class CObjectEditorApp;
    class CApp;
    class CSceneEditorApp;
    class CSceneObjectParams;
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
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();

  private
slots:

    void on_mTreeObjects_itemSelectionChanged();

  private:
    Ui::EditorWindow *ui;
    bool InitScene();
    drash::CObjectEditorApp *mObjectApp = nullptr;
    drash::CSceneEditorApp *mSceneApp = nullptr;
    drash::CApp *mCurrentApp = nullptr;

    virtual void timerEvent(QTimerEvent *) override;

    SceneWidget *mWidgetForScene = nullptr;
    SceneWidget *mWidgetForObjects = nullptr;
    SceneWidget *mCurrentSceneWidget = nullptr;
    // drash::CSceneObject * mCurrentObject = nullptr;

    bool mDragActivated = false;

    // Actions
  private:
    // for editor object
    QAction *mQuit;
    QAction *mRemoveAction;
    QAction *mMoveActiveAction;
    QAction *mStretchActiveAction;
    QAction *mMoveOfAxisActiveAction;
    QAction *mNewFigureAction;
    QAction *mNewObjectAction;
    QAction *mDeleteModeActiveAction;
    QAction *mSplitFigureActiveAction;
    QAction *mSplitObjectActiveAction;
    QActionGroup mModeActions;
    QAction *mCombineFiguresMode;

    // for editor scene
    QAction *mOpenLevelAction;
    QAction *mSaveLevelAction;
    QAction *mSaveLevelAsAction;
    QAction *mPlayLevelAction;
    QAction *mPauseLevelAction;
    QAction *mStopLevelAction;
    QAction *mNewLevelAction;

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

    void ChangeMode(QAction *_action);

    // for editor scene
    void OpenLevel();
    void SaveLevel();
    void SaveLevelAs();
    void PlayLevel();
    void PauseLevel();
    void StopLevel();

    void NewLevel();
    void on_mManageWidget_currentChanged(int index);

    void on_mTreeSceneObjects_clicked(const QModelIndex &);

    void on_mCheckBoxDynamic_clicked();

    void on_mCheckBoxFixedRotation_clicked();

    void on_mAngleBox_valueChanged(double arg1);

  private:
    // GuiObjects
    QLabel *mLabelOfStatusBar;

    QToolBar *mObjectToolBar;
    QToolBar *mSceneToolbar;

  private:
    void CreateActions();

    bool UpdateTreeTemplates(QTreeWidget *_tree, drash::CApp *_app);

    void UpdateTreeSceneObjects();

    drash::CTimer mTimer;

    void AddFigure();
    void SetObjectParams(const drash::CSceneObjectParams &_params);
    drash::CSceneObjectParams GetObjectParams() const;

  private:
    QHBoxLayout *mLayoutForScene;

  public:
};

#endif // EDITORWINDOW_H
