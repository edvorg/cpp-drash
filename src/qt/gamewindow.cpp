// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2014 Edward Knyshov, Yuriy Shatilin.

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
#include "../app/app.h"
#include "../test/test.h"

using namespace drash;

void GameWindowParams::SetCommandLine(unsigned int _argc, char* _argv[]) {
    argv.resize(_argc);

    for (unsigned int i = 0; i < _argc; i++) {
        argv[i] = _argv[i];
    }
}

GameWindow::GameWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::GameWindow), sceneWidget(nullptr) {
    ui->setupUi(this);

    statusLabel = new QLabel(QString(""));
    ui->statusBar->addWidget(statusLabel);

    setCentralWidget(sceneWidget = new SceneWidget(this));

    sceneWidget->setFocus();

    gameTimer.Reset(true);
}

GameWindow::~GameWindow() {
    disconnect(&updateTimer);

    if (app != nullptr) {
        delete app;
        app = nullptr;
    }

    delete ui;
}

bool GameWindow::Init(const GameWindowParams& _params) {
    SceneParams params;
    params.gravity.Set(0.0f, -9.8f);

    QString title = "drash";

    for (unsigned int i = 0; i < _params.argv.size(); i++) {
        if (_params.argv[i] == "--test") {
            if (i + 1 < _params.argv.size()) {
                app = test::StartApp(_params.argv[i + 1].c_str());

                if (app == nullptr) {
                    LOG_ERR("App::App(): test app "
                            << _params.argv[i + 1].c_str() << " not found");
                    return false;
                }

                title += " - ";
                title += _params.argv[i + 1].c_str();

                app->SetQuitHandler([this]() {
                    disconnect(&updateTimer);
                    QApplication::quit();
                });

                break;
            } else {
                LOG_ERR("App::App(): test app name expected");
                return false;
            }
        }
    }

    sceneWidget->SetApp(app);

    this->setWindowTitle(title);

    updateTimer.start(0);
    connect(&updateTimer, SIGNAL(timeout()), this, SLOT(UpdateScene()));

    return true;
}

void GameWindow::UpdateScene() {
    if (sceneWidget->GetApp() == nullptr) {
        disconnect(&updateTimer);
        QApplication::quit();
    }

    gameTimer.Tick();
    if (app != nullptr) {
        app->Step(gameTimer.GetDeltaTime());
    }

    statusLabel->setText(QString(drash::Logger::Tail().c_str()));

    sceneWidget->updateGL();
}
