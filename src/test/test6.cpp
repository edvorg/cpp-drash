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
#include "../scene/player.h"
#include "../debugdrawsystem/camera.h"

namespace drash
{

namespace test
{

bool CTest6::Init()
{
    if (CApp::Init() == false)
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

    CSceneObjectGeometry *g2 = GetTemplateSystem().CreateSceneObjectTemplate("platform");

    g2->mFigures.resize(1);
    g2->mFigures[0].mDepth = 50;
    g2->mFigures[0].mVertices.push_back(CVec2f(-50, 1));
    g2->mFigures[0].mVertices.push_back(CVec2f(-50, -1));
    g2->mFigures[0].mVertices.push_back(CVec2f(50, -1));
    g2->mFigures[0].mVertices.push_back(CVec2f(50, 1));

    CSceneObjectParams p2;
    p2.mDynamic = false;

    if (GetTemplateSystem().CreateSceneObjectFromTemplate("platform", p2) == nullptr)
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
    CApp::Step(_dt);

    mAngle += _dt;

    while (mAngle >= M_PI * 2.0)
    {
        mAngle -= M_PI * 2.0;
    }

    mPointLight1.mPosition.Set(sin(mAngle) * 40, 20, 0);

    CVec2f dir(GetPlayersSystem().GetPlayers()[0]->GetPosXY().Get().mX - mPlayer1OldPos.mX,
               mPlayer1OldPos.mZ - GetPlayersSystem().GetPlayers()[0]->GetPosZ());

    if (dir.Length() < 0.00001)
    {
        mPlayer1Angle *= 0.99;
    }
    else
    {
        dir.Normalize();

        float new_angle = acos(dir.mX);

        if (dir.mY < 0.0f)
        {
            new_angle *= -1.0f;
        }

        mPlayer1Angle = 0.99 * mPlayer1Angle + 0.01 * new_angle;

        mPlayer1OldPos = GetPlayersSystem().GetPlayers()[mPlayer1Id]->GetPos();

        LOG_ERR(mPlayer1Angle);
    }
}

void CTest6::Render()
{
    CApp::Render();

    CMatrix4f r;
    MatrixRotationY(r, M_PI * 0.5 + mPlayer1Angle);

    CMatrix4f s;
    MatrixScale(s, CVec3f(1));

    CMatrix4f rot;
    MatrixMultiply(r, s, rot);

    CMatrix4f transl;
    MatrixTranslation(transl, GetPlayersSystem().GetPlayers()[mPlayer1Id]->GetPos() - CVec3f(0, 1, 0));

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
    CSceneObjectGeometry *g1 = GetTemplateSystem().CreateSceneObjectTemplate("player");

    if (g1 == nullptr)
    {
        return false;
    }

    g1->mFigures.resize(1);
    g1->mFigures[0].mVertices.push_back(CVec2f(-1, 1));
    g1->mFigures[0].mVertices.push_back(CVec2f(-1, -1));
    g1->mFigures[0].mVertices.push_back(CVec2f(1, -1));
    g1->mFigures[0].mVertices.push_back(CVec2f(1, 1));

    CPlayerParams p1;
    p1.mFixedRotation = true;
    p1.mPos.Set(0, 10, 0);

    mPlayer1Id = GetPlayersSystem().AddPlayer(*g1, p1);

    if (mPlayer1Id == -1)
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
    GetEventSystem().SetProcessor("C-q", CAppEventProcessor(
    [this] ()
    {
        this->Quit();
    }));

    GetEventSystem().SetProcessor("w", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        GetPlayersSystem().OnPlayerEvent(CPlayerEvent::PlayerActionMoveDeep, mPlayer1Id);
    }));

    GetEventSystem().SetProcessor("a", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        GetPlayersSystem().OnPlayerEvent(CPlayerEvent::PlayerActionMoveLeft, mPlayer1Id);
    }));

    GetEventSystem().SetProcessor("s", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        GetPlayersSystem().OnPlayerEvent(CPlayerEvent::PlayerActionMoveOut, mPlayer1Id);
    }));

    GetEventSystem().SetProcessor("d", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        GetPlayersSystem().OnPlayerEvent(CPlayerEvent::PlayerActionMoveRight, mPlayer1Id);
    }));

    GetEventSystem().SetProcessor("SPC", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        GetPlayersSystem().OnPlayerEvent(CPlayerEvent::PlayerActionJump, mPlayer1Id);
    }));

    return true;
}

} // namespace test
} // namespace drash
