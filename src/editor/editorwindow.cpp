#include "editorwindow.h"
#include "ui_editorwindow.h"


EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow)
{
    ui->setupUi(this);
    CreateActions();
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
    mApp = drash::StartApp("test1");
    if (mApp == NULL) {
        return false;
    }

    if (mApp->Init() == false) {
        return false;
    }

    ui->mScene->SetTestApp(mApp);
    return true;
}

void EditorWindow::timerEvent(QTimerEvent *)
{
    mTimer.Tick();

    if (mApp != nullptr)
    {
        mApp->Step(mTimer.GetDeltaTime());
    }

    ui->mScene->updateGL();
}

void EditorWindow::CreateActions()
{
    mQuit = new QAction("Quit",this);
    mQuit->setShortcut(tr("Ctrl+Q"));
    this->addAction(mQuit);
    connect(mQuit,SIGNAL(triggered()),
            this,SLOT(close()));
}


void EditorWindow::on_pushButton_clicked()
{
    this->ui->mScene->StartBuildObject();
}

void EditorWindow::on_pushButton_2_clicked()
{
    ui->mScene->BuildNow();
}
