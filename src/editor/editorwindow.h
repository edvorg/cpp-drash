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

#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include "../diag/timer.h"
#include <QActionGroup>

class QTreeWidgetItem;
class QLabel;

namespace drash
{
    class CObjectEditorApp;
    class CApp;
}

namespace Ui
{
    class EditorWindow;
}

class QStatusBar;

class EditorWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();
    
private slots:

    void on_mTreeObjects_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_mTreeObjects_itemClicked(QTreeWidgetItem *item, int column);

    void on_mTreeObjects_itemSelectionChanged();

    void Remove_Object();

    void SaveObject();
private:
    Ui::EditorWindow *ui;
    bool InitScene();
    drash::CObjectEditorApp  *mObjectApp = nullptr;

    drash::CApp * mCurrentApp = nullptr;

    void timerEvent(QTimerEvent *) override;

    //drash::CSceneObject * mCurrentObject = nullptr;

    // Actions
private:
    QAction *mQuit;
    QAction *mRemoveAction;
    QAction *mSaveAction;
    QAction *mMoveActiveAction;
    QAction *mStretchActiveAction;
    QAction *mNewFigureAction;
    QAction *mNewObjectAction;
    QAction *mZoomUpAction;
    QAction *mZoomDownAction;

    QActionGroup mModeActions;

    // Slots for Actions
private slots:
    void CreateNewObject();
    void AddNewFigure();
    void MoveActive();
    void StretchActive();
    void ZoomUp();
    void ZoomDown();
    // GuiObjects
private:
    QLabel *mLabelOfStatusBar;

private:
    void CreateActions();

    bool UpdateTreeObject();

    drash::CTimer mTimer;

    void AddFigure();
public:

};

#endif // EDITORWINDOW_H
