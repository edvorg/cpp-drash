#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include "cscene.h"
#include "sceneobjects.h"
#include "test/ctestapp.h"
#include "scenewidget.h"

namespace Ui {
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
    bool UpdateScene();

public:
    bool Init( const GameWindowParams &_params );
    
private:
    Ui::GameWindow *ui;
    drash::CScene mScene;
    drash::CCamera *mCamera;
    drash::CTestApp *mTestApp;
    bool mInitialized;
    SceneWidget *mSceneWidget;
    QTimer timer;
    drash::CTimer mTimer;
};

#endif // GAMEWINDOW_H
