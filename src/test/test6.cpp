// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "test6.h"
#include "../scene/sceneobject.h"
#include "../scene/figure.h"
#include "../players/player.h"
#include "../greng/camera.h"
#include "../scene/scene.h"
#include "../levelmanager/level.h"
#include "../misc/math.h"

namespace drash
{

namespace test
{

bool CTest6::Init()
{
    if (CTest1::Init() == false ||
        InitCamera() == false ||
        InitLevel() == false ||
        InitPlayer() == false ||
        InitLight() == false ||
        InitProcessors() == false ||
        InitTarget() == false)
    {
        return false;
    }

    GetDebugRenderer().SetTexCoordsScale(0.5);

    return true;
}

void CTest6::Step(double _dt)
{
    CTest1::Step(_dt);

    mTargetCreateTimer += _dt;
    mAngle += _dt;

    while (mAngle >= M_PI * 2.0)
    {
        mAngle -= M_PI * 2.0;
    }

    mLight1.mPosition.Set(sin(mAngle) * 40, 20, 0);

    GetDebugRenderer().SetLight(&mLight1);

    CVec2f dir(GetPlayersSystem().GetPlayers()[0]->GetSceneObject()->GetPosXY().Get().mX - mPlayer1OldPos.mX,
               mPlayer1OldPos.mZ - GetPlayersSystem().GetPlayers()[0]->GetSceneObject()->GetPosZ());

    if (dir.LengthSquared() < 0.00001)
    {
        dir.Set(1, 0);
    }
    else
    {
        dir.Normalize();
    }

    float angle = acos(dir.mX * mPlayer1MeshDir.mX + dir.mY * mPlayer1MeshDir.mY);

    if (angle > 0.000001f)
    {
        CVec3f cross;
        Vec3Cross(CVec3f(mPlayer1MeshDir, 0), CVec3f(dir, 0), cross);

        CMatrix4f m;
        if (cross.mZ < 0.0)
        {
            MatrixRotationZ(m, angle * -10.0 * _dt);
        }
        else
        {
            MatrixRotationZ(m, angle * 10.0 * _dt);
        }

        CVec4f new_dir;
        MatrixMultiply(m, CVec4f(mPlayer1MeshDir, 0, 0), new_dir);
        mPlayer1MeshDir = new_dir;

        if (mPlayer1MeshDir.LengthSquared() > 0.00001f)
        {
            mPlayer1MeshDir.Normalize();
        }
    }

    mPlayer1OldPos = mPlayer1->GetSceneObject()->GetPos();

    if (mFollowPlayer == true)
    {
        CVec3f t = GetCamera()->GetPos().GetTarget();
        t.mX = mPlayer1->GetSceneObject()->GetPos().mX;
        GetCamera()->GetPos().SetTarget(t, 1.0, AnimatorBehavior::Single);
    }

    if (mTargetCreateTimer > 5.0 && mTargetDestroyed)
    {
        CSceneObjectParams p;
        p.mPos.Set(math::Rand<float>(-5, 5, 0.5), 10, 0);

        auto o = GetScene().CreateObject(*mTargetGeometry, p);

        mTargetDestroyed = false;
        mTargetCreateTimer = 0.0;

        o->AddDestroyHandler([this] (CSceneObject *)
        {
            mTargetDestroyed = true;
        });
    }
}

void CTest6::Render()
{
    CTest1::Render();

    float new_angle = acos(mPlayer1MeshDir.mX);

    if (mPlayer1MeshDir.mY < 0.0f)
    {
        new_angle *= -1;
    }

    CMatrix4f r;
    MatrixRotationY(r, M_PI * 0.5 + new_angle);

    CMatrix4f s;
    MatrixScale(s, CVec3f(0.4));

    CMatrix4f rot;
    MatrixMultiply(r, s, rot);

    CMatrix4f transl;
    MatrixTranslation(transl, mPlayer1->GetSceneObject()->GetPos() - CVec3f(0, 0.4, 0));

    CMatrix4f model;
    MatrixMultiply(transl, rot, model);

    CMatrix4f model_view;
    MatrixMultiply(GetCamera()->GetViewMatrix(), model, model_view);

    for (unsigned int i = 0; i < 6; i++)
    {
        GetGrengSystems().GetRenderer().RenderMesh(mPlayer1Mesh,
                                 i,
                                 &mPlayer1Texture,
                                 1,
                                 mPlayer1ShaderProgram,
                                 &model,
                                 nullptr,
                                 &model_view,
                                 &GetCamera()->GetProjectionMatrix(),
                                 &mLight1);
    }

    GetGrengSystems().GetRenderer().DrawPoint(GetCamera(),
                            mLight1.mPosition,
                            10,
                            CColor4f(1, 1, 1, 1),
                                              true);
}

bool CTest6::InitCamera()
{
    GetCamera()->GetPos().Set(CVec3f(0, 15, 30));
    GetCamera()->GetRotation().Set(CVec2f(-M_PI / 12.0, 0));

    return true;
}

bool CTest6::InitLevel()
{
    std::map<std::string,CSceneObjectGeometry*> & templates = GetTemplateSystem().GetSceneObjectTemplates();

    for (auto i = templates.begin(); i != templates.end(); i++)
    {
        i->second->ComputeDestructionGraph(0.5);
    }

    CLevel *l = GetLevelManager().CreateLevel();

    if (l == nullptr)
    {
        return false;
    }

    l->Load("demo_level.dlvl");

    return GetLevelManager().StartLevel(l);
}

bool CTest6::InitPlayer()
{
    CSceneObjectGeometry g1;

    g1.mFigures.resize(1);
    g1.mFigures[0].mDepth = 0.8;
    g1.mFigures[0].mVertices.push_back(CVec2f(0, 0.4));
    g1.mFigures[0].mVertices.push_back(CVec2f(-0.4, -0.2));
    g1.mFigures[0].mVertices.push_back(CVec2f(0, -0.4));
    g1.mFigures[0].mVertices.push_back(CVec2f(0.4, -0.2));

    CPlayerParams p1;
    p1.mVelocityLimit = 10;
    p1.mSceneObjectParams.mFixedRotation = true;
    p1.mSceneObjectParams.mPos.Set(0, 20, 0);

    mPlayer1 = GetPlayersSystem().CreatePlayer(g1, p1);

    if (mPlayer1 == nullptr)
    {
        return false;
    }

    mPlayer1Mesh = GetGrengSystems().GetMeshManager().CreateMeshFromObjFile("assets/player.obj");

    if (mPlayer1Mesh == nullptr)
    {
        return false;
    }

    GetGrengSystems().GetMeshManager().ComputeNormals(mPlayer1Mesh);

    mPlayer1Texture = GetGrengSystems().GetTextureManager().CreateTextureDummy();

    if (mPlayer1Texture == nullptr)
    {
        return false;
    }

    mPlayer1VertexShader = GetGrengSystems().GetVertexShaderManager().CreateShaderFromFile("shaders/shader2.120.vs");

    if (mPlayer1VertexShader == nullptr)
    {
        return false;
    }

    mPlayer1FragmentShader = GetGrengSystems().GetFragmentShaderManager().CreateShaderFromFile("shaders/shader2.120.fs");

    if (mPlayer1FragmentShader == nullptr)
    {
        return false;
    }

    mPlayer1ShaderProgram = GetGrengSystems().GetShaderProgramManager().CreateProgram(mPlayer1VertexShader, mPlayer1FragmentShader);

    if (mPlayer1ShaderProgram == nullptr)
    {
        return false;
    }

    return true;
}

bool CTest6::InitLight()
{
    mLight1.mPosition.Set(10, 10, 0);

    GetDebugRenderer().SetLight(&mLight1);

    return true;
}

bool CTest6::InitProcessors()
{
    GetEventSystem().SetMode("editor_mode");

    GetEventSystem().SetProcessor("C-x", CAppEventProcessor(
    [] () {},
    [this] ()
    {
        mFollowPlayer = true;
        GetEventSystem().SetMode("test6");
    }));

    GetEventSystem().SetMode("test6");

    GetEventSystem().SetProcessor("C-x", CAppEventProcessor(
    [] () {},
    [this] ()
    {
        mFollowPlayer = false;
        GetEventSystem().SetMode("editor_mode");
    }));

    GetEventSystem().SetProcessor("C-q", CAppEventProcessor(
    [this] ()
    {
        this->Quit();
    }));

    GetEventSystem().SetProcessor("w", CAppEventProcessor(
    [] () {},
    [this] ()
    {
//        this->GetPlayersSystem().SendMessage(GetPlayersSystem().GetPlayers()[0], PlayerMessage::Deep);
    }));

    GetEventSystem().SetProcessor("a", CAppEventProcessor(
    [] () {},
    [this] ()
    {
        this->GetPlayersSystem().SendMessage(GetPlayersSystem().GetPlayers()[0], PlayerMessage::Left);
    }));

    GetEventSystem().SetProcessor("s", CAppEventProcessor(
    [] () {},
    [this] ()
    {
//        this->GetPlayersSystem().SendMessage(GetPlayersSystem().GetPlayers()[0], PlayerMessage::AntiDeep);
    }));

    GetEventSystem().SetProcessor("d", CAppEventProcessor(
    [] () {},
    [this] ()
    {
        this->GetPlayersSystem().SendMessage(GetPlayersSystem().GetPlayers()[0], PlayerMessage::Right);
    }));

    GetEventSystem().SetProcessor("SPC", CAppEventProcessor(
    [] () {},
    [this] ()
    {
        this->GetPlayersSystem().SendMessage(GetPlayersSystem().GetPlayers()[0], PlayerMessage::Jump);
    }));

    GetEventSystem().SetProcessor("LB", CAppEventProcessor(
    [this] ()
    {
        CSceneObjectGeometry g;
        g.mFigures.resize(1);
        g.mFigures[0].mVertices.push_back(CVec2f(-0.1, -0.1));
        g.mFigures[0].mVertices.push_back(CVec2f(0.1, -0.1));
        g.mFigures[0].mVertices.push_back(CVec2f(0.1, 0.1));
        g.mFigures[0].mVertices.push_back(CVec2f(-0.1, 0.1));
        g.mFigures[0].mDepth = 0.2;

        CPlane pl(PlaneXY);
        CVec3f pos;
        GetCamera()->CastRay(GetCursorPos(), pl, pos);
        pos.Vec2() -= mPlayer1->GetSceneObject()->GetPos().Vec2();
        pos *= 2;

        CSceneObjectParams p;
        p.mPos = mPlayer1->GetSceneObject()->GetPos();

        if (pos.mX > 0.0f)
        {
            p.mPos.mX += 1;
        }
        else
        {
            p.mPos.mX -= 1;
        }

        CSceneObject * o = GetScene().CreateObject(g, p);
        o->SetLinearVelocity(pos.Vec2());
    }));

    return true;
}

bool CTest6::InitTarget()
{
    auto i = GetTemplateSystem().GetSceneObjectTemplates().find("Object1");

    if (i == GetTemplateSystem().GetSceneObjectTemplates().end())
    {
        return false;
    }

    mTargetGeometry = i->second;

    if (mTargetGeometry == nullptr)
    {
        return false;
    }

    return true;
}

} // namespace test
} // namespace drash
