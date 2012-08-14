#ifndef CBULLET_H
#define CBULLET_H

#include "../cscene.h"

namespace drash{

class CBulletParams : public CSolidBodyParams
{
public:
    CVec2 mTarget;
    CBulletParams();
};

class CBullet : public CSolidBody
{
public:
    friend class CScene;
    typedef CBulletParams ParamsT;


    CBullet();
    virtual bool Init(const ParamsT &_params);
    virtual void BeginContact(const CContact &_contact);

    virtual void PostSolve(const CContact &_contact);
    virtual void Boom(const CBoomParams &_boom);
};

}// namespace drash
#endif // CBULLET_H
