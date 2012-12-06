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

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include "cscene.h"
#include "sceneobjects.h"
#include "test/test.h"
#include "scenewidget.h"
#include "diag/ctimer.h"

namespace Ui
{
class GameWindow;
}

class GameWindowParams
{
public:
    void SetCommandLine( unsigned int _argc, char *_argv[] );

    std::vector<std::string> mArgv;
};

class GameWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

public slots:
    void UpdateScene();

public:
    bool Init( const GameWindowParams &_params );
    
private:
    bool mInitialized = false;
    Ui::GameWindow *ui = nullptr;
    drash::CApp *mApp = nullptr;
    SceneWidget *mSceneWidget = nullptr;
    QTimer timer;
    drash::CTimer mTimer;
    QLabel *mStatusLabel = nullptr;
};

#endif // GAMEWINDOW_H
