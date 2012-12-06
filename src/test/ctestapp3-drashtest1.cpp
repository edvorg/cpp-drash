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

#include "ctestapp3-drashtest1.h"

#include "../cscene.h"
#include "../sceneobjects.h"

namespace drash
{

void GenDrashBodyParams( CDrashBodyParams* _params , float _subset_size, unsigned int _level, unsigned int _levels );

bool CTestApp3::Init()
{
    if ( CTestEditorApp::Init() == false )
    {
        return false;
    }

    SetProcessors();

    InitObjects();

    GetDebugDrawSystem().GetActiveCam()->SetZTarget( 280, 1.0f, AnimationBehaviorSingle );

    return true;
}

void CTestApp3::Step(double _dt)
{
    CTestEditorApp::Step(_dt);

    if (mMoveObject != nullptr)
    {
        CVec2 coords = GetCursorPos();
        if (GetDebugDrawSystem().ScreenSpaceToWorldSpace(coords, mMoveObject->GetZ().Get() - GetDebugDrawSystem().GetActiveCam()->GetZ().Get()))
        {
            coords -= mMoveObject->GetPos().Get();
            coords *= 10;
            mMoveObject->SetLinearVelocity(coords);
        }
    }

    if (GetPlayersSystem().EnumPlayers())
    {
        CPlayer *p = GetPlayersSystem().GetPlayers()[0];
        GetDebugDrawSystem().GetActiveCam()->SetPosTarget( p->GetBody()->GetWorldCenter(), 1.0, AnimationBehaviorSingle );
    }

    mTime += _dt;

    if (mTime >= 1.0)
    {
        CDrashBodyParams params;
        params.mDynamic = true;
        params.mPos.RandX(-100, 100);
        params.mPos.y = 100;

        GetTemplateSystem().CreateDrashBodyFromTemplate("circle", params);

        mTime = 0;
    }
}

void CTestApp3::Render()
{
    CTestEditorApp::Render();

    if (mO1 != nullptr)
    {
        mO1->ComputeBoundingBox();
        const b2AABB &b = mO1->GetBoundingBox();
        CVec2 upper = b.upperBound;
        CVec2 lower = b.lowerBound;
        GetDebugDrawSystem().WorldSpaceToScreenSpace(lower, mO1->GetZ().Get() - GetDebugDrawSystem().GetActiveCam()->GetZ().Get());
        GetDebugDrawSystem().WorldSpaceToScreenSpace(upper, mO1->GetZ().Get() - GetDebugDrawSystem().GetActiveCam()->GetZ().Get());
        b2Color col(1, 0, 0);
        CVec2 tmp1(upper.x, lower.y);
        CVec2 tmp2(lower.x, upper.y);
        GetDebugDrawSystem().DrawLine(tmp1, upper, col);
        GetDebugDrawSystem().DrawLine(tmp1, lower, col);
        GetDebugDrawSystem().DrawLine(tmp2, upper, col);
        GetDebugDrawSystem().DrawLine(tmp2, lower, col);
    }
}

void CTestApp3::SetProcessors()
{
    GetEventSystem().SetMode("editor_mode");

    GetEventSystem().SetProcessor("C-x", CAppEventProcessor(
    [] () {},
    [this] ()
    {
        GetEventSystem().SetMode("test3");
    }));

    GetEventSystem().SetMode("test3");

    GetEventSystem().SetProcessor("C-x", CAppEventProcessor(
    [] () {},
    [this] ()
    {
        GetEventSystem().SetMode("editor_mode");
    }));

    GetEventSystem().SetProcessor("w", CAppEventProcessor(
    [] () {},
    [this] ()
    {
        this->GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveDeep ), 0 );
    }));

    GetEventSystem().SetProcessor("a", CAppEventProcessor(
    [] () {},
    [this] ()
    {
        this->GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveLeft ), 0 );
    }));

    GetEventSystem().SetProcessor("s", CAppEventProcessor(
    [] () {},
    [this] ()
    {
        this->GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveOut ), 0 );
    }));

    GetEventSystem().SetProcessor("d", CAppEventProcessor(
    [] () {},
    [this] ()
    {
        this->GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionMoveRight ), 0 );
    }));

    GetEventSystem().SetProcessor("SPC", CAppEventProcessor(
    [] () {},
    [this] ()
    {
        this->GetPlayersSystem().OnPlayerEvent( CPlayerEvent( CPlayerEvent::PlayerActionJump ), 0 );
    }));

    GetEventSystem().SetProcessor("LB", CAppEventProcessor(
    [this] ()
    {
        CSceneObjectGeometry g;
        CExplosionParams p;
        p.mLifeTime = 1;
        p.mStregth = -5;
        p.mRadius = 200;
        p.mPos = GetCursorPos();
        auto cam = GetDebugDrawSystem().GetActiveCam();
        GetDebugDrawSystem().ScreenSpaceToWorldSpace(p.mPos, -cam->GetZ().Get());
        GetScene().CreateObject<CExplosion>(g, p);
    }));

    GetEventSystem().SetProcessor("RB", CAppEventProcessor(
    [this] ()
    {
        if (mO1 == nullptr)
        {
            mO1 = GetDebugDrawSystem().FindObject(GetCursorPos());
        }
        else if (mO2 == nullptr)
        {
            mO2 = GetDebugDrawSystem().FindObject(GetCursorPos());
            if (mO1 == mO2)
            {
                mO2 = nullptr;
            }
            else if (mO2 != nullptr)
            {
                GetScene().CreateJoint(mO1, mO2, mO1->GetPos().Get());
                mO1 = nullptr;
                mO2 = nullptr;
            }
        }
    }));

    GetEventSystem().SetProcessor("MB", CAppEventProcessor(
    [this] ()
    {
        if (mMoveObject == nullptr)
        {
            mMoveObject = GetDebugDrawSystem().FindObject(GetCursorPos());
        }
        else
        {
            /// if our body is not dynamic. it wil never stop, until we make it's velocity module to 0
            if (mMoveObject->GetBody()->GetType() == b2_kinematicBody)
            {
                mMoveObject->SetLinearVelocity(CVec2(0));
            }
            mMoveObject = nullptr;
        }
    }));

    GetEventSystem().SetProcessor("WHUP", CAppEventProcessor(
    [this] ()
    {
        float pos = GetDebugDrawSystem().GetActiveCam()->GetZ().GetTarget();
        pos += 10.0f;
        GetDebugDrawSystem().GetActiveCam()->SetZTarget( pos, 0.3, AnimationBehaviorSingle );
    }));

    GetEventSystem().SetProcessor("WHDN", CAppEventProcessor(
    [this] ()
    {
        float pos = GetDebugDrawSystem().GetActiveCam()->GetZ().GetTarget();
        pos -= 10.0f;
        GetDebugDrawSystem().GetActiveCam()->SetZTarget( pos, 0.3, AnimationBehaviorSingle );
    }));
}

void CTestApp3::InitObjects()
{
    CSceneObjectGeometry sbg;
    sbg.mFigures.resize(1);
    sbg.mFigures[0].mDepth = 40;
    sbg.mFigures[0].mRestitution = 0.0;
    sbg.mFigures[0].mVertices.push_back( CVec2( -300, -5 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 300, -5 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 300, 5 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( -300, 5 ) );
    CSceneObjectParams sbp;
    sbp.mDynamic = false;
    sbp.mAngle = 0;
    GetScene().CreateObject<CSceneObject>(sbg, sbp);

    sbp.mPos.Set( 0, 600 );
    GetScene().CreateObject<CSceneObject>(sbg, sbp);

    sbg.mFigures[0].mVertices.clear();
    sbg.mFigures.resize(1);
    sbg.mFigures[0].mVertices.push_back( CVec2( -5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 5, 300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( -5, 300 ) );
    sbp.mPos.Set( -300, 300 );
    GetScene().CreateObject<CSceneObject>(sbg, sbp);

    sbg.mFigures[0].mVertices.clear();
    sbg.mFigures.resize(1);
    sbg.mFigures[0].mVertices.push_back( CVec2( -5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 5, 300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( -5, 300 ) );
    sbp.mPos.Set( 300, 300 );
    GetScene().CreateObject<CSceneObject>(sbg, sbp);

    CDrashBodyTemplate *b1 = GetTemplateSystem().CreateDrashBodyTemplate("box_small");
    b1->mGeometry.mFigures.resize(1);
    b1->mGeometry.mFigures[0].mDepth = 10;
    b1->mGeometry.mFigures[0].mVertices.push_back( CVec2( -5, -5 ) );
    b1->mGeometry.mFigures[0].mVertices.push_back( CVec2( 5, -5 ) );
    b1->mGeometry.mFigures[0].mVertices.push_back( CVec2( 5, 5 ) );
    b1->mGeometry.mFigures[0].mVertices.push_back( CVec2( -5, 5 ) );
    CDrashBodyTemplate *b0 = GetTemplateSystem().CreateDrashBodyTemplate("box_big");
    b0->mGeometry.mFigures.resize(1);
    b0->mGeometry.mFigures[0].mDepth = 20;
    b0->mGeometry.mFigures[0].mVertices.push_back( CVec2( -10, -10 ) );
    b0->mGeometry.mFigures[0].mVertices.push_back( CVec2( 10, -10 ) );
    b0->mGeometry.mFigures[0].mVertices.push_back( CVec2( 10, 10 ) );
    b0->mGeometry.mFigures[0].mVertices.push_back( CVec2( -10, 10 ) );

    b0->mGeometry.mDestructionChilds.resize(8);
    b0->mGeometry.mDestructionChilds[0].mTemplate = b1;
    b0->mGeometry.mDestructionChilds[0].mParams.mPos.Set(-5,-5);
    b0->mGeometry.mDestructionChilds[0].mParams.mZ = 5;
    b0->mGeometry.mDestructionChilds[1].mTemplate = b1;
    b0->mGeometry.mDestructionChilds[1].mParams.mPos.Set(-5,5);
    b0->mGeometry.mDestructionChilds[1].mParams.mZ = 5;
    b0->mGeometry.mDestructionChilds[2].mTemplate = b1;
    b0->mGeometry.mDestructionChilds[2].mParams.mPos.Set(5,5);
    b0->mGeometry.mDestructionChilds[2].mParams.mZ = 5;
    b0->mGeometry.mDestructionChilds[3].mTemplate = b1;
    b0->mGeometry.mDestructionChilds[3].mParams.mPos.Set(5,-5);
    b0->mGeometry.mDestructionChilds[3].mParams.mZ = 5;
    b0->mGeometry.mDestructionChilds[4].mTemplate = b1;
    b0->mGeometry.mDestructionChilds[4].mParams.mPos.Set(-5,-5);
    b0->mGeometry.mDestructionChilds[4].mParams.mZ = -5;
    b0->mGeometry.mDestructionChilds[5].mTemplate = b1;
    b0->mGeometry.mDestructionChilds[5].mParams.mPos.Set(-5,5);
    b0->mGeometry.mDestructionChilds[5].mParams.mZ = -5;
    b0->mGeometry.mDestructionChilds[6].mTemplate = b1;
    b0->mGeometry.mDestructionChilds[6].mParams.mPos.Set(5,5);
    b0->mGeometry.mDestructionChilds[6].mParams.mZ = -5;
    b0->mGeometry.mDestructionChilds[7].mTemplate = b1;
    b0->mGeometry.mDestructionChilds[7].mParams.mPos.Set(5,-5);
    b0->mGeometry.mDestructionChilds[7].mParams.mZ = -5;

    CDrashBodyParams dbp;
    dbp.mPos.Set( 0, 100 );
    dbp.mAngle = M_PI / 4;

    GetScene().CreateObject<CDrashBody>(b0->mGeometry, dbp);

    CSceneObjectGeometry pg;
    pg.mFigures.resize(1);
    pg.mFigures[0].mMass = 3;
    CSceneObjectParams pp;
    pp.mPos.Set( -200, 100 );
    GetScene().CreateObject<CSceneObject>(pg, pp)->SetLinearVelocity( CVec2( 200, 0 ) );

    CSceneObjectGeometry ppg;
    ppg.mFigures.resize(1);
    ppg.mFigures[0].mDepth = 2;
    CPlayerParams ppp;
    ppp.mPos.Set( 0, 10 );
    GetPlayersSystem().AddPlayer(ppg, ppp);

    CSceneObjectGeometry platform_geometry;
    platform_geometry.mFigures.resize(1);
    platform_geometry.mFigures[0].mDepth = 8;
    platform_geometry.mFigures[0].mFriction = 1.0;
    platform_geometry.mFigures[0].mVertices.push_back( CVec2( -100, -5 ) );
    platform_geometry.mFigures[0].mVertices.push_back( CVec2( 100, -5 ) );
    platform_geometry.mFigures[0].mVertices.push_back( CVec2( 100, 5 ) );
    platform_geometry.mFigures[0].mVertices.push_back( CVec2( -100, 5 ) );
    CSceneObject::ParamsT platform_params;
    platform_params.mPos.Set( 0, 50 );
    platform_params.mAngle = -M_PI / 18.0;
    platform_params.mDynamic = false;

    CSceneObject *platform = GetScene().CreateObject<CSceneObject>(platform_geometry, platform_params);
    platform->SetPos( CVec2( -100, 50 ) );
    platform->SetPosTarget( CVec2( 100, 50 ), 10, AnimationBehaviorBounce );
    platform->SetAngleTarget( M_PI / 18.0, 10, AnimationBehaviorBounce );

    static const unsigned int segments = 16;
    static const float d_angle = M_PI * 2.0 / static_cast<double>(segments);
    static const float rad = 5;

    CDrashBodyTemplate *t2 = GetTemplateSystem().CreateDrashBodyTemplate("circle_div_1");
    if (t2 != nullptr)
    {
        t2->mGeometry.mFigures.resize(4);
        for (unsigned int i=0; i<4; i++)
        {
            t2->mGeometry.mFigures[i].mVertices.push_back(CVec2(rad * cos(i * d_angle), rad * sin(i * d_angle)));
            t2->mGeometry.mFigures[i].mVertices.push_back(CVec2(rad * cos((i + 1) * d_angle), rad * sin((i + 1) * d_angle)));
            t2->mGeometry.mFigures[i].mVertices.push_back(CVec2(rad * 0.5 * cos((i + 1) * d_angle), rad * 0.5 * sin((i + 1) * d_angle)));
            t2->mGeometry.mFigures[i].mVertices.push_back(CVec2(rad * 0.5 * cos(i * d_angle), rad * 0.5 * sin(i * d_angle)));
            t2->mGeometry.mFigures[i].mDepth = rad * 0.5;
        }
    }

    CDrashBodyTemplate *t3 = GetTemplateSystem().CreateDrashBodyTemplate("circle_div_2");
    if (t3 != nullptr)
    {
        t3->mGeometry.mFigures.resize(4);
        for (unsigned int i=4; i<8; i++)
        {
            t3->mGeometry.mFigures[i-4].mVertices.push_back(CVec2(rad * cos(i * d_angle), rad * sin(i * d_angle)));
            t3->mGeometry.mFigures[i-4].mVertices.push_back(CVec2(rad * cos((i + 1) * d_angle), rad * sin((i + 1) * d_angle)));
            t3->mGeometry.mFigures[i-4].mVertices.push_back(CVec2(rad * 0.5 * cos((i + 1) * d_angle), rad * 0.5 * sin((i + 1) * d_angle)));
            t3->mGeometry.mFigures[i-4].mVertices.push_back(CVec2(rad * 0.5 * cos(i * d_angle), rad * 0.5 * sin(i * d_angle)));
            t3->mGeometry.mFigures[i-4].mDepth = rad * 0.5;
        }
    }

    CDrashBodyTemplate *t4 = GetTemplateSystem().CreateDrashBodyTemplate("circle_div_3");
    if (t4 != nullptr)
    {
        t4->mGeometry.mFigures.resize(4);
        for (unsigned int i=8; i<12; i++)
        {
            t4->mGeometry.mFigures[i-8].mVertices.push_back(CVec2(rad * cos(i * d_angle), rad * sin(i * d_angle)));
            t4->mGeometry.mFigures[i-8].mVertices.push_back(CVec2(rad * cos((i + 1) * d_angle), rad * sin((i + 1) * d_angle)));
            t4->mGeometry.mFigures[i-8].mVertices.push_back(CVec2(rad * 0.5 * cos((i + 1) * d_angle), rad * 0.5 * sin((i + 1) * d_angle)));
            t4->mGeometry.mFigures[i-8].mVertices.push_back(CVec2(rad * 0.5 * cos(i * d_angle), rad * 0.5 * sin(i * d_angle)));
            t4->mGeometry.mFigures[i-8].mDepth = rad * 0.5;
        }
    }

    CDrashBodyTemplate *t5 = GetTemplateSystem().CreateDrashBodyTemplate("circle_div_4");
    if (t5 != nullptr)
    {
        t5->mGeometry.mFigures.resize(4);
        for (unsigned int i=12; i<16; i++)
        {
            t5->mGeometry.mFigures[i-12].mVertices.push_back(CVec2(rad * cos(i * d_angle), rad * sin(i * d_angle)));
            t5->mGeometry.mFigures[i-12].mVertices.push_back(CVec2(rad * cos((i + 1) * d_angle), rad * sin((i + 1) * d_angle)));
            t5->mGeometry.mFigures[i-12].mVertices.push_back(CVec2(rad * 0.5 * cos((i + 1) * d_angle), rad * 0.5 * sin((i + 1) * d_angle)));
            t5->mGeometry.mFigures[i-12].mVertices.push_back(CVec2(rad * 0.5 * cos(i * d_angle), rad * 0.5 * sin(i * d_angle)));
            t5->mGeometry.mFigures[i-12].mDepth = rad * 0.5;
        }
    }

    CDrashBodyTemplate *t1 = GetTemplateSystem().CreateDrashBodyTemplate("circle");
    if (t1 != nullptr)
    {
        t1->mGeometry.mFigures.resize(segments);
        for (unsigned int i=0; i<segments; i++)
        {
            t1->mGeometry.mFigures[i].mVertices.push_back(CVec2(rad * cos(i * d_angle), rad * sin(i * d_angle)));
            t1->mGeometry.mFigures[i].mVertices.push_back(CVec2(rad * cos((i + 1) * d_angle), rad * sin((i + 1) * d_angle)));
            t1->mGeometry.mFigures[i].mVertices.push_back(CVec2(rad * 0.5 * cos((i + 1) * d_angle), rad * 0.5 * sin((i + 1) * d_angle)));
            t1->mGeometry.mFigures[i].mVertices.push_back(CVec2(rad * 0.5 * cos(i * d_angle), rad * 0.5 * sin(i * d_angle)));
            t1->mGeometry.mFigures[i].mDepth = rad * 0.5;
        }

        t1->mGeometry.mDestructionChilds.resize(4);
        t1->mGeometry.mDestructionChilds[0].mTemplate = t2;
        t1->mGeometry.mDestructionChilds[1].mTemplate = t3;
        t1->mGeometry.mDestructionChilds[2].mTemplate = t4;
        t1->mGeometry.mDestructionChilds[3].mTemplate = t5;
    }
}

} // namespace drash

