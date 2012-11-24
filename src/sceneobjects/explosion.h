#pragma once
#ifndef CBOOMPARAMS_H
#define CBOOMPARAMS_H

#include "csceneobject.h"

namespace drash
{

class CExplosionParams : public CSceneObjectParams
{
public:
    CExplosionParams();

    /// if this value is less than zero, bodies will move in direction of explosion
    float mStregth;
    float mLifeTime;
    /// maximal distance between explosion and object allowed for interation
    /// if this value is less than zero, we assume that it's infinite (positive)
    float mRadius;
};

class CExplosion : public CSceneObject
{
public:
    friend class CScene;

    typedef CExplosionParams ParamsT;

    virtual void ComputeBoundingBox() override;

protected:
    CExplosion(void);

    bool Init( const GeometryT &_geometry, const ParamsT &_params );

    virtual void Step( double _dt ) override;

    virtual void DrawDebug() const override;

    inline const CExplosionParams &GetParams() const;
    inline float GetTime() const;

private:
    CExplosionParams mParams;
    double mTime = 0;
};

inline const CExplosionParams &CExplosion::GetParams() const
{
    return mParams;
}

inline float CExplosion::GetTime() const
{
    return mTime;
}

}// namespace drash

#endif // CBOOMPARAMS_H
