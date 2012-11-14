#include "ctestapp4.h"

#include <sceneobjects.h>

namespace drash
{

CTestApp4::CTestApp4()
{
}

bool CTestApp4::Init()
{
    if (CTestApp::Init() == false)
    {
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
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-2, 1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(2, 1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(1, -1));
        t->mGeometry.mFigures[0].mVertices.push_back(CVec2(-1, -1));
    }

    mTimer.Reset(true);

    return true;
}

void CTestApp4::Update()
{
    CTestApp::Update();

    mTimer.Tick();

    if (mTimer.GetFullTime() >= 1.0)
    {
        CSceneObjectParams params;
        params.mDynamic = true;
        params.mPos.RandX(-20, 20);
        params.mPos.y = 0;

        GetTemplateSystem().CreateSceneObjectFromTemplate(rand()%2 == 0 ? "name1" : "name2", params);

        mTimer.Reset(true);
    }
}

}// namespace drash
