#include "test10.h"

#include "../greng/camera.h"
#include "../scene/sceneobject.h"
#include "../scene/figure.h"

namespace drash
{

namespace test
{

bool CTest10::Init()
{
    if (CApp::Init() == false ||
        InitCamera() == false ||
        InitObjects() == false ||
        InitLights() == false ||
        InitProcessors() == false)
    {
        return false;
    }

    GetDebugRenderer().SetTexCoordsScale(0.2);

    return true;
}

void CTest10::Step(double _dt)
{
    CApp::Step(_dt);

    mLightPoint1.mPosition = mLamp1Object->GetPos();
    mSpotLight.mPosition = mLamp1Object->GetPos();

    float angle = mLamp1Object->GetAngle() + M_PI * 1.5;

    mSpotLight.mDirection.Set(cos(angle), sin(angle), 0);
}

void CTest10::Render()
{
    CApp::Render();

    CVec3f p1(mLamp1Object->GetWorldPoint(CVec2f(0, 0.5)), mLamp1Object->GetPosZ());
    GetGrengSystems().GetRenderer().DrawLine(mCamera,
                                             p1,
                                             mWallTopObject->GetPos(),
                                             2,
                                             CColor4f(0, 0, 0, 1),
                                             true);
}

bool CTest10::InitCamera()
{
    greng::CCameraParams params;
    params.mPos.Set(0, 2, 20);
    params.mRotation.mX = - M_PI / 18.0;

    greng::CCamera *cam = GetGrengSystems().GetCameraManager().CreateCamera(params);

    if (cam == nullptr)
    {
        return false;
    }

    GetDebugRenderer().SetCamera(cam);

    mCamera = cam;

    return true;
}

bool CTest10::InitObjects()
{
    CSceneObjectGeometry *g1 = GetTemplateSystem().CreateSceneObjectTemplate("wall_back");
    CSceneObjectGeometry *g2 = GetTemplateSystem().CreateSceneObjectTemplate("wall_top");
    CSceneObjectGeometry *g3 = GetTemplateSystem().CreateSceneObjectTemplate("wall_bottom");
    CSceneObjectGeometry *g4 = GetTemplateSystem().CreateSceneObjectTemplate("wall");
    CSceneObjectGeometry *g5 = GetTemplateSystem().CreateSceneObjectTemplate("lamp");

    if (g1 == nullptr ||
        g2 == nullptr ||
        g3 == nullptr ||
        g4 == nullptr ||
        g5 == nullptr)
    {
        return false;
    }

    g1->mFigures.resize(1);
    g1->mFigures[0].mDepth = 0.5;
    g1->mFigures[0].mVertices.push_back(CVec2f(-10, 5));
    g1->mFigures[0].mVertices.push_back(CVec2f(-10, -5));
    g1->mFigures[0].mVertices.push_back(CVec2f(10, -5));
    g1->mFigures[0].mVertices.push_back(CVec2f(10, 5));

    g2->mFigures.resize(1);
    g2->mFigures[0].mDepth = 8;
    g2->mFigures[0].mVertices.push_back(CVec2f(-10, 0.2));
    g2->mFigures[0].mVertices.push_back(CVec2f(-10, -0.2));
    g2->mFigures[0].mVertices.push_back(CVec2f(10, -0.2));
    g2->mFigures[0].mVertices.push_back(CVec2f(10, 0.2));

    g3->mFigures.resize(1);
    g3->mFigures[0].mDepth = 3;
    g3->mFigures[0].mVertices.push_back(CVec2f(-10, 0.2));
    g3->mFigures[0].mVertices.push_back(CVec2f(-10, -0.2));
    g3->mFigures[0].mVertices.push_back(CVec2f(10, -0.2));
    g3->mFigures[0].mVertices.push_back(CVec2f(10, 0.2));

    g4->mFigures.resize(1);
    g4->mFigures[0].mDepth = 3;
    g4->mFigures[0].mVertices.push_back(CVec2f(-0.4, 4.0));
    g4->mFigures[0].mVertices.push_back(CVec2f(-0.4, -4.0));
    g4->mFigures[0].mVertices.push_back(CVec2f(0.4, -4.0));
    g4->mFigures[0].mVertices.push_back(CVec2f(0.4, 4.0));

    g5->mFigures.resize(1);
    g5->mFigures[0].mDepth = 0.4;
    g5->mFigures[0].mMass = 0.1;
    g5->mFigures[0].mVertices.push_back(CVec2f(-0.2, 0.5));
    g5->mFigures[0].mVertices.push_back(CVec2f(-0.2, -0.5));
    g5->mFigures[0].mVertices.push_back(CVec2f(0.2, -0.5));
    g5->mFigures[0].mVertices.push_back(CVec2f(0.2, 0.5));

    CSceneObjectParams p1;
    p1.mPos.Set(0, 0, -5);
    p1.mDynamic = false;
    GetTemplateSystem().CreateSceneObjectFromTemplate("wall_back", p1);

    CSceneObjectParams p2;
    p2.mPos.Set(0, 5, 0);
    p2.mDynamic = true;
    CSceneObject *o2 = GetTemplateSystem().CreateSceneObjectFromTemplate("wall_top", p2);

    CSceneObjectParams p3;
    p3.mPos.Set(0, -4, 0);
    p3.mDynamic = false;
    GetTemplateSystem().CreateSceneObjectFromTemplate("wall_bottom", p3);

    CSceneObjectParams p4;
    p4.mPos.Set(-9, 0, 0);
    p4.mDynamic = true;
    GetTemplateSystem().CreateSceneObjectFromTemplate("wall", p4);

    CSceneObjectParams p5;
    p5.mPos.Set(9, 0, 0);
    p5.mDynamic = true;
    GetTemplateSystem().CreateSceneObjectFromTemplate("wall", p5);

    CSceneObjectParams p6;
    p6.mPos.Set(-3, 3, 0);
    p6.mDynamic = true;
    CSceneObject *o6 = GetTemplateSystem().CreateSceneObjectFromTemplate("lamp", p6);

    CVec3f anchor1(0, 0, 0);
    anchor1.mY += 0.5;
    CVec3f anchor2(0, 0, 0);
    GetScene().CreateJointRope(o6, o2, anchor1, anchor2, 3);

    mLamp1Object = o6;
    mWallTopObject = o2;

    return true;
}

bool CTest10::InitLights()
{
    mLightPoint1.mPosition.Set(0, 0, 0);

    GetDebugRenderer().SetLight(nullptr);
    GetDebugRenderer().SetSpotLight(&mSpotLight);

    return true;
}

bool CTest10::InitProcessors()
{
    GetEventSystem().SetProcessor("C-q", CAppEventProcessor(
    [this] ()
    {
        this->Quit();
    },
    [] ()
    {
    },
    [] ()
    {
    }));

    return true;
}

} // namespace test

} // namespace drash
