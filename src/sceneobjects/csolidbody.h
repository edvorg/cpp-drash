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
    float mFriction;
    float mRestitution;
    float mMass;

    /// if size of mVertices is NULL, creates a box with (1, 1) dimentions
    std::vector<CVec2> mVertices;

    CSolidBodyParams();
};

class CSolidBody : public CSceneObject
{
public:
    friend class CScene;

    typedef CSolidBodyParams ParamsT;

protected:
    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);
    virtual void Boom(const CBoomParams &_boom);
    CSolidBody(void);
    virtual ~CSolidBody(void);
};

}// namespace drash

#endif // COBJECTRECTANGLE_H
