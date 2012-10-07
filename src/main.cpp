#include <QApplication>
#include "gamewindow.h"

int main(int argc, char *argv[])
{
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
