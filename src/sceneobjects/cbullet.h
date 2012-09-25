#pragma once
#ifndef CBULLET_H
#define CBULLET_H

#include "csolidbody.h"

namespace drash
{

class CBulletParams : public CSolidBodyParams
{
public:
    CBulletParams();

    CVec2 mTarget;
};

class CBullet : public CSolidBody
{
public:
    friend class CScene;

    typedef CBulletParams ParamsT;

protected:
    CBullet();

    virtual bool Init( const ParamsT &_params );

    virtual void OnContactBegin( const CContact &_contact );
    virtual void OnBoom( const CBoomParams &_boom );
};

}// namespace drash
#endif // CBULLET_H
