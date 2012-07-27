#ifndef CSCENEOBJECT_H
#define CSCENEOBJECT_H

#include <Box2D/Box2D.h>
#include "cvec2.h"

namespace drash
{

class CSceneObjectParams
{
public:
    CSceneObjectParams():
        mPos(0),
        mSize(0),
        angle(0)
    {
    }

    CVec2 mPos;
    CVec2 mSize;

    /*
     * angle in radians
    */
    float angle;
};

class CSceneObject
{
public:
    friend class CScene;

private:
    CSceneObject();
    ~CSceneObject();

    b2Body* mBody;
};

}// namespace drash

#endif // CSCENEOBJECT_H
