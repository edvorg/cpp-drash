#ifndef CSCENE_H
#define CSCENE_H

#include "csceneobject.h"
#include "cdebugrenderer.h"

namespace drash
{

class CSceneParams
{
public:
    CSceneParams():
        mGravity(0)
    {
    }

    CVec2 mGravity;
};

class CScene
{
public:
    CScene();

    bool Init(const CSceneParams& _params);
    void Release();

    CSceneObject* CreateObject(const CSceneObjectParams& _params);
    void DestroyObject(CSceneObject* _obj);

    /*
     * must be called once in update cycle
     * dt - microseconds
    */
    void Step(unsigned int dt);

    void Draw();

private:
    b2World mWorld;
    bool mInitialized;
    static const int mVelocityIterations = 1;
    static const int mPositionIterations = 1;
    CDebugRenderer mDebugRenderer;
};

}// namespace drash

#endif // CSCENE_H
