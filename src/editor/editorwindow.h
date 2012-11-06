#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
using namespace drash;
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


};

#endif // EDITORWINDOW_H
