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

#include <QApplication>
#include "gamewindow.h"
#include "editor/editorwindow.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[])
{
    srand( time(nullptr) );

    QApplication a(argc, argv);

    // look for --editor key

    bool editor = false;

    for (int i=0; i<argc; i++)
    {
        if (strcmp(argv[i], "--editor") == 0)
        {
            editor = true;
            break;
        }
    }

    // allocate and init editor or game window

    QMainWindow *w = nullptr;

    if (editor)
    {
        w = new EditorWindow();
    }
    else
    {
        GameWindowParams p;
        p.SetCommandLine( argc, argv );
        w = new GameWindow();
        if (reinterpret_cast<GameWindow*>(w)->Init(p) == false)
        {
            delete w;
            w = nullptr;
        }
    }

    // show window and run game loop

    int res = 1;

    if (w != nullptr)
    {
        w->show();
        res = a.exec();
        delete w;
    }

    return res;
}
