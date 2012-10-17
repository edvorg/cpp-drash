#pragma once
#ifndef CBULLET_H
#define CBULLET_H

#include "csceneobject.h"

namespace drash
{

class CBulletParams : public CSceneObjectParams
{
public:
    CBulletParams();

    CVec2 mTarget;
};

class CBullet : public CSceneObject
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
