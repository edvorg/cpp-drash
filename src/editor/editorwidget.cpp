#include "editorwidget.h"

using namespace drash;

EditorWidget::EditorWidget(QWidget *parent):
    SceneWidget(parent),
    mStartBuild(false)
{

}

void EditorWidget::mousePressEvent(QMouseEvent *_event)
{
    if (mStartBuild == true) {
        CVec2 pos = TranslateToLocalCoordinate( _event->posF().x(),
                                    _event->posF().y(),
                                    mTestApp->GetCamera()->GetZoom(),
                                    mTestApp->GetCamera()->GetZoom(),
                                    mHeight,
                                    mWidth);
        mVertexs.push_back(pos);
    }
}

void EditorWidget::StartBuildObject()
{
    mStartBuild = true;
}

bool EditorWidget::BuildNow()
{
    if (mStartBuild == false ||
            mVertexs.empty()) {
        return false;
    }
    mStartBuild = false;
    CSceneObjectParams sbp;
    sbp.mDynamic = false;
    sbp.mFigures.resize(1);
    sbp.mFigures[0].mLayers.Set( -2000, 2000 );
    sbp.mFigures[0].mRestitution = 0.0;
    sbp.mAngle = 0;
    sbp.mPos.Set(0,0);
    sbp.mFigures[0].mVertices = mVertexs;
    mVertexs.clear();
    mTestApp->GetScene().CreateObject<CSceneObject>(sbp);
//    for (auto iter = mVertexs.begin() ; iter != mVertexs.end() ; iter++) {

//    }
//    sbp.mFigures[0].mVertices.push_back( CVec2( -300, -5 ) );
//    sbp.mFigures[0].mVertices.push_back( CVec2( 300, -5 ) );
//    sbp.mFigures[0].mVertices.push_back( CVec2( 300, 5 ) );
//    sbp.mFigures[0].mVertices.push_back( CVec2( -300, 5 ) );
    return true;
}

//CVec2 PointSDLToWorldPoint( unsigned int _x,
//                            unsigned int _y,
//                            float _zoom,
//                            const CVec2 & _posCamera,
//                            unsigned  int _height,
//                            unsigned int _width )
//{
//    CVec2 res;
//    res.x =  ( -(float)_width / _zoom ) / 2.0f + (float)_x / _zoom;
//    res += _posCamera;
//    return res;
//}

CVec2 EditorWidget::TranslateToLocalCoordinate(const float _x,
                                               const float _y,
                                               float _zoom,
                                               const CVec2 & _posCamera,
                                               unsigned  int _height,
                                               unsigned int _width){
    CVec2 res;
    res.x = ( -(float)_width / _zoom ) / 2.0f + (_x + (float)_width/2 ) / _zoom;
    res.y = - (_y + (float)_height/2) / _zoom + ( (float)_height / _zoom ) / 2.0f;
    return res;
}
