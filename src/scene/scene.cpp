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

#include "scene.h"

#include "../diag/assert.h"
#include "../diag/logger.h"

#include "sceneobject.h"
#include "figure.h"

#include <Box2D/Box2D.h>

namespace drash
{

CScene::CScene():
    mWorld(b2Vec2(0, 0)),
    mObjectsFactory(mObjectsCountLimit, "CSceneObject")
{
}

CScene::~CScene()
{
}

bool CScene::Init(const CSceneParams & _params)
{
	Release();

    mWorld.SetContactFilter(this);
    mWorld.SetContactListener(this);

    mWorld.SetAllowSleeping(true);
    mWorld.SetContinuousPhysics(false);
    mWorld.SetGravity(CVec2ToB2Vec2(_params.mGravity));

    return mGeometryManager.Init();
}

void CScene::Release(void)
{
    DestroyObjects();
}

void CScene::Step( double _dt )
{
    if (mPaused == true)
    {
        return;
    }

    mLocked = true;

    for (unsigned int i = 0; i < mObjectsFactory.EnumObjects();)
    {
        if (mObjectsFactory.GetObjects()[i]->mDead)
        {
            DestroyObjectImpl(mObjectsFactory.GetObjects()[i]);
        }
        else
        {
            for (unsigned int j = 0; j < mObjectsFactory.GetObjects()[i]->mFiguresCount; j++)
            {
                if (mObjectsFactory.GetObjects()[i]->mFigures[j]->mDead == true)
                {
                    // detaching figure

                    CSceneObjectGeometry g;
                    g.mFigures.resize(1);
                    g.mFigures[0].mVertices.resize(mObjectsFactory.GetObjects()[i]->mFigures[j]->EnumVertices());
                    g.mFigures[0].mZ = mObjectsFactory.GetObjects()[i]->mFigures[j]->GetZ();
                    g.mFigures[0].mDepth = mObjectsFactory.GetObjects()[i]->mFigures[j]->GetDepth();
                    memcpy(&*g.mFigures[0].mVertices.begin(),
                           mObjectsFactory.GetObjects()[i]->mFigures[j]->GetVertices(),
                           sizeof(CVec2f) * mObjectsFactory.GetObjects()[i]->mFigures[j]->EnumVertices());

                    CSceneObjectParams p;
                    p.mAngle = mObjectsFactory.GetObjects()[i]->mAngle;
                    p.mDynamic = true;
                    p.mFixedRotation = false;
                    p.mPos = mObjectsFactory.GetObjects()[i]->mPos;

                    auto o = CreateObject(g, p);

                    o->SetLinearVelocity(mObjectsFactory.GetObjects()[i]->GetLinearVelocity());
                    o->SetAngularVelocity(mObjectsFactory.GetObjects()[i]->GetAngularVelocity());

                    mObjectsFactory.GetObjects()[i]->DestroyFigure(mObjectsFactory.GetObjects()[i]->mFigures[j]);

                    // recompute destruction graph (TODO: optimize this)

                    CSceneObjectGeometry ng;
                    mObjectsFactory.GetObjects()[i]->DumpGeometry(&ng);

                    ng.ComputeDestructionGraph(0.5);

                    mObjectsFactory.GetObjects()[i]->mDestructionGraph = ng.mDestructionGraph;

                    // looking for connectivity

                    std::vector<int> used;
                    used.resize(ng.mFigures.size());
                    memset(&*used.begin(), 0, sizeof(int) * ng.mFigures.size());
                    std::vector<int> comp;

                    std::function<void (unsigned int _v)> dfs;

                    dfs = [&] (unsigned int _v)
                    {
                        used[_v] = 1;
                        comp.push_back(_v);

                        for (unsigned int a = 0; a < ng.mFigures.size(); a++)
                        {
                            if (ng.mDestructionGraph[_v * ng.mFigures.size() + a] != 0)
                            {
                                if (used[a] == 0)
                                {
                                    dfs(a);
                                }
                            }
                        }
                    };

                    for (unsigned int a = 0; a < ng.mFigures.size(); a++)
                    {
                        if (used[a] == 0)
                        {
                            comp.clear();
                            dfs(a);

                            CSceneObjectGeometry g1;
                            g1.mFigures.resize(comp.size());

                            for (unsigned int b = 0; b < comp.size(); b++)
                            {
                                g1.mFigures[b].mVertices.resize(ng.mFigures[comp[b]].mVertices.size());
                                g1.mFigures[b].mZ = ng.mFigures[comp[b]].mZ;
                                g1.mFigures[b].mDepth = ng.mFigures[comp[b]].mDepth;
                                memcpy(&*g1.mFigures[b].mVertices.begin(),
                                       &*ng.mFigures[comp[b]].mVertices.begin(),
                                       sizeof(CVec2f) * ng.mFigures[comp[b]].mVertices.size());
                            }

                            g1.ComputeDestructionGraph(0.5);

                            CSceneObjectParams p1;
                            p1.mAngle = mObjectsFactory.GetObjects()[i]->mAngle;
                            p1.mDynamic = true;
                            p1.mFixedRotation = false;
                            p1.mPos = mObjectsFactory.GetObjects()[i]->mPos;

                            auto o = CreateObject(g1, p1);

                            o->SetLinearVelocity(mObjectsFactory.GetObjects()[i]->GetLinearVelocity());
                            o->SetAngularVelocity(mObjectsFactory.GetObjects()[i]->GetAngularVelocity());
                        }
                    }

                    DestroyObjectImpl(mObjectsFactory.GetObjects()[i]);

                    break;
                }
            }

            auto o = mObjectsFactory.GetObjects()[i];

            o->mLifeTime += _dt;

            bool phys_pos_changed = false;

            if (o->mPosXYAnimator.Step(_dt))
            {
                if (o->mPosXYAnimator.IsTargetSet() == true)
                {
                    CVec2f lv = o->mPosXYAnimator.GetTarget();
                    lv -= o->mPos;
                    lv /= o->mPosXYAnimator.GetTimeRemains();
                    o->mBody->SetLinearVelocity(CVec2ToB2Vec2(lv));
                }
                else
                {
                    o->mBody->SetLinearVelocity(b2Vec2(0, 0));
                    phys_pos_changed = true;
                }
            }
            else
            {
                o->mPos.Vec2() = B2Vec2ToCVec2(o->mBody->GetWorldPoint(b2Vec2(0, 0)));
            }

            if (o->mPosZAnimator.Step(_dt))
            {
                for (auto i = o->mCurrentContacts.begin(); i != o->mCurrentContacts.end(); i++)
                {
                    const CFigure *f1 = i->first;
                    const CFigure *f2 = i->second;

                    float z1 = f1->GetZ() + f1->GetSceneObject()->GetPosZ();
                    float z2 = f2->GetZ() + f2->GetSceneObject()->GetPosZ();

                    if (math::Abs(z1 - z2) > ((f1->GetDepth() + f2->GetDepth()) * 0.5f))
                    {
                        o->mBody->SetActive(false);
                        o->mBody->SetActive(true);
                    }
                }
            }

            if (o->mAngleAnimator.Step(_dt))
            {
                if (o->mAngleAnimator.IsTargetSet())
                {
                    float av = (o->mAngleAnimator.GetTarget() - o->mAngle) / o->mAngleAnimator.GetTimeRemains();
                    o->mBody->SetAngularVelocity(av);
                }
                else
                {
                    o->mBody->SetAngularVelocity(0);
                    phys_pos_changed = true;
                }
            }
            else
            {
                o->mAngle = o->mBody->GetAngle();
            }

            if (phys_pos_changed)
            {
                o->mBody->SetTransform(CVec2ToB2Vec2(o->mPos), o->mAngle);
            }

            i++;
        }
    }

    mLocked = false;

    mWorld.Step(_dt, mVelocityIterations, mPositionIterations);
}

CSceneObject* CScene::CreateObject(const CSceneObjectGeometry &_geometry, const CSceneObjectParams& _params)
{
    if (mWorld.IsLocked())
    {
        LOG_ERR("CScene::CreateObject(): world is locked now");
        return nullptr;
    }

    CSceneObject* res = mObjectsFactory.CreateObject();

    if (res == nullptr)
    {
        return nullptr;
    }

    b2BodyDef bdef;
    bdef.position = CVec2ToB2Vec2(_params.mPos);
    bdef.angle = _params.mAngle;
    bdef.active = true;
    bdef.awake = true;
    bdef.allowSleep = true;
    bdef.userData = res;
    bdef.angularDamping = 0;
    bdef.bullet = false;
    bdef.fixedRotation = _params.mFixedRotation;
    bdef.linearDamping = 0;
    bdef.gravityScale = 1;
    bdef.type = _params.mDynamic ? b2_dynamicBody : b2_kinematicBody;

    b2Body * b = mWorld.CreateBody(&bdef);

    if (b == nullptr)
    {
        LOG_ERR("CScene::CreateObject(): something wrong with box2d");
        return nullptr;
    }

    res->mBody = b;
    res->mPos = _params.mPos;
    res->mAngle = _params.mAngle;
    res->mDestructionGraph = _geometry.mDestructionGraph;

    for ( auto i = _geometry.mFigures.begin(), i_e = _geometry.mFigures.end(); i != i_e; i++ )
    {
        res->CreateFigure(*i);
    }

    return res;
}

bool CScene::DestroyObject(CSceneObject *_obj)
{
    if (mObjectsFactory.IsObject(_obj) == false)
    {
        LOG_ERR("CScene::DestroyObject(): invalid object taken");
        return false;
    }

    if (mLocked == false && mWorld.IsLocked() == false)
    {
        DestroyObjectImpl(_obj);
    }
    else
    {
        _obj->mDead = true;
    }

    return true;
}

void CScene::DestroyObjects(void)
{
    while (mObjectsFactory.EnumObjects() != 0)
    {
        DestroyObjectImpl(mObjectsFactory.GetObjects()[0]);
    }
}

void CScene::SetGravity(const CVec2f &_g)
{
    mWorld.SetGravity(CVec2ToB2Vec2(_g));
}

void CScene::DestroyObjectImpl(CSceneObject *_obj)
{
    for (auto & i : _obj->mDestroyHandlers)
    {
        i(_obj);
    }

    _obj->mBody->SetActive(false);

    while (_obj->EnumFigures() != 0)
    {
        _obj->DestroyFigure(_obj->GetFigures()[0]);
    }
    _obj->mBody->SetUserData(nullptr);

    mWorld.DestroyBody(_obj->mBody);

    _obj->mBody = nullptr;

    mObjectsFactory.DestroyObject(_obj);
}

bool CScene::ShouldCollide(b2Fixture * fixtureA, b2Fixture * fixtureB)
{
    if (fixtureA->GetUserData() == nullptr ||
        fixtureB->GetUserData() == nullptr ||
        fixtureA->GetBody()->GetUserData() == nullptr ||
        fixtureB->GetBody()->GetUserData() == nullptr)
    {
        return false;
    }

    CSceneObject *o1 = reinterpret_cast<CSceneObject*>(fixtureA->GetBody()->GetUserData());
    CSceneObject *o2 = reinterpret_cast<CSceneObject*>(fixtureB->GetBody()->GetUserData());

    CFigure *f1 = reinterpret_cast<CFigure*>( fixtureA->GetUserData() );
    CFigure *f2 = reinterpret_cast<CFigure*>( fixtureB->GetUserData() );

    float z1 = f1->GetZ() + o1->GetPosZ();
    float z2 = f2->GetZ() + o2->GetPosZ();

    return drash::math::Abs(z1 - z2) < (f1->GetDepth() * 0.5 + f2->GetDepth() * 0.5);
}

void CScene::BeginContact(b2Contact * _contact)
{
    b2ContactListener::BeginContact(_contact);

    CFigure *f1 = reinterpret_cast<CFigure*>(_contact->GetFixtureA()->GetUserData());
    CFigure *f2 = reinterpret_cast<CFigure*>(_contact->GetFixtureB()->GetUserData());

    if ( f1 == nullptr || f2 == nullptr )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "can not get pointer for one of figures. Skipping" );
        return;
    }

    if ( f1 == f2 || f1->GetSceneObject() == f2->GetSceneObject() )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "it's seems that pair of figures is part of same object" );
        return;
    }

    for (auto & i : f1->GetSceneObject()->mContactBeginHandlers)
    {
        i(f1, f2);
    }
    for (auto & i : f2->GetSceneObject()->mContactBeginHandlers)
    {
        i(f2, f1);
    }

    // ---------------------------------------------------------------------------------------------
    f1->GetSceneObject()->mCurrentContacts.insert(std::pair<const CFigure*, const CFigure*>(f2, f1));
    f2->GetSceneObject()->mCurrentContacts.insert(std::pair<const CFigure*, const CFigure*>(f1, f2));
    // ^
    // | this code block required for correct object movement along Z axis
    // | look for CSceneObject::() implementation
    // ---------------------------------------------------------------------------------------------

    CVec3f speed(f2->GetSceneObject()->GetLinearVelocity(), 0);
    speed.Vec2() -= f1->GetSceneObject()->GetLinearVelocity();

    if (speed.Length() > 10)
    {
        if (f1->GetSceneObject()->IsDynamic() &&
            f1->GetSceneObject()->mFiguresCount > 1 &&
            f1->GetSceneObject()->mLifeTime > 0.1)
        {
            f1->mDead = true;
        }

        if (f2->GetSceneObject()->IsDynamic() &&
            f2->GetSceneObject()->mFiguresCount > 1 &&
            f2->GetSceneObject()->mLifeTime > 0.1)
        {
            f2->mDead = true;
        }
    }
}

void CScene::PreSolve(b2Contact * _contact, const b2Manifold * _old_manifold)
{
    b2ContactListener::PreSolve(_contact, _old_manifold);

    /*
    CFigure *f1 = reinterpret_cast<CFigure*>(_contact->GetFixtureA()->GetUserData());
    CFigure *f2 = reinterpret_cast<CFigure*>(_contact->GetFixtureB()->GetUserData());

    if ( f1 == nullptr || f2 == nullptr )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "can not get pointer for one of figures. Skipping" );
        return;
    }

    if ( f1 == f2 || f1->GetSceneObject() == f2->GetSceneObject() )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "it's seems that pair of figures is part of same object" );
        return;
    }
    */
}

void CScene::PostSolve(b2Contact * _contact, const b2ContactImpulse * _impulse)
{
    b2ContactListener::PostSolve(_contact, _impulse);
}

void CScene::EndContact(b2Contact * _contact)
{
    b2ContactListener::EndContact(_contact);

    CFigure *f1 = reinterpret_cast<CFigure*>(_contact->GetFixtureA()->GetUserData());
    CFigure *f2 = reinterpret_cast<CFigure*>(_contact->GetFixtureB()->GetUserData());

    if ( f1 == nullptr || f2 == nullptr )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "can not get pointer for one of figures. Skipping" );
        return;
    }

    if ( f1 == f2 || f1->GetSceneObject() == f2->GetSceneObject() )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "it's seems that pair of figures is part of same object" );
        return;
    }

    for (auto & i : f1->GetSceneObject()->mContactEndHandlers)
    {
        i(f1, f2);
    }
    for (auto & i : f2->GetSceneObject()->mContactEndHandlers)
    {
        i(f2, f1);
    }


    // ---------------------------------------------------------------------------------------------
    auto f = f1->GetSceneObject()->mCurrentContacts.find(f2);

    if (f != f1->GetSceneObject()->mCurrentContacts.end())
    {
        f1->GetSceneObject()->mCurrentContacts.erase(f);
    }

    f = f2->GetSceneObject()->mCurrentContacts.find(f1);

    if (f != f2->GetSceneObject()->mCurrentContacts.end())
    {
        f2->GetSceneObject()->mCurrentContacts.erase(f);
    }
    // ^
    // | this code block required for correct object movement along Z axis
    // | look for CScene::Step() implementation
    // ---------------------------------------------------------------------------------------------
}

void CScene::SayGoodbye(b2Joint * _joint)
{
}

void CScene::SayGoodbye(b2Fixture * _fixture)
{
}

} // namespace drash
