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
#include "app/appeventprocessor.h"

namespace drash
{

bool CTestApp4::Init()
{
    if (CApp::Init() == false)
    {
        return false;
    }

    GetEventSystem().SetProcessor("C-x C-c", CAppEventProcessor(
    [] ()
    {
      LOG_INFO("C-x C-c pressed");
    },
    [] () {},
    [] ()
    {
      LOG_INFO("C-x C-c released");
    }
    ));

    GetEventSystem().SetProcessor("C-x C-s", CAppEventProcessor(
    [] ()
    {
        LOG_INFO("C-x C-s pressed");
    },
    [] () {},
    [] ()
    {
        LOG_INFO("C-x C-s released");
    }
    ));

    GetEventSystem().SetProcessor("C-s", CAppEventProcessor(
    [] ()
    {
        LOG_INFO("C-s pressed");
    },
    [] () {},
    [] ()
    {
        LOG_INFO("C-s released");
    }
    ));

    GetEventSystem().SetProcessor("C-s C-d", CAppEventProcessor(
    [] ()
    {
        LOG_INFO("C-s C-d pressed");
    },
    [] () {},
    [] ()
    {
        LOG_INFO("C-s C-d released");
    }
    ));

    GetEventSystem().SetProcessor("C-f", CAppEventProcessor(
    [] ()
    {
        LOG_INFO("C-f pressed");
    },
    [] () {},
    [] ()
    {
        LOG_INFO("C-f release");
    }));

    GetEventSystem().SetProcessor("f", CAppEventProcessor(
    [] ()
    {
        LOG_INFO("f pressed");
    },
    [] () {},
    [] ()
    {
        LOG_INFO("f released");
    }
    ));

    GetEventSystem().SetProcessor("w", CAppEventProcessor(
    [] ()
    {
        LOG_INFO("w pressed");
    },
    [] () {},
    [] ()
    {
        LOG_INFO("w released");
    }
    ));

    GetDebugDrawSystem().GetActiveCam()->SetZ(100);

    return true;
}

}// namespace drash
