#pragma once
#ifndef COBJECTCAMERA_H
#define COBJECTCAMERA_H

#include "csceneobject.h"
#include "../misc/canimatedparam.h"

namespace drash
{

class CCameraParams : public CSceneObjectParams
{
public:
    CCameraParams();
};

class CCamera : public CSceneObject
{
public:
    friend class CScene;

    typedef CCameraParams ParamsT;

    inline void SetFov(float _fov);
    inline float GetFov() const;

protected:        
    CCamera( void );

    bool Init( const GeometryT &_geometry, const ParamsT &_params );

private:
    float mFov = M_PI / 3.0f;
};

inline void CCamera::SetFov(float _fov)
{
    mFov = _fov;
}

inline float CCamera::GetFov() const
{
    return mFov;
}

}// namespace drash

#endif // COBJECTCAMERA_H
