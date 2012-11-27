// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

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
    float mFov = M_PI / 4.0f;
    float mDepthOfView = 1000.0f;
};

class CCamera : public CSceneObject
{
public:
    friend class CScene;

    typedef CCameraParams ParamsT;

    inline void SetFov(float _fov);
    inline float GetFov() const;

    inline void SetDepthOfView(float _depth);
    inline float GetDepthOfView() const;

protected:        
    CCamera( void );

    bool Init( const GeometryT &_geometry, const ParamsT &_params );

private:
    float mFov = M_PI / 4.0f;
    float mDepthOfView = 1000.0f;
};

inline void CCamera::SetFov(float _fov)
{
    mFov = _fov;
}

inline float CCamera::GetFov() const
{
    return mFov;
}

inline void CCamera::SetDepthOfView(float _depth)
{
    mDepthOfView = drash::math::Abs(_depth);
}

inline float CCamera::GetDepthOfView() const
{
    return mDepthOfView;
}

}// namespace drash

#endif // COBJECTCAMERA_H
