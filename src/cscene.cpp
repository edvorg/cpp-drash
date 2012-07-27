#include "cscene.h"
#include <assert.h>
#include "clogger.h"

namespace drash
{

CScene::CScene():
    mWorld(b2Vec2(0,0)),
    mInitialized(false)
{
}

bool CScene::Init(const CSceneParams &_params)
{
    mWorld.SetAllowSleeping(true);
    mWorld.SetContinuousPhysics(false);
    mWorld.SetGravity(_params.mGravity);

    if ( mDebugRenderer.Init() == false )
    {
        LOG_WARN("CScene::Init(): debug renderer init failed");
    }
    else
    {
        mWorld.SetDebugDraw(&mDebugRenderer);
    }

    mInitialized = true;
    return true;
}

void CScene::Release()
{
    while (mWorld.GetBodyCount())
    {
        CSceneObject* obj = reinterpret_cast<CSceneObject*>(mWorld.GetBodyList()->GetUserData());
        this->DestroyObject(obj);
    }

    mInitialized = false;
}

CSceneObject* CScene::CreateObject(const CSceneObjectParams &_params)
{
    assert(mInitialized == true);

    CSceneObject* obj = new CSceneObject();

    assert(obj != NULL);

    b2BodyDef bdef;
    bdef.userData = obj;
    bdef.position.Set(_params.mPos.x, _params.mPos.y);
    bdef.angle = _params.angle;
    bdef.angularVelocity = 0.0f;
    bdef.linearVelocity.SetZero();
    bdef.angularDamping = 0.0f;
    bdef.linearDamping = 0.0f;
    bdef.allowSleep = true;
    bdef.awake = true;
    bdef.fixedRotation = false;
    bdef.bullet = false;
    bdef.type = b2_dynamicBody;
    bdef.active = true;
    bdef.gravityScale = 1.0f;

    obj->mBody = mWorld.CreateBody(&bdef);

    assert(obj->mBody != NULL);

    return obj;
}

void CScene::DestroyObject(CSceneObject *_obj)
{
    assert(mInitialized == true);

    if (_obj == NULL)
    {
        return;
    }

    mWorld.DestroyBody(_obj->mBody);

    delete _obj;
    _obj = NULL;
}

void CScene::Step(unsigned int dt)
{
    assert(mInitialized == true);

    mWorld.Step(static_cast<float>(dt) / 1000.0f, mVelocityIterations, mPositionIterations);
}

void CScene::Draw()
{
    mWorld.DrawDebugData();
}

}// namespace drash
