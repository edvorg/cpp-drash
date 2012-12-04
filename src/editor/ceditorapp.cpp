#include "ceditorapp.h"

namespace drash {

bool CObjectEditorApp::Init()
{
    if (CApp::Init() == false) {
        return false;
    }

    auto t = GetTemplateSystem().CreateSceneObjectTemplate("name1");
    if (t != nullptr)
    {
        t->mGeometry.mFigures.resize(1);
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-2, -1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(2, -1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(1, 1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-1, 1));
    }
    t = GetTemplateSystem().CreateSceneObjectTemplate("name2");
    if (t != nullptr)
    {
        t->mGeometry.mFigures.resize(1);
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(2, 1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-2, 1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-1, -1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(1, -1));
    }
    t = GetTemplateSystem().CreateSceneObjectTemplate("ground");
    if (t != nullptr)
    {
        t->mGeometry.mFigures.resize(1);
        t->mGeometry.mFigures[0].mDepth = 10;
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(50, 5));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-50, 5));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-50, -5));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(50, -5));
    }
    SetProcessor();

    return true;
}

void CObjectEditorApp::Step(double _dt)
{
    CApp::Step(_dt);
}

void CObjectEditorApp::Render()
{
    CApp::Render();
    if (mVertexs.size() != 0) {
        for (unsigned int i = 1 ; i < mVertexs.size() ; i++) {
            GetDebugDrawSystem().DrawLine(mVertexs[i-1],mVertexs[i],b2Color(0,255,0));
        }
        GetDebugDrawSystem().DrawLine(mVertexs[mVertexs.size() -1 ],GetCursorPos(),b2Color(0,255,0));
    }
//    GetDebugDrawSystem().DrawLine();

}

void CObjectEditorApp::StartBuild()
{
    mBuildStart = true;
}

void CObjectEditorApp::SetProcessor()
{
    GetEventSystem().SetProcessor("LB", CAppEventProcessor(
    [this] ()
    {
        //LOG_INFO("Click !!!");
        if (mBuildStart == true) {
            mVertexs.push_back(GetCursorPos());
        }
    }
    ));
}

bool CObjectEditorApp::BuildObject(std::string _objectName)
{
    if (mVertexs.size() < 3) {
        return false;
    }
    if (ValidateFigure() == false) {
        LOG_ERR("This figure can't build");
        return false;
    }

    auto obj = GetTemplateSystem().FindTemplate(_objectName);
    if (obj == nullptr) {
        return false;
    }
    CFigureParams param;
    std::for_each(mVertexs.begin() , mVertexs.end() , [this] (CVec2 &v)
    {
        GetDebugDrawSystem().ScreenSpaceToWorldSpace(v,
                 -GetDebugDrawSystem().GetActiveCam()->GetZ().Get());
    });
    param.mVertices = mVertexs;
    obj->mGeometry.mFigures.push_back(param);

    mBuildStart = false;
    mVertexs.clear();
    return true;
}

void CObjectEditorApp::AddNewObjectToTemplate(std::string _name)
{
    GetTemplateSystem().CreateSceneObjectTemplate(_name);
}


// TODO: Implements this!
bool CObjectEditorApp::ValidateFigure()
{
    return true;
}


}// namespace drash
