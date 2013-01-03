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

#include "test5.h"

#include "../debugdrawsystem/camera.h"

#include "../misc/plane.h"
#include "../misc/ray.h"

namespace drash
{

namespace test
{

bool CTest5::Init()
{
    if (CApp::Init() == false)
    {
        return false;
    }

    CPlane p;
    p.SetNormal(CVec3f(0, 0, 1));
    p.SetPoint(CVec3f(0));

    CVec3f cast_result;
    CRay r;

    r.SetDirection(CVec3f(1, 1, -1));
    r.SetPoint(CVec3f(0, 0, 1));
    p.CastRay(r, cast_result);
    LOG_INFO(cast_result);

    r.SetDirection(CVec3f(1, 0, -1));
    r.SetPoint(CVec3f(0, 0, 1));
    p.CastRay(r, cast_result);
    LOG_INFO(cast_result);

    r.SetDirection(CVec3f(-1, 0, -1));
    r.SetPoint(CVec3f(0, 0, 1));
    p.CastRay(r, cast_result);
    LOG_INFO(cast_result);

    r.SetDirection(CVec3f(0, -1, -1));
    r.SetPoint(CVec3f(0, 0, 1));
    p.CastRay(r, cast_result);
    LOG_INFO(cast_result);

    this->Quit();

    return true;
}

} // namespace test

} // namespace drash
