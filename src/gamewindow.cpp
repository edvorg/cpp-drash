// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QLabel>
#include "scenewidget.h"
#include "app/app.h"
#include "test/test.h"

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

    mStatusLabel = new QLabel(QString(""));
    ui->statusBar->addWidget(mStatusLabel);

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

    QString title = "drash";

    for ( unsigned int i=0; i<_params.mArgv.size(); i++ )
    {
        if ( _params.mArgv[i] == "--test" )
        {
            if ( i+1 < _params.mArgv.size() )
            {
                mApp = test::StartApp( _params.mArgv[i+1].c_str() );

                if (mApp == nullptr)
                {
                    LOG_ERR("CApp::Init(): test app "<<_params.mArgv[i+1].c_str()<<" not found");
                    return false;
                }

                if (mApp->Init() == false)
                {
                    return false;
                }

                title += " - ";
                title += _params.mArgv[i+1].c_str();

                mApp->SetQuitHandler([this] ()
                {
                    disconnect(&timer);
                    mSceneWidget->SetTestApp(nullptr);
                    QApplication::quit();
                });

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

    this->setWindowTitle(title);

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

    mStatusLabel->setText(QString(drash::CLogger::Tail().c_str()));

    mSceneWidget->updateGL();
}
