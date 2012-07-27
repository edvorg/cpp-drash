#include "cscene.h"
#include <assert.h>
#include "clogger.h"
#include "cobjectsolidbody.h"

namespace drash
{

CScene::CScene(void):
    mWorld(b2Vec2(0,0)),
    mInitialized(false)
{
}

CScene::~CScene(void)
{
    if ( mInitialized == true )
    {
        LOG_WARN("CScene::~CScene(): warning Release() called from destructor");
        Release();
    }
}

bool CScene::Init(const CSceneParams &_params)
{
    if ( mInitialized == true )
    {
        LOG_WARN("CScene::Init(): already initialized");
        return false;
    }

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

void CScene::Release(void)
{
    if ( mInitialized == false )
    {
        LOG_WARN("CScene::Release(): already released");
        return;
    }

    if ( mObjects.size() )
    {
        LOG_WARN("CScene::Release(): "<<
                 (unsigned int)mObjects.size()<<
                 " object(s) haven't been destroyed. Autorelease");

        while ( mObjects.size() )
        {
            DestroyObject<CSceneObject>(*mObjects.begin());
        }
    }

    mInitialized = false;
}

void CScene::Step(unsigned int dt)
{
    assert(mInitialized == true);

    mWorld.Step(static_cast<float>(dt) / 1000.0f, mVelocityIterations, mPositionIterations);
}

void CScene::Draw(void)
{
    assert(mInitialized == true);

    mWorld.DrawDebugData();
}

}// namespace drash
