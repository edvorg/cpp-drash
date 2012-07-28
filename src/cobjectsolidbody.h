#ifndef COBJECTRECTANGLE_H
#define COBJECTRECTANGLE_H

#include "csceneobject.h"

namespace drash
{

class CSolidBodyParams
{
public:
    bool mDynamic;
    CVec2 mPos;
    float mAngle;
    float mFriction;
    float mRestitution;
    float mMass;
};

class CObjectSolidBody : public CSceneObject
{
public:
    typedef CSolidBodyParams ParamsT;

    CObjectSolidBody(void);
    virtual ~CObjectSolidBody(void);

    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);
};

} // namespace drash

#endif // COBJECTRECTANGLE_H
