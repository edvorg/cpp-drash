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

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "../diag/timer.h"

class SceneWidget;
class QLabel;

namespace drash
{
    class CApp;
}

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
    Ui::GameWindow *ui = nullptr;
    drash::CApp *mApp = nullptr;
    SceneWidget *mSceneWidget = nullptr;
    QTimer mUpdateTimer;
    drash::CTimer mGameTimer;
    QLabel *mStatusLabel = nullptr;
};

#endif // GAMEWINDOW_H
