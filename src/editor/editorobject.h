#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <vector>


#include "../scenewidget.h"
#include "../misc/cvec2.h"

// TODO: temp Testing build object
class EditorObject : public SceneWidget
{
public:
    explicit EditorObject(QWidget * _parent = NULL);

    virtual void mousePressEvent( QMouseEvent * _event );

    void StartBuildObject();

    bool BuildNow();

private:
    enum ModeWork{Edit, Create};
    bool mStartBuild;
    ModeWork mMode;
//    bool mEditObject;

    std::vector<drash::CVec2> mVertexs;
public slots:
};

#endif // EDITORWIDGET_H
