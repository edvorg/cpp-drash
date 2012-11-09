#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <vector>


#include "../scenewidget.h"
#include "../misc/cvec2.h"

// TODO: temp Testing build object
class EditorObject : public SceneWidget
{
    Q_OBJECT

public:
    explicit EditorObject(QWidget * _parent = NULL);

    virtual ~EditorObject();

    virtual void mousePressEvent( QMouseEvent * _event );

    void StartBuildObject();

    bool BuildNow();

signals:
    void CreateNewObject();

private:
    enum WorkMode{Edit, Create};
    bool mStartBuild;
    WorkMode mMode;
//    bool mEditObject;

    std::vector<drash::CVec2> mVertexs;
//private slots:
//    void ClearScene();

};

#endif // EDITORWIDGET_H
