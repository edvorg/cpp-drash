#include <QApplication>
#include "gamewindow.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char *argv[])
{
    srand( time(NULL) );

    QApplication a(argc, argv);

    GameWindowParams p;
    p.SetCommandLine( argc, argv );

    GameWindow w;
    if ( w.Init(p) == false )
    {
        return 0;
    }
    w.show();
    
    return a.exec();
}
