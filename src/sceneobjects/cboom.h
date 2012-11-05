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

class CBoom : public CSceneObject
{
public:
    friend class CScene;

    typedef CBoomParams ParamsT;

protected:
    CBoom(void);

    bool Init( const ParamsT &_params );

private:
};

}// namespace drash

#endif // CBOOMPARAMS_H
