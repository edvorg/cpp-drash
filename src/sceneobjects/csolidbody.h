#pragma once
#ifndef COBJECTRECTANGLE_H
#define COBJECTRECTANGLE_H

#include "csceneobject.h"

#include <vector>

namespace drash
{

class CSolidBodyParams : public CSceneObjectParams
{
public:
    std::vector<CFigureParams> mFigures;
};

class CSolidBody : public CSceneObject
{
public:
    friend class CScene;

    typedef CSolidBodyParams ParamsT;

protected:
    CSolidBody(void);
    virtual ~CSolidBody(void);

    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);

    virtual void OnBoom( const CBoomParams &_boom );
};

}// namespace drash

#endif // COBJECTRECTANGLE_H
