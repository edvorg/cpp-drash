#pragma once
#ifndef CBOOMPARAMS_H
#define CBOOMPARAMS_H

#include "csceneobject.h"

namespace drash
{

class CBoomParams : public CSceneObjectParams
{
public:
    CBoomParams();

    float mStregth;
    float mLifeTime;
};

class CExplosion : public CSceneObject
{
public:
    friend class CScene;

    typedef CBoomParams ParamsT;

protected:
    CExplosion(void);

    bool Init( const ParamsT &_params );

    virtual void Step( double _dt );

    const CBoomParams &GetParams() const;
    float GetTime() const;

private:
    CBoomParams mParams;
    float mTime;
};

}// namespace drash

#endif // CBOOMPARAMS_H
