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
#include "../test/app.h"
#include "../test/test.h"
#include "../diag/ctimer.h"

namespace Ui
{
    class EditorWindow;
}

class EditorWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::EditorWindow *ui;
    bool InitScene();
    drash::CApp  *mApp;

    void timerEvent(QTimerEvent *);

    // Actions
private:
    QAction *mQuit;
private:
    void CreateActions();
    drash::CTimer mTimer;
};

#endif // EDITORWINDOW_H
