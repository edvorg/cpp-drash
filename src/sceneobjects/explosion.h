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
