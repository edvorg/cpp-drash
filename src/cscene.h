#ifndef CSCENE_H
#define CSCENE_H

#include "csceneobject.h"
#include <vector>

namespace drash
{

#define DRASH_BOX2D_USE_SAFE_DESTROY

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
    void step(unsigned int dt);

    static const int mVelocityIterations = 1;
    static const int mPositionIterations = 1;

private:
    b2World mWorld;
    bool mInitialized;
};

}// namespace drash

#endif // CSCENE_H
