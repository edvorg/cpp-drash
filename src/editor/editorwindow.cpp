#include "editorwindow.h"
#include "ui_editorwindow.h"


EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("DRASH Editor");
    this->InitScene();

    this->startTimer(0);
}



EditorWindow::~EditorWindow()
{
    delete ui;
}

bool EditorWindow::InitScene()
{
    drash::CSceneParams params;
    params.mGravity.Set( 0.0f, -9.8f );
    mTestApp = drash::StartApp("test3");
    if (mTestApp == NULL) {
        return false;
    }

    if (mTestApp->Init() == false) {
        return false;
    }

    ui->mScene->SetTestApp(mTestApp);

    return true;
}

void EditorWindow::timerEvent(QTimerEvent *)
{
    if (mTestApp != NULL) {
        mTestApp->Update();
    }
    ui->mScene->updateGL();
}

