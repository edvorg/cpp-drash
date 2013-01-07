// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

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
#include "../debugdrawsystem/camera.h"
#include "../scene/scene.h"

namespace drash
{

namespace test
{

bool CTest6::Init()
{
    if (CTest1::Init() == false)
    {
        return false;
    }

    if (InitPlayer() == false)
    {
        return false;
    }

    if (InitLight() == false)
    {
        return false;
    }

    if (InitProcessors() == false)
    {
        return false;
    }

    CSceneObjectGeometry g2;

    g2.mFigures.resize(1);
    g2.mFigures[0].mDepth = 50;
    g2.mFigures[0].mVertices.push_back(CVec2f(-50, 1));
    g2.mFigures[0].mVertices.push_back(CVec2f(-50, -1));
    g2.mFigures[0].mVertices.push_back(CVec2f(50, -1));
    g2.mFigures[0].mVertices.push_back(CVec2f(50, 1));

    CSceneObjectParams p2;
    p2.mDynamic = false;

    if (GetScene().CreateObject(g2, p2) == nullptr)
    {
        return false;
    }

    if (GetDebugDrawSystem().GetActiveCam() == nullptr)
    {
        return false;
    }

    GetDebugDrawSystem().GetActiveCam()->GetPos().Set(CVec3f(0, 75, 150));
    GetDebugDrawSystem().GetActiveCam()->GetRotation().Set(CVec2f(-M_PI / 12.0, 0));

    return true;
}

void CTest6::Step(double _dt)
{
    CTest1::Step(_dt);

    mAngle += _dt;

    while (mAngle >= M_PI * 2.0)
    {
        mAngle -= M_PI * 2.0;
    }

    mPointLight1.mPosition.Set(sin(mAngle) * 40, 20, 0);

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
            MatrixRotationZ(m, angle * -0.02);
        }
        else
        {
            MatrixRotationZ(m, angle * 0.02);
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
    MatrixScale(s, CVec3f(1));

    CMatrix4f rot;
    MatrixMultiply(r, s, rot);

    CMatrix4f transl;
    MatrixTranslation(transl, mPlayer1->GetSceneObject()->GetPos() - CVec3f(0, 1, 0));

    CMatrix4f model;
    MatrixMultiply(transl, rot, model);

    CMatrix4f model_view;
    MatrixMultiply(GetDebugDrawSystem().GetActiveCam()->GetViewMatrix(), model, model_view);

    for (unsigned int i = 0; i < 6; i++)
    {
        GetRenderer().RenderMesh(mPlayer1Mesh,
                                 i,
                                 mPlayer1Texture,
                                 mPlayer1ShaderProgram,
                                 model,
                                 GetDebugDrawSystem().GetActiveCam()->GetViewMatrix(),
                                 model_view,
                                 GetDebugDrawSystem().GetActiveCam()->GetProjectionMatrix(),
                                 &mPointLight1);
    }

    GetDebugDrawSystem().DrawPoint(mPointLight1.mPosition,
                                   10,
                                   CColor4f(1, 1, 1, 1),
                                   true);
}

bool CTest6::InitPlayer()
{
    CSceneObjectGeometry g1;

    g1.mFigures.resize(1);
    g1.mFigures[0].mVertices.push_back(CVec2f(-1, 1));
    g1.mFigures[0].mVertices.push_back(CVec2f(-1, -1));
    g1.mFigures[0].mVertices.push_back(CVec2f(1, -1));
    g1.mFigures[0].mVertices.push_back(CVec2f(1, 1));

    CPlayerParams p1;
    p1.mVelocityLimit = 10;
    p1.mSceneObjectParams.mFixedRotation = true;
    p1.mSceneObjectParams.mPos.Set(0, 10, 0);

    mPlayer1 = GetPlayersSystem().CreatePlayer(g1, p1);

    if (mPlayer1 == nullptr)
    {
        return false;
    }

    mPlayer1Mesh = GetMeshManager().CreateMeshFromObjFile("player.obj");

    if (mPlayer1Mesh == nullptr)
    {
        return false;
    }

    GetMeshManager().ComputeNormals(mPlayer1Mesh);

    mPlayer1Texture = GetTextureManager().CreateTextureDummy();

    if (mPlayer1Texture == nullptr)
    {
        return false;
    }

    mPlayer1VertexShader = GetVertexShaderManager().CreateShaderFromFile("shader2.120.vs");

    if (mPlayer1VertexShader == nullptr)
    {
        return false;
    }

    mPlayer1FragmentShader = GetFragmentShaderManager().CreateShaderFromFile("shader2.120.fs");

    if (mPlayer1FragmentShader == nullptr)
    {
        return false;
    }

    mPlayer1ShaderProgram = GetShaderProgramManager().CreateProgram(mPlayer1VertexShader, mPlayer1FragmentShader);

    if (mPlayer1ShaderProgram == nullptr)
    {
        return false;
    }

    return true;
}

bool CTest6::InitLight()
{
    mPointLight1.mPosition.Set(10, 10, 0);

    return true;
}

bool CTest6::InitProcessors()
{
    GetEventSystem().SetMode("editor_mode");

    GetEventSystem().SetProcessor("C-x", CAppEventProcessor(
    [] () {},
    [this] ()
    {
        GetEventSystem().SetMode("test6");
    }));

    GetEventSystem().SetMode("test6");

    GetEventSystem().SetProcessor("C-x", CAppEventProcessor(
    [] () {},
    [this] ()
    {
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
        this->GetPlayersSystem().SendMessage(GetPlayersSystem().GetPlayers()[0], PlayerMessage::Deep);
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
        this->GetPlayersSystem().SendMessage(GetPlayersSystem().GetPlayers()[0], PlayerMessage::AntiDeep);
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

    return true;
}

} // namespace test
} // namespace drash
