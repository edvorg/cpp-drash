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

    CSceneObjectGeometry g1;
    g1.mFigures.resize(1);
    g1.mFigures[0].mLayers.Set(-100,100);
    g1.mFigures[0].mVertices.push_back( CVec2(-100, -10) );
    g1.mFigures[0].mVertices.push_back( CVec2(100, -10) );
    g1.mFigures[0].mVertices.push_back( CVec2(100, 10) );
    g1.mFigures[0].mVertices.push_back( CVec2(-100, 10) );
    CSceneObjectParams p1;
    p1.mDynamic = false;
    p1.mPos.Set(0, -50);
    GetScene().CreateObject<CSceneObject>(g1, p1);

    CSceneObjectGeometry g2;
    g2.mFigures.resize(1);
    g2.mFigures[0].mLayers.Set(-100,100);
    CPlayerParams p2;
    GetPlayersSystem().AddPlayer(g2, p2);

    return true;
}

}// namespace drash
