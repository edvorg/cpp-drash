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
};

#endif // GAMEWINDOW_H
