#pragma once
#ifndef CGRENADE_H
#define CGRENADE_H

#include "cbullet.h"
#include "../diag/ctimer.h"

namespace drash
{

class CGrenadeParams : public CBulletParams
{
public:
    CGrenadeParams();

    double mTime;
};

class CGrenade : public CBullet
{
public:
    friend class CScene;

    typedef CGrenadeParams ParamsT;

protected:
    CGrenade();

    bool Init( const ParamsT &_params );

    virtual void OnContactBegin( const CContact &_contact );

    virtual void Step( double _dt );

private:
    double mTime;
    CTimer mTimer;
    int mCounter;

};


}// namespace drash
#endif // CGRENADE_H
