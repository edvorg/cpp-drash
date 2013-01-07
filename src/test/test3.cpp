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

#include "test3.h"

#include "../scene/scene.h"
#include "../explosion/explosion.h"
#include "../players/player.h"
#include "../scene/figure.h"

#include "../debugdrawsystem/camera.h"
#include "../misc/plane.h"

namespace drash
{

namespace test
{

bool CTest3::Init()
{
    if ( CTest1::Init() == false )
    {
        return false;
    }

    SetProcessors();

    InitObjects();

    auto c = GetDebugDrawSystem().GetActiveCam();
    c->GetPos().SetTarget(CVec3f(0, 50, 180), 1.0f, AnimatorBehavior::Single);
    c->GetRotation().Set(CVec3f(-M_PI / 12, 0, 0));

    mSlider1.Connect(&GetUISystem());
    mSlider1.SetPos(CVec2i(10, 10));
    mSlider1.SetSize(CVec2ui(100, 20));
    mSlider1.SetMin(-50);
    mSlider1.SetMax(50);
    mSlider1.SetValue(0);
    mSlider1.SetValueHandler([this] (float _value)
    {
        GetScene().SetGravity(CVec2f(_value, mSlider2.GetValue()));
    });

    mSlider2.Connect(&GetUISystem());
    mSlider2.SetPos(CVec2i(10, 40));
    mSlider2.SetSize(CVec2ui(100, 20));
    mSlider2.SetMin(-50);
    mSlider2.SetMax(50);
    mSlider2.SetValue(-9.8);
    mSlider2.SetValueHandler([this] (float _value)
    {
        GetScene().SetGravity(CVec2f(mSlider1.GetValue(), _value));
    });

    return true;
}

void CTest3::Step(double _dt)
{
    CTest1::Step(_dt);
}

float angle = 0;

void CTest3::Render()
{
    CTest1::Render();

    CVec4f origin(40, 20, 0, 1);
    CVec4f pos(0, 0, 0, 1);
    CMatrix4f m;
    MatrixRotationX(m, angle);
    MatrixMultiply(m, origin, pos);
    GetDebugDrawSystem().DrawPoint(pos, 10, CColor4f(1, 0, 0), false);

    angle += 0.01;
}

void CTest3::SetProcessors()
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
            CPlane p(PlaneXY);
            p.SetPoint(mMoveObject->GetPos());

            CVec3f pos;

            GetDebugDrawSystem().CastRay(GetCursorPos(), p, pos);

            pos.Vec2() -= mMoveObject->GetMassCenter();
            pos *= 10;
            mMoveObject->SetLinearVelocity(pos);
        }
    },
    [this] ()
    {
        if (mMoveObject != nullptr)
        {
            /// if our body is not dynamic. it wil never stop, until we make it's velocity module to 0
            if (mMoveObject->IsDynamic() == false)
            {
                mMoveObject->SetLinearVelocity(CVec2f(0));
            }
            mMoveObject = nullptr;
        }
    }));

    GetEventSystem().SetProcessor("WHUP", CAppEventProcessor(
    [this] ()
    {
        CVec3f pos;

        if (GetDebugDrawSystem().GetActiveCam()->GetPos().IsTargetSet())
        {
            pos = GetDebugDrawSystem().GetActiveCam()->GetPos().GetTarget();
        }
        else
        {
            pos = GetDebugDrawSystem().GetActiveCam()->GetPos().Get();
        }

        pos.mZ += 10.0f;

        GetDebugDrawSystem().GetActiveCam()->GetPos().SetTarget(pos, 0.3, AnimatorBehavior::Single);
    }));

    GetEventSystem().SetProcessor("WHDN", CAppEventProcessor(
    [this] ()
    {
        CVec3f pos;

        if (GetDebugDrawSystem().GetActiveCam()->GetPos().IsTargetSet())
        {
            pos = GetDebugDrawSystem().GetActiveCam()->GetPos().GetTarget();
        }
        else
        {
            pos = GetDebugDrawSystem().GetActiveCam()->GetPos().Get();
        }

        pos.mZ -= 10.0f;

        GetDebugDrawSystem().GetActiveCam()->GetPos().SetTarget(pos, 0.3, AnimatorBehavior::Single);
    }));

    GetEventSystem().SetProcessor("C-q", CAppEventProcessor(
    [this] ()
    {
        this->Quit();
    }));

    GetEventSystem().SetProcessor("e", CAppEventProcessor(
    [this] ()
    {
    },
    [this] ()
    {
        if (GetDebugDrawSystem().GetActiveCam() != nullptr)
        {
            GetDebugDrawSystem().GetActiveCam()->Forward(5);
        }
    }));

    GetEventSystem().SetProcessor("q", CAppEventProcessor(
    [this] ()
    {
    },
    [this] ()
    {
        if (GetDebugDrawSystem().GetActiveCam() != nullptr)
        {
            GetDebugDrawSystem().GetActiveCam()->Forward(-5);
        }
    }));

    GetEventSystem().SetProcessor("z", CAppEventProcessor(
    [this] ()
    {
    },
    [this] ()
    {
        if (GetDebugDrawSystem().GetActiveCam() != nullptr)
        {
            GetDebugDrawSystem().GetActiveCam()->Strafe(5);
        }
    }));

    GetEventSystem().SetProcessor("c", CAppEventProcessor(
    [this] ()
    {
    },
    [this] ()
    {
        if (GetDebugDrawSystem().GetActiveCam() != nullptr)
        {
            GetDebugDrawSystem().GetActiveCam()->Strafe(-5);
        }
    }));
}

void CTest3::InitObjects()
{
    CSceneObjectGeometry sbg;
    sbg.mFigures.resize(1);
    sbg.mFigures[0].mDepth = 40;
    sbg.mFigures[0].mRestitution = 0.0;
    sbg.mFigures[0].mVertices.push_back( CVec2f( -300, -5 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2f( 300, -5 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2f( 300, 5 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2f( -300, 5 ) );
    CSceneObjectParams sbp;
    sbp.mDynamic = false;
    sbp.mAngle = 0;
    GetScene().CreateObject(sbg, sbp);

    sbp.mPos.Set(0, 600, 0);
    GetScene().CreateObject(sbg, sbp);

    sbg.mFigures[0].mVertices.clear();
    sbg.mFigures.resize(1);
    sbg.mFigures[0].mVertices.push_back( CVec2f( -5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2f( 5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2f( 5, 300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2f( -5, 300 ) );
    sbp.mPos.Set(-300, 300, 0);
    GetScene().CreateObject(sbg, sbp);

    sbg.mFigures[0].mVertices.clear();
    sbg.mFigures.resize(1);
    sbg.mFigures[0].mVertices.push_back( CVec2f( -5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2f( 5, -300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2f( 5, 300 ) );
    sbg.mFigures[0].mVertices.push_back( CVec2f( -5, 300 ) );
    sbp.mPos.Set(300, 300, 0);
    GetScene().CreateObject(sbg, sbp);

    CSceneObjectGeometry pg;
    pg.mFigures.resize(1);
    pg.mFigures[0].mMass = 3;
    CSceneObjectParams pp;
    pp.mPos.Set(-200, 100, 0);
    GetScene().CreateObject(pg, pp)->SetLinearVelocity( CVec2f( 200, 0 ) );

    CSceneObjectGeometry ppg;
    ppg.mFigures.resize(1);
    ppg.mFigures[0].mDepth = 2;
    CPlayerParams ppp;
    ppp.mSceneObjectParams.mPos.Set(0, 10, 0);
    ppp.mVelocityLimit = 10;
    GetPlayersSystem().CreatePlayer(ppg, ppp);
}

} // namespace test

} // namespace drash

