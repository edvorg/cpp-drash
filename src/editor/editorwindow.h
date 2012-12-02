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

#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include "../app/app.h"
#include "../test/test.h"
#include "../diag/ctimer.h"
#include "ceditorapp.h"

class QTreeWidgetItem;
class QLabel;

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

    void on_mNewObjectButton_clicked();

    void on_mBuildButton_clicked();

    void on_mNewFigureButton_clicked();

private:
    Ui::EditorWindow *ui;
    bool InitScene();
    drash::CEditorApp  *mApp;

    void timerEvent(QTimerEvent *);

    drash::CSceneObject * mCurrentObject = nullptr;

    // Actions
private:
    QAction *mQuit;

    // GuiObjects
private:
    QLabel *mLabelOfStatusBar;
private:
    void CreateActions();
    bool UpdateTreeObject();

    void RemoveCurrentObject();

    drash::CTimer mTimer;

public:
    void AddFigure(const std::vector<drash::CVec2> &_vertexs);

};

#endif // EDITORWINDOW_H
