#ifndef CGRENADE_H
#define CGRENADE_H

#include "cbullet.h"
#include "../ctimer.h"

namespace drash {

class CGrenadeParams : public CBulletParams
{
public:
    CGrenadeParams();
    unsigned int mBoomTime;
};

class CGrenade : public CBullet
{
public:
    friend class CScene;
    typedef CGrenadeParams ParamsT;

    CGrenade();

    virtual bool Init(const ParamsT &_params);
    virtual void BeginContact(const CContact &_contact);

    virtual void PostSolve(const CContact &_contact);
    virtual void Boom(const CBoomParams &_boom);
    virtual void Step(double _dt);

private:
    double mBoomTime;

    CTimer mTimerOfBoom;
    bool mActivatedTimer;

};


}// namespace drash
#endif // CGRENADE_H
