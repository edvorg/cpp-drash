#include "cscene.h"
#include <assert.h>

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
    mWorld.SetGravity(b2Vec2(_params.mGravity.x, _params.mGravity.y));

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

#ifdef DRASH_BOX2D_USE_SAFE_DESTROY
    // look for body in mWorld, delete if it only exists
    bool notfound = true;

    for (b2Body* b=mWorld.GetBodyList(); b; b++)
    {
        if (b == _obj->mBody)
        {
            notfound = false;
            break;
        }
    }

    if (notfound)
    {
        return;
    }
#endif

    mWorld.DestroyBody(_obj->mBody);

    delete _obj;
    _obj = NULL;
}

void CScene::step(unsigned int dt)
{
    assert(mInitialized == true);

    mWorld.Step(static_cast<float>(dt) * 1000.0f, mVelocityIterations, mPositionIterations);
}

}// namespace drash
