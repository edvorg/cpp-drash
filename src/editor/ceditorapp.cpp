#include "ceditorapp.h"
namespace drash {

bool CEditorApp::Init()
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

    return true;
}

void CEditorApp::Step(double _dt)
{
    CApp::Step(_dt);

}

void CEditorApp::Render()
{
    CApp::Render();

}


}// namespace drash
