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
#include "../debugdrawsystem/camera.h"

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

    auto c = GetDebugDrawSystem().GetActiveCam();
    c->GetPos().SetTarget(CVec3f(0, 0, 180), 1.0f, AnimationBehaviorSingle);
    c->GetRotation().Set(CVec3f(30, 0, 0));

    mSlider1.Connect(&GetUISystem());
    mSlider1.SetPos(10, 10);
    mSlider1.SetSize(100, 20);
    mSlider1.SetMin(-50);
    mSlider1.SetMax(50);
    mSlider1.SetValue(0);
    mSlider1.SetValueHandler([this] (float _value)
    {
        GetScene().SetGravity(CVec2(_value, mSlider2.GetValue()));
    });

    mSlider2.Connect(&GetUISystem());
    mSlider2.SetPos(10, 40);
    mSlider2.SetSize(100, 20);
    mSlider2.SetMin(-50);
    mSlider2.SetMax(50);
    mSlider2.SetValue(-9.8);
    mSlider2.SetValueHandler([this] (float _value)
    {
        GetScene().SetGravity(CVec2(mSlider1.GetValue(), _value));
    });

    mButton1.Connect(&GetUISystem());
    mButton1.SetPos(10, 70);
    mButton1.SetSize(100, 20);
    mButton1.SetClickHandler([this] ()
    {
        CDrashBodyParams params;
        params.mDynamic = true;
        params.mPos.mX = Randf(-100, 100);
        params.mPos.mY = 200;

        GetTemplateSystem().CreateDrashBodyFromTemplate("box_big", params);
    });

    return true;
}

void CTestApp3::Step(double _dt)
{
    CTestEditorApp::Step(_dt);

    if (GetPlayersSystem().EnumPlayers() && GetDebugDrawSystem().GetActiveCam() != nullptr)
    {
        CPlayer *p = GetPlayersSystem().GetPlayers()[0];
		auto c = GetDebugDrawSystem().GetActiveCam();
        c->GetPos().SetTarget(CVec3f(p->GetPos().Get().Vec2(), c->GetPos().GetTarget().mZ) + CVec3f(0, 100, 0), 1.0, AnimationBehaviorSingle );
    }

    mTime += _dt;

    if (mTime >= 1.0)
    {
        CDrashBodyParams params;
        params.mDynamic = true;
        params.mPos.mX = Randf(-100, 100);
        params.mPos.mY = 100;

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
        GetDebugDrawSystem().WorldSpaceToScreenSpace(lower, - mO1->GetPos().Get().mZ + GetDebugDrawSystem().GetActiveCam()->GetPos().Get().mZ);
        GetDebugDrawSystem().WorldSpaceToScreenSpace(upper, - mO1->GetPos().Get().mZ + GetDebugDrawSystem().GetActiveCam()->GetPos().Get().mZ);
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
        CVec2 tmp = GetCursorPos();
        auto cam = GetDebugDrawSystem().GetActiveCam();
        GetDebugDrawSystem().ScreenSpaceToWorldSpace(tmp, cam->GetPos().Get().mZ);
        p.mPos.mX = tmp.x;
        p.mPos.mY = tmp.y;
        GetScene().CreateObject<CExplosion>(g, p);
    }));

    GetEventSystem().SetProcessor("RB", CAppEventProcessor(
    [this] ()
    {
        if (mO1 == nullptr)
        {
            CFigure *f = GetDebugDrawSystem().FindFigure(GetCursorPos());
            if (f != nullptr)
            {
                mO1 = f->GetSceneObject();
            }
        }
        else if (mO2 == nullptr)
        {
            CFigure *f = GetDebugDrawSystem().FindFigure(GetCursorPos());
            if (f != nullptr)
            {
                mO2 = f->GetSceneObject();
            }

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
            CFigure *f = GetDebugDrawSystem().FindFigure(GetCursorPos());
            if (f != nullptr)
            {
                mMoveObject = f->GetSceneObject();
            }
        }
    },
    [this] ()
    {
        if (mMoveObject != nullptr)
        {
            CVec2 coords = GetCursorPos();
            if (GetDebugDrawSystem().ScreenSpaceToWorldSpace(coords, - mMoveObject->GetPos().Get().mZ + GetDebugDrawSystem().GetActiveCam()->GetPos().Get().mZ))
            {
                coords -= mMoveObject->GetPos().Get().Vec2();
                coords *= 10;
                mMoveObject->SetLinearVelocity(coords);
            }
        }
    },
    [this] ()
    {
        if (mMoveObject != nullptr)
        {
            /// if our body is not dynamic. it wil never stop, until we make it's velocity module to 0
            if (mMoveObject->IsDynamic() == false)
            {
                mMoveObject->SetLinearVelocity(CVec2(0));
            }
            mMoveObject = nullptr;
        }
    }));

    GetEventSystem().SetProcessor("WHUP", CAppEventProcessor(
    [this] ()
    {
        CVec3f pos = GetDebugDrawSystem().GetActiveCam()->GetPos().GetTarget();
        pos.mZ += 10.0f;
        GetDebugDrawSystem().GetActiveCam()->GetPos().SetTarget(pos, 0.3, AnimationBehaviorSingle);
    }));

    GetEventSystem().SetProcessor("WHDN", CAppEventProcessor(
    [this] ()
    {
        CVec3f pos = GetDebugDrawSystem().GetActiveCam()->GetPos().GetTarget();
        pos.mZ -= 10.0f;
        GetDebugDrawSystem().GetActiveCam()->GetPos().SetTarget(pos, 0.3, AnimationBehaviorSingle);
    }));

    GetEventSystem().SetProcessor("C-q", CAppEventProcessor(
    [this] ()
    {
        this->Quit();
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

    sbp.mPos.Set(0, 600, 0);
    GetScene().CreateObject<CSceneObject>(sbg, sbp);

    sbg.mFigures[0].mVertices.clear();
    sbg.mFigures.resize(1);
    sbg.mFigures[0].mVertices.push_back( CVec2( -5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 5, 300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( -5, 300 ) );
    sbp.mPos.Set(-300, 300, 0);
    GetScene().CreateObject<CSceneObject>(sbg, sbp);

    sbg.mFigures[0].mVertices.clear();
    sbg.mFigures.resize(1);
    sbg.mFigures[0].mVertices.push_back( CVec2( -5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( 5, 300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2( -5, 300 ) );
    sbp.mPos.Set(300, 300, 0);
    GetScene().CreateObject<CSceneObject>(sbg, sbp);

    CDrashBodyGeometry *b1 = GetTemplateSystem().CreateDrashBodyTemplate("box_small");
    b1->mFigures.resize(1);
    b1->mFigures[0].mDepth = 10;
    b1->mFigures[0].mVertices.push_back( CVec2( -5, -5 ) );
    b1->mFigures[0].mVertices.push_back( CVec2( 5, -5 ) );
    b1->mFigures[0].mVertices.push_back( CVec2( 5, 5 ) );
    b1->mFigures[0].mVertices.push_back( CVec2( -5, 5 ) );
    CDrashBodyGeometry *b0 = GetTemplateSystem().CreateDrashBodyTemplate("box_big");
    b0->mFigures.resize(1);
    b0->mFigures[0].mDepth = 20;
    b0->mFigures[0].mVertices.push_back( CVec2( -10, -10 ) );
    b0->mFigures[0].mVertices.push_back( CVec2( 10, -10 ) );
    b0->mFigures[0].mVertices.push_back( CVec2( 10, 10 ) );
    b0->mFigures[0].mVertices.push_back( CVec2( -10, 10 ) );

    b0->mDestructionChilds.resize(8);
    b0->mDestructionChilds[0].mTemplate = b1;
    b0->mDestructionChilds[0].mParams.mPos.Set(-5, -5, 5);
    b0->mDestructionChilds[1].mTemplate = b1;
    b0->mDestructionChilds[1].mParams.mPos.Set(-5, 5, 5);
    b0->mDestructionChilds[2].mTemplate = b1;
    b0->mDestructionChilds[2].mParams.mPos.Set(5, 5, 5);
    b0->mDestructionChilds[3].mTemplate = b1;
    b0->mDestructionChilds[3].mParams.mPos.Set(5, -5, 5);
    b0->mDestructionChilds[4].mTemplate = b1;
    b0->mDestructionChilds[4].mParams.mPos.Set(-5, -5, -5);
    b0->mDestructionChilds[5].mTemplate = b1;
    b0->mDestructionChilds[5].mParams.mPos.Set(-5, 5, -5);
    b0->mDestructionChilds[6].mTemplate = b1;
    b0->mDestructionChilds[6].mParams.mPos.Set(5, 5, -5);
    b0->mDestructionChilds[7].mTemplate = b1;
    b0->mDestructionChilds[7].mParams.mPos.Set(5, -5, -5);

    CDrashBodyParams dbp;
    dbp.mPos.Set(0, 100, 0);
    dbp.mAngle = M_PI / 4;

    GetScene().CreateObject<CDrashBody>(*b0, dbp);

    CSceneObjectGeometry pg;
    pg.mFigures.resize(1);
    pg.mFigures[0].mMass = 3;
    CSceneObjectParams pp;
    pp.mPos.Set(-200, 100, 0);
    GetScene().CreateObject<CSceneObject>(pg, pp)->SetLinearVelocity( CVec2( 200, 0 ) );

    CSceneObjectGeometry ppg;
    ppg.mFigures.resize(1);
    ppg.mFigures[0].mDepth = 2;
    CPlayerParams ppp;
    ppp.mPos.Set(0, 10, 0);
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
    platform_params.mPos.Set(0, 50, 0);
    platform_params.mAngle = -M_PI / 18.0;
    platform_params.mDynamic = false;

    CSceneObject *platform = GetScene().CreateObject<CSceneObject>(platform_geometry, platform_params);
    platform->GetPos().Set(CVec3f(-100, 50, 0));
    platform->GetPos().SetTarget(CVec3f(100, 50, 0), 10, AnimationBehaviorBounce);
    platform->GetAngle().SetTarget(M_PI / 18.0, 10, AnimationBehaviorBounce);

    static const unsigned int segments = 16;
    static const float d_angle = M_PI * 2.0 / static_cast<double>(segments);
    static const float rad = 5;

    CDrashBodyGeometry *t2 = GetTemplateSystem().CreateDrashBodyTemplate("circle_div_1");
    if (t2 != nullptr)
    {
        t2->mFigures.resize(4);
        for (unsigned int i=0; i<4; i++)
        {
            t2->mFigures[i].mVertices.push_back(CVec2(rad * cos(i * d_angle), rad * sin(i * d_angle)));
            t2->mFigures[i].mVertices.push_back(CVec2(rad * cos((i + 1) * d_angle), rad * sin((i + 1) * d_angle)));
            t2->mFigures[i].mVertices.push_back(CVec2(rad * 0.5 * cos((i + 1) * d_angle), rad * 0.5 * sin((i + 1) * d_angle)));
            t2->mFigures[i].mVertices.push_back(CVec2(rad * 0.5 * cos(i * d_angle), rad * 0.5 * sin(i * d_angle)));
            t2->mFigures[i].mDepth = rad * 0.5;
        }
    }

    CDrashBodyGeometry *t3 = GetTemplateSystem().CreateDrashBodyTemplate("circle_div_2");
    if (t3 != nullptr)
    {
        t3->mFigures.resize(4);
        for (unsigned int i=4; i<8; i++)
        {
            t3->mFigures[i-4].mVertices.push_back(CVec2(rad * cos(i * d_angle), rad * sin(i * d_angle)));
            t3->mFigures[i-4].mVertices.push_back(CVec2(rad * cos((i + 1) * d_angle), rad * sin((i + 1) * d_angle)));
            t3->mFigures[i-4].mVertices.push_back(CVec2(rad * 0.5 * cos((i + 1) * d_angle), rad * 0.5 * sin((i + 1) * d_angle)));
            t3->mFigures[i-4].mVertices.push_back(CVec2(rad * 0.5 * cos(i * d_angle), rad * 0.5 * sin(i * d_angle)));
            t3->mFigures[i-4].mDepth = rad * 0.5;
        }
    }

    CDrashBodyGeometry *t4 = GetTemplateSystem().CreateDrashBodyTemplate("circle_div_3");
    if (t4 != nullptr)
    {
        t4->mFigures.resize(4);
        for (unsigned int i=8; i<12; i++)
        {
            t4->mFigures[i-8].mVertices.push_back(CVec2(rad * cos(i * d_angle), rad * sin(i * d_angle)));
            t4->mFigures[i-8].mVertices.push_back(CVec2(rad * cos((i + 1) * d_angle), rad * sin((i + 1) * d_angle)));
            t4->mFigures[i-8].mVertices.push_back(CVec2(rad * 0.5 * cos((i + 1) * d_angle), rad * 0.5 * sin((i + 1) * d_angle)));
            t4->mFigures[i-8].mVertices.push_back(CVec2(rad * 0.5 * cos(i * d_angle), rad * 0.5 * sin(i * d_angle)));
            t4->mFigures[i-8].mDepth = rad * 0.5;
        }
    }

    CDrashBodyGeometry *t5 = GetTemplateSystem().CreateDrashBodyTemplate("circle_div_4");
    if (t5 != nullptr)
    {
        t5->mFigures.resize(4);
        for (unsigned int i=12; i<16; i++)
        {
            t5->mFigures[i-12].mVertices.push_back(CVec2(rad * cos(i * d_angle), rad * sin(i * d_angle)));
            t5->mFigures[i-12].mVertices.push_back(CVec2(rad * cos((i + 1) * d_angle), rad * sin((i + 1) * d_angle)));
            t5->mFigures[i-12].mVertices.push_back(CVec2(rad * 0.5 * cos((i + 1) * d_angle), rad * 0.5 * sin((i + 1) * d_angle)));
            t5->mFigures[i-12].mVertices.push_back(CVec2(rad * 0.5 * cos(i * d_angle), rad * 0.5 * sin(i * d_angle)));
            t5->mFigures[i-12].mDepth = rad * 0.5;
        }
    }

    CDrashBodyGeometry *t1 = GetTemplateSystem().CreateDrashBodyTemplate("circle");
    if (t1 != nullptr)
    {
        t1->mFigures.resize(segments);
        for (unsigned int i=0; i<segments; i++)
        {
            t1->mFigures[i].mVertices.push_back(CVec2(rad * cos(i * d_angle), rad * sin(i * d_angle)));
            t1->mFigures[i].mVertices.push_back(CVec2(rad * cos((i + 1) * d_angle), rad * sin((i + 1) * d_angle)));
            t1->mFigures[i].mVertices.push_back(CVec2(rad * 0.5 * cos((i + 1) * d_angle), rad * 0.5 * sin((i + 1) * d_angle)));
            t1->mFigures[i].mVertices.push_back(CVec2(rad * 0.5 * cos(i * d_angle), rad * 0.5 * sin(i * d_angle)));
            t1->mFigures[i].mDepth = rad * 0.5;
        }

        t1->mDestructionChilds.resize(4);
        t1->mDestructionChilds[0].mTemplate = t2;
        t1->mDestructionChilds[1].mTemplate = t3;
        t1->mDestructionChilds[2].mTemplate = t4;
        t1->mDestructionChilds[3].mTemplate = t5;
    }
}

} // namespace drash

