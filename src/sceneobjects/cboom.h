#pragma once
#ifndef CBOOMPARAMS_H
#define CBOOMPARAMS_H

#include "csceneobject.h"

namespace drash
{

class CBoomParams : public CSceneObjectParams
{
public:
    float mStregth;
    float mLifeTime;
};

class CBoom : public CSceneObject
{
public:
    friend class CScene;

    typedef CBoomParams ParamsT;

    CBoom(void);
    virtual ~CBoom(void);

protected:
    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);

    virtual void Step(double _dt);

private:
};

}// namespace drash

#endif // CBOOMPARAMS_H
