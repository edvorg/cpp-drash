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

    CSceneObjectParams p1;
    p1.mDynamic = false;
    p1.mPos.Set(0, -50);
    p1.mFigures.resize(1);
    p1.mFigures[0].mLayers.Set(-100,100);
    p1.mFigures[0].mVertices.push_back( CVec2(-100, -10) );
    p1.mFigures[0].mVertices.push_back( CVec2(100, -10) );
    p1.mFigures[0].mVertices.push_back( CVec2(100, 10) );
    p1.mFigures[0].mVertices.push_back( CVec2(-100, 10) );
    GetScene().CreateObject<CSceneObject>(p1);

    CPlayerParams p2;
    p2.mFigures.resize(1);
    p2.mFigures[0].mLayers.Set(-100,100);
    GetPlayersSystem().AddPlayer(p2);

    return true;
}

}// namespace drash
