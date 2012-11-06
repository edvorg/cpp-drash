#include <QApplication>
#include "gamewindow.h"
#include "editor/editorwindow.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[])
{
    srand( time(NULL) );

    QApplication a(argc, argv);

    GameWindowParams p;
    p.SetCommandLine( argc, argv );
    QMainWindow *win;
    if ( p.mArgv.size() > 1) {
        if (p.mArgv[1] != "--editor") {
            win = new GameWindow();
            if ( dynamic_cast<GameWindow*>(win)->Init(p) == false )
            {
                return 0;
            }
            win->showFullScreen();
        } else {
            win = new EditorWindow();
            win->show();
        }
    }
    //EditorWindow w;
    //w.show();
//    QMainWindow *w;s
//    if ( argc > 2 ) {
//        if (strcmp(argv[0],"--editor") == 0) {
//            w = new GameWindow();
//        } else {
//            w = new EditorWindow();
//        }
//    }
//    w.show();
    //w.showFullScreen();
    int res = a.exec();
    delete win;
    return res;
}
