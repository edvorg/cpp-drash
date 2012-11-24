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
    mInitialized(false),
    ui(new Ui::GameWindow),
    mSceneWidget(NULL)
{
    ui->setupUi(this);

    setCentralWidget( mSceneWidget = new SceneWidget(this) );

    mSceneWidget->setFocus();

    mTimer.Reset(true);
}

GameWindow::~GameWindow()
{
    if (!mInitialized)
    {
        return;
    }

    if (mApp != nullptr)
    {
        mApp->Release();
        delete mApp;
        mApp = nullptr;
    }

    delete ui;
}

bool GameWindow::Init( const GameWindowParams &_params )
{
    CSceneParams params;
    params.mGravity.Set( 0.0f, -9.8f );

    for ( unsigned int i=0; i<_params.mArgv.size(); i++ )
    {
        if ( _params.mArgv[i] == "--test" )
        {
            if ( i+1 < _params.mArgv.size() )
            {
                mApp = StartApp( _params.mArgv[i+1].c_str() );

                if (mApp == nullptr)
                {
                    LOG_ERR("CApp::Init(): test app "<<_params.mArgv[i+1].c_str()<<" not found");
                    return false;
                }

                if (mApp->Init() == false)
                {
                    return false;
                }

                break;
            }
            else
            {
                LOG_ERR("CApp::Init(): test app name expected");
                return false;
            }
        }
    }

    mSceneWidget->SetTestApp(mApp);

    mInitialized = true;    

    timer.start(0);
    connect( &timer, SIGNAL( timeout() ), this, SLOT( UpdateScene() ) );

    return true;
}

void GameWindow::UpdateScene()
{
    mTimer.Tick();
    if (mApp != nullptr)
    {
        mApp->Step(mTimer.GetDeltaTime());
    }

    mSceneWidget->updateGL();
}
