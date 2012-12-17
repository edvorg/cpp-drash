#include "test5.h"

#include "../debugdrawsystem/camera.h"

namespace drash
{

namespace test
{

bool CTest5::Init()
{
    if (CApp::Init() == false)
    {
        return false;
    }

    CCamera *cam = GetDebugDrawSystem().GetActiveCam();
    cam->GetPos().Set(CVec3f(0, 10, 50));

    ////////////////
    /// platform ///

    CSceneObjectGeometry* g1 = GetTemplateSystem().CreateSceneObjectTemplate("platform");
    g1->mFigures.resize(1);
    g1->mFigures[0].mDepth = 10;
    g1->mFigures[0].mVertices.resize(4);
    g1->mFigures[0].mVertices[0].Set(-10, -0.5);
    g1->mFigures[0].mVertices[1].Set(10, -0.5);
    g1->mFigures[0].mVertices[2].Set(10, 0.5);
    g1->mFigures[0].mVertices[3].Set(-10, 0.5);

    CSceneObjectParams p1;
    p1.mDynamic = false;

    GetScene().CreateObject<CSceneObject>(*g1, p1);

    ///////////
    /// box ///

    CSceneObjectGeometry* g2 = GetTemplateSystem().CreateSceneObjectTemplate("box");
    g2->mFigures.resize(1);
    g2->mFigures[0].mDepth = 1;
    g2->mFigures[0].mVertices.resize(4);
    g2->mFigures[0].mVertices[0].Set(-0.5, -0.5);
    g2->mFigures[0].mVertices[1].Set(0.5, -0.5);
    g2->mFigures[0].mVertices[2].Set(0.5, 0.5);
    g2->mFigures[0].mVertices[3].Set(-0.5, 0.5);

    CSceneObjectParams p2;
    p2.mPos.Set(0, 10);

    GetScene().CreateObject<CSceneObject>(*g2, p2);

    //////////////////////////////
    /// destruction graph test ///

    const int figures = 10;

    CSceneObjectGeometry* g3 = GetTemplateSystem().CreateSceneObjectTemplate("test_obj");
    g3->mFigures.resize(figures);

    ////////////////
    /// vertices ///

    for (int i = 0; i < figures; i++)
    {
        int index = i - figures / 2;
        g3->mFigures[i].mDepth = 1;
        g3->mFigures[i].mVertices.resize(4);
        g3->mFigures[i].mVertices[0].Set(-0.5 + index, -0.5);
        g3->mFigures[i].mVertices[1].Set(0.5 + index, -0.5);
        g3->mFigures[i].mVertices[2].Set(0.5 + index, 0.5);
        g3->mFigures[i].mVertices[3].Set(-0.5 + index, 0.5);
    }

    /////////////////////////
    /// destruction graph ///

    g3->mDestructionGraph.resize(figures);
    for (unsigned int i = 1; i < figures; i++)
    {
        g3->mDestructionGraph[i-1].push_back(i);
        g3->mDestructionGraph[i].push_back(i-1);
    }

    //////////////
    /// create ///

    CSceneObjectParams p3;
    p3.mPos.Set(0, 30);

    GetScene().CreateObject<CSceneObject>(*g3, p3);

    return true;
}

} // namespace test
} // namespace drash
