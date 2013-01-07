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

#include "scene.h"

#include "../diag/assert.h"
#include "../diag/logger.h"

#include "sceneobject.h"
#include "figure.h"
#include "joint.h"

#include <Box2D/Box2D.h>
#include "physobserver.h"

namespace drash
{

CScene::CScene():
    mObjectsFactory(mObjectsCountLimit, "CSceneObject")
{
}

bool CScene::Init( const CSceneParams &_params )
{
	Release();

    mObserver = new CPhysObserver();
    mWorld = new b2World(b2Vec2(0, 0));

    mWorld->SetContactFilter(mObserver);
    mWorld->SetContactListener(mObserver);
    mWorld->SetAllowSleeping(true);
    mWorld->SetContinuousPhysics(false);
    mWorld->SetGravity(CVec2ToB2Vec2(_params.mGravity));

    return true;
}

void CScene::Release(void)
{
	if (mWorld != nullptr)
	{
        while (mWorld->GetJointCount())
        {
            delete reinterpret_cast<CJoint*>(mWorld->GetJointList()->GetUserData());
            mWorld->GetJointList()->SetUserData(nullptr);
            mWorld->DestroyJoint(mWorld->GetJointList());
        }

        DestroyObjects();

        delete mWorld;
	}

    delete mObserver;
}

void CScene::Step( double _dt )
{
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

                    CreateObject(g, p);

                    mObjectsFactory.GetObjects()[i]->DestroyFigure(mObjectsFactory.GetObjects()[i]->mFigures[j]);

                    break;
                }
            }

            mObjectsFactory.GetObjects()[i]->Step(_dt);
            i++;
        }
    }

    mLocked = false;

    mWorld->Step( _dt, mVelocityIterations, mPositionIterations );
}

CSceneObject* CScene::CreateObject(const CSceneObjectGeometry &_geometry, const CSceneObjectParams& _params)
{
    if (mWorld->IsLocked())
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

    b2Body *b = mWorld->CreateBody(&bdef);

    if (b == nullptr)
    {
        LOG_ERR("CScene::CreateObject(): something wrong with box2d");
        return nullptr;
    }

    res->mBody = b;
    res->mPos = _params.mPos;
    res->mAngle = _params.mAngle;

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

    if (mLocked == false && mWorld->IsLocked() == false)
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
    res->mJoint = mWorld->CreateJoint(&jdef);;
    res->mJoint->SetUserData(res);
    return res;
}

void CScene::DestroyJoint(CJoint *_joint)
{
    for (auto j=mWorld->GetJointList(); j!=nullptr; j=j->GetNext())
    {
        if (j->GetUserData() == _joint)
        {
            delete reinterpret_cast<CJoint*>(j->GetUserData());
            mWorld->DestroyJoint(j);
            return;
        }
    }
}

void CScene::SetGravity(const CVec2f &_g)
{
    mWorld->SetGravity(CVec2ToB2Vec2(_g));
}

void CScene::DestroyObjectImpl(CSceneObject *_obj)
{
    while (b2Fixture *f = _obj->mBody->GetFixtureList())
    {
        delete reinterpret_cast<CFigure*>(f->GetUserData());
        f->SetUserData(nullptr);
        _obj->mBody->DestroyFixture(f);
    }

    _obj->mBody->SetActive(false);
    _obj->mBody->SetUserData(nullptr);

    mWorld->DestroyBody(_obj->mBody);

    _obj->mBody = nullptr;

    mObjectsFactory.DestroyObject(_obj);
}

} // namespace drash
