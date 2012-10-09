#include "gamewindow.h"
#include "ui_gamewindow.h"

using namespace drash;

void GameWindowParams::SetCommandLine(unsigned int _argc, char *_argv[])
{
    mArgv.resize(_argc);

    for( unsigned int i = 0 ; i < _argc ; i++ )
    {
        mArgv[i] = _argv[i];
    }
}

GameWindow::GameWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameWindow),
    mInitialized(false),
    mCamera(NULL),
    mTestApp(NULL),
    mSceneWidget(NULL)
{
    ui->setupUi(this);

    setCentralWidget( mSceneWidget = new SceneWidget(this) );

    mTimer.SetPaused(false);

    mSceneWidget->setFocus();
}

GameWindow::~GameWindow()
{
    if (!mInitialized)
    {
        return;
    }

    if ( mTestApp )
    {
        mTestApp->Release();
        delete mTestApp;
        mTestApp = NULL;
    }

    if ( mCamera != NULL )
    {
        mScene.DestroyObject(mCamera);
        mCamera = NULL;
    }

    mScene.Release();

    delete ui;
}

bool GameWindow::Init( const GameWindowParams &_params )
{
    CSceneParams params;
    params.mGravity.Set( 0.0f, -9.8f );

    if ( mScene.Init(params) == false )
    {
        return false;
    }

    CCameraParams p;
    mCamera = mScene.CreateObject< CCamera >(p);

    for ( unsigned int i=0; i<_params.mArgv.size(); i++ )
    {
        if ( _params.mArgv[i] == "--test" )
        {
            if ( i+1 < _params.mArgv.size() )
            {
                mTestApp = CTestApp::StartApp( _params.mArgv[i+1].c_str() );

                if ( mTestApp == NULL )
                {
                    LOG_ERR("CApp::Init(): test app "<<_params.mArgv[i+1].c_str()<<" not found");
                    mScene.DestroyObject(mCamera);
                    mCamera = NULL;
                    mScene.Release();
                    return false;
                }

                if ( mTestApp->Init( &mScene, mCamera ) == false )
                {
                    mScene.DestroyObject(mCamera);
                    mCamera = NULL;
                    mScene.Release();
                    return false;
                }

                break;
            }
            else
            {
                LOG_ERR("CApp::Init(): test app name expected");
                mScene.DestroyObject(mCamera);
                mCamera = NULL;
                mScene.Release();
                return false;
            }
        }
    }

    mSceneWidget->SetScene(&mScene);
    mSceneWidget->SetCamera(mCamera);
    mSceneWidget->SetTestApp(mTestApp);

    mInitialized = true;    

    timer.start(1);
    connect( &timer, SIGNAL( timeout() ), this, SLOT( UpdateScene() ) );

    return true;
}

bool GameWindow::UpdateScene()
{
    mTimer.Tick();
    printf("%f\n", (float)mTimer.GetDeltaTime());
    mScene.Step( mTimer.GetDeltaTime() );

    if ( mTestApp != NULL )
    {
        mTestApp->Update();
    }

    mSceneWidget->updateGL();
}
