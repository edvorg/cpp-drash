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
    
private:
    Ui::EditorWindow *ui;
    bool InitScene();
    drash::CTestApp  *mTestApp;

    void timerEvent(QTimerEvent *);
};

#endif // EDITORWINDOW_H
