#ifndef CSCENE_H
#define CSCENE_H

#include "csceneobject.h"
#include "cdebugrenderer.h"
#include <vector>

namespace drash
{

class CSceneParams
{
public:
    CVec2 mGravity;
};

class CScene
{
public:
    typedef std::vector<CSceneObject*> ObjectsT;

    CScene(void);
    virtual ~CScene(void);

    bool Init(const CSceneParams& _params);
    void Release(void);

    /// T must extend CSceneObject class
    template <typename T>
    T* CreateObject(void);

    /// T must extend CSceneObject class
    template <typename T>
    void DestroyObject(T* _obj);

    /// must be called once in update cycle
    /// dt - microseconds
    void Step(unsigned int dt);

    void Draw(void);

protected:

private:
    b2World mWorld;
    bool mInitialized;
    static const int mVelocityIterations = 1;
    static const int mPositionIterations = 1;
    CDebugRenderer mDebugRenderer;
    ObjectsT mObjects;
};

template <typename T>
T* CScene::CreateObject(void)
{
    T* res = new T();

    b2BodyDef bdef;
    res->mBody = mWorld.CreateBody(&bdef);

    if ( res->mBody == NULL || res->Init() == false )
    {
        delete res;
        return NULL;
    }

    mObjects.push_back(res);

    return res;
}

template <typename T>
void CScene::DestroyObject(T* _obj)
{
    for ( ObjectsT::iterator i=mObjects.begin(), i_e=mObjects.end(); i!=i_e; i++ )
    {
        if ( (*i) == _obj )
        {
            // ok, it's our object

            b2Body* body = _obj->mBody;
            mObjects.erase(i);
            _obj->Release();
            delete _obj;

            for ( b2Body *b = mWorld.GetBodyList(); b; b = b->GetNext() )
            {
                if ( b == body )
                {
                    // ok, it's out body

                    mWorld.DestroyBody(body);
                    break;
                }
            }

            break;
        }
    }
}

}// namespace drash

#endif // CSCENE_H
