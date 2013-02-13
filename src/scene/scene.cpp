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
#include "joint.h"

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

    return true;
}

void CScene::Release(void)
{
    while (auto j = mWorld.GetJointList())
    {
        delete reinterpret_cast<CJoint*>(j->GetUserData());
        j->SetUserData(nullptr);
        mWorld.DestroyJoint(j);
    }

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

            mObjectsFactory.GetObjects()[i]->Step(_dt);
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

CJoint *CScene::CreateJoint(CSceneObject *_obj1, CSceneObject *_obj2, const CVec3f &_anchor)
{
    b2WeldJointDef jdef;
    jdef.Initialize(_obj1->mBody, _obj2->mBody, CVec2ToB2Vec2(_anchor.Vec2()));

    CJoint *res = new CJoint;
    res->mJoint = mWorld.CreateJoint(&jdef);;
    res->mJoint->SetUserData(res);
    return res;
}

CJoint *CScene::CreateJointDistance(CSceneObject *_obj1, CSceneObject *_obj2, const CVec3f &_anchor1, const CVec3f &_anchor2, float _length)
{
    b2DistanceJointDef jdef;
    jdef.Initialize(_obj1->mBody, _obj2->mBody, CVec2ToB2Vec2(_anchor1.Vec2()), CVec2ToB2Vec2(_anchor2.Vec2()));
    jdef.length = _length;

    CJoint *res = new CJoint;
    res->mJoint = mWorld.CreateJoint(&jdef);;
    res->mJoint->SetUserData(res);
    return res;
}

CJoint *CScene::CreateJointRope(CSceneObject *_obj1, CSceneObject *_obj2, const CVec3f &_anchor1, const CVec3f &_anchor2, float _length)
{
    b2RopeJointDef jdef;
    jdef.maxLength = _length;
    jdef.localAnchorA = CVec2ToB2Vec2(_anchor1.Vec2());
    jdef.localAnchorB = CVec2ToB2Vec2(_anchor2.Vec2());
    jdef.bodyA = _obj1->mBody;
    jdef.bodyB = _obj2->mBody;
    jdef.type = e_ropeJoint;
    jdef.collideConnected = false;

    CJoint *res = new CJoint;
    res->mJoint = mWorld.CreateJoint(&jdef);;
    res->mJoint->SetUserData(res);
    return res;
}

void CScene::DestroyJoint(CJoint *_joint)
{
    for (auto j=mWorld.GetJointList(); j!=nullptr; j=j->GetNext())
    {
        if (j->GetUserData() == _joint)
        {
            delete reinterpret_cast<CJoint*>(j->GetUserData());
            mWorld.DestroyJoint(j);
            return;
        }
    }
}

void CScene::SetGravity(const CVec2f &_g)
{
    mWorld.SetGravity(CVec2ToB2Vec2(_g));
}

void CScene::DestroyObjectImpl(CSceneObject *_obj)
{    
    _obj->mBody->SetActive(false);

    while (b2Fixture *f = _obj->mBody->GetFixtureList())
    {
        delete reinterpret_cast<CFigure*>(f->GetUserData());
        f->SetUserData(nullptr);
        _obj->mBody->DestroyFixture(f);
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

    f1->GetSceneObject()->OnContactBegin(f1, f2);
    f2->GetSceneObject()->OnContactBegin(f2, f1);
}

void CScene::PreSolve(b2Contact * _contact, const b2Manifold * _old_manifold)
{
    b2ContactListener::PreSolve(_contact, _old_manifold);

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

    f1->GetSceneObject()->OnContactPreSolve(f1, f2);
    f2->GetSceneObject()->OnContactPreSolve(f2, f1);
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

    f1->GetSceneObject()->OnContactEnd(f1, f2);
    f2->GetSceneObject()->OnContactEnd(f2, f1);
}

} // namespace drash
