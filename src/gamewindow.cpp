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
    mTestApp(NULL),
    mSceneWidget(NULL)
{
    ui->setupUi(this);

    setCentralWidget( mSceneWidget = new SceneWidget(this) );

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
                mTestApp = StartApp( _params.mArgv[i+1].c_str() );

                if ( mTestApp == NULL )
                {
                    LOG_ERR("CApp::Init(): test app "<<_params.mArgv[i+1].c_str()<<" not found");
                    return false;
                }

                if ( mTestApp->Init() == false )
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

    mSceneWidget->SetTestApp(mTestApp);

    mInitialized = true;    

    timer.start(1);
    connect( &timer, SIGNAL( timeout() ), this, SLOT( UpdateScene() ) );

    return true;
}

void GameWindow::UpdateScene()
{
    if ( mTestApp != NULL )
    {
        mTestApp->Update();
    }

    mSceneWidget->updateGL();
}
