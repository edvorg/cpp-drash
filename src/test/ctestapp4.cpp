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

#include "ctestapp4.h"

#include <sceneobjects.h>

namespace drash
{

void PressCallback()
{
    LOG_INFO("control pressed");
}

void PressingCallback()
{
    LOG_INFO("control pressing");
}

void ReleaseCallback()
{
    LOG_INFO("control released");
}

bool CTestApp4::Init()
{
    if (CApp::Init() == false)
    {
        return false;
    }

    GetEventSystem().SetProcessor("C", CAppEventProcessor(PressCallback, PressingCallback, ReleaseCallback));

    GetDebugDrawSystem().GetActiveCam()->SetZ(100);

    return true;
}

}// namespace drash
