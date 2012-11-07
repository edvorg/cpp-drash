#include <QApplication>
#include "gamewindow.h"
#include "editor/editorwindow.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[])
{
    srand( time(NULL) );

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

    QMainWindow *w = NULL;

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
            w = NULL;
        }
    }

    // show window and run game loop

    int res = 1;

    if (w != NULL)
    {
        w->show();
        res = a.exec();
        delete w;
    }

    return res;
}
