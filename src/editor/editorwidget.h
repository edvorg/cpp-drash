#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <vector>


#include "../scenewidget.h"
#include "../misc/cvec2.h"

// TODO: temp Testing build object
class EditorWidget : public SceneWidget
{
public:
    explicit EditorWidget(QWidget *parent = NULL);

    virtual void mousePressEvent( QMouseEvent * _event );

    void StartBuildObject();

    bool BuildNow();
    static drash::CVec2 TranslateToLocalCoordinate(const float _x,
                                                   const float _y,
                                                   float _zoom,
                                                   const drash::CVec2 & _posCamera,
                                                   unsigned  int _height,
                                                   unsigned int _width );
private:
    //enum TypeEditor{EditObject, EditScene};

    bool mStartBuild;

    std::vector<drash::CVec2> mVertexs;
public slots:
};

#endif // EDITORWIDGET_H
