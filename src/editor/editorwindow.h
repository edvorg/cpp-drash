#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include "../test/ctestapp.h"
#include "../test/test.h"
namespace Ui {
class EditorWindow;
}

class EditorWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::EditorWindow *ui;
    bool InitScene();
    drash::CTestApp  *mTestApp;

    void timerEvent(QTimerEvent *);

    // Actions
private:
    QAction *mQuit;
private:
    void CreateActions();
};

#endif // EDITORWINDOW_H
