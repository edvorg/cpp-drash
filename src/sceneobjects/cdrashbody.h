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
#ifndef CDRASHBODY_H
#define CDRASHBODY_H

#include "csceneobject.h"
#include <vector>

namespace drash
{

//class CDrashBodyTemplate;

class CDrashBodyParams : public CSceneObjectParams
{
public:
    double mDestroyDelay = 0;
    float mDestroySpeed = 0;
};

class CDrashBodyGeometry;

class CDrashBodyChild
{
public:
    const CDrashBodyGeometry *mTemplate = nullptr;
    CDrashBodyParams mParams;
};

class CDrashBodyGeometry : public CSceneObjectGeometry
{
public:
    std::vector<CDrashBodyChild> mDestructionChilds;
};

class CDrashBody : public CSceneObject
{
public:
    friend class CScene;

    typedef CDrashBodyParams ParamsT;
    typedef CDrashBodyGeometry GeometryT;

protected:
    CDrashBody();

    bool Init( const GeometryT &_geometry, const ParamsT &_params );

    virtual void Step( double _dt ) override;

    virtual void OnContactBegin(const CFigure *_f1, const CFigure *_f2) override;

private:
    int mCounter = 0;
    CVec2 mLastVelocity = CVec2(0);
    float mLastAngularVelocity = 0;
    std::vector<CDrashBodyChild> mDestructionChilds;
    double mDestroyDelay = 0;
    float mDestroySpeed = 0;
    double mTime = 0;
};

} // namespace drash

#endif // CDRASHBODY_H
