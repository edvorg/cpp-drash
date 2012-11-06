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

    if ( p.mArgv.size() > 2) {
        if (p.mArgv.at(1) != "--editor") {
            GameWindow w;
            if ( w.Init(p) == false )
            {
                return 0;
            }
            w.showFullScreen();
        } else {
            EditorWindow w;
            w.show();
        }
    }

//    QMainWindow *w;
//    if ( argc > 2 ) {
//        if (strcmp(argv[0],"--editor") == 0) {
//            w = new GameWindow();
//        } else {
//            w = new EditorWindow();
//        }
//    }
//    w.show();
    //w.showFullScreen();
    
    return a.exec();
}
