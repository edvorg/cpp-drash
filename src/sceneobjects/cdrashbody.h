#pragma once
#ifndef CDRASHBODY_H
#define CDRASHBODY_H

#include "csceneobject.h"
#include "explosion.h"
#include <vector>

namespace drash
{

class CDrashBodyParams : public CSceneObjectParams
{
public:
    CDrashBodyParams();

    std::vector<CDrashBodyParams> mChilds;
    CVec2 mLocalPos;
    double mDestroyDelay;
    float mDestroySpeed;
};

class CDrashBody : public CSceneObject
{
public:
    friend class CScene;

    typedef CDrashBodyParams ParamsT;

protected:
    CDrashBody();

    bool Init( const GeometryT &_geometry, const ParamsT &_params );

    virtual void Step( double _dt ) override;

    virtual void OnContactBegin( const CContact &_contact ) override;

private:
    int mCounter;
    CVec2 mLastVelocity;
    float mLastAngularVelocity;
    ParamsT mParams;
    double mTime = 0;
    std::vector<CExplosionParams> mBoomParams;
};

} // namespace drash

#endif // CDRASHBODY_H
