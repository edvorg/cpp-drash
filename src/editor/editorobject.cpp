#include "editorobject.h"

using namespace drash;

EditorObject::EditorObject(QWidget *_parent):
    SceneWidget(_parent),
    mStartBuild(false)
{
    mMode = Edit;
    connect(this,SIGNAL(CreateNewObject()),
            this,SLOT(RemoveObjects()));
}

EditorObject::~EditorObject()
{
}

void EditorObject::mousePressEvent(QMouseEvent *_event){
    QWidget::mousePressEvent(_event);
    if (mStartBuild == true) {
        auto cam = mTestApp->GetCamera();
        CVec2 pos = this->ScreenSpaceToWorldSpace(CVec2(_event->x(),
                                                        _event->y()),
                                                        cam->m_ZoomMax -
                                                  cam->GetZoom());

        mVertexs.push_back(pos);
    }
}

void EditorObject::StartBuildObject(){
    mMode = Create;
    mStartBuild = true;
    mVertexs.clear();
    emit this->CreateNewObject();
}

bool EditorObject::BuildNow(){
    if (mStartBuild == false ||
            mVertexs.empty()) {
        return false;
    }
    mStartBuild = false;
    CSceneObjectGeometry sbg;
    sbg.mFigures.resize(1);
    sbg.mFigures[0].mLayers.Set( -2000, 2000 );
    sbg.mFigures[0].mRestitution = 0.0;
    sbg.mFigures[0].mVertices = mVertexs;
    CSceneObjectParams sbp;
    sbp.mDynamic = false;
    sbp.mAngle = 0;
    sbp.mPos.Set(0,0);
    mVertexs.clear();
    mTestApp->GetScene().CreateObject<CSceneObject>(sbg, sbp);
//    for (auto iter = mVertexs.begin() ; iter != mVertexs.end() ; iter++) {
//    }
//    sbp.mFigures[0].mVertices.push_back( CVec2( -300, -5 ) );
//    sbp.mFigures[0].mVertices.push_back( CVec2( 300, -5 ) );
//    sbp.mFigures[0].mVertices.push_back( CVec2( 300, 5 ) );
//    sbp.mFigures[0].mVertices.push_back( CVec2( -300, 5 ) );
    return true;
}
