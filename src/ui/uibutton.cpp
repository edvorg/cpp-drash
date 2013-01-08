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

#include "uibutton.h"

#include "uisystem.h"
#include "../misc/vec2.h"
#include "../misc/math.h"
#include "../debugrenderer/debugdrawsystem.h"
#include "../misc/color4.h"

namespace drash
{

namespace ui
{

void CUIButton::Connect(CUISystem *_system)
{
    CUIWidget::Connect(_system);

    SetStepHandler([this] (double _dt)
    {
        int local_x = GetUISystem()->GetCursorPosX() - GetPos().mX;
        int local_y = GetUISystem()->GetCursorPosY() - GetPos().mY;

        if (0 <= local_x && local_x <= static_cast<int>(GetSize().mX) &&
            0 <= local_y && local_y <= static_cast<int>(GetSize().mY))
        {
            mTime += _dt * 2.0f;
        }
        else
        {
            mTime -= _dt * 2.0f;
        }

        mTime = drash::math::Clamp<float>(mTime, 0, 1);
    });

    SetDrawHandler([this] ()
    {
        CColor4f c(mPressed ? 0 : mTime, mPressed ? 0.0f : 1.0f - mTime, mPressed ? 1 : 0, 1);
        CVec2f p1;
        CVec2f p2;
        CVec2f p3;
        CVec2f p4;

        GetUISystem()->UISpaceToScreenSpace(GetPos().mX, GetPos().mY, p1);
        GetUISystem()->UISpaceToScreenSpace(GetPos().mX, GetPos().mY + GetSize().mY, p2);
        GetUISystem()->UISpaceToScreenSpace(GetPos().mX + GetSize().mX, GetPos().mY + GetSize().mY, p3);
        GetUISystem()->UISpaceToScreenSpace(GetPos().mX + GetSize().mX, GetPos().mY, p4);

        GetUISystem()->GetDebugDrawSystem()->DrawTriangle(p4, p2, p1, c);
        GetUISystem()->GetDebugDrawSystem()->DrawTriangle(p3, p2, p4, c);

        GetUISystem()->GetDebugDrawSystem()->DrawLine(p1, p2, 1, c);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p3, p2, 1, c);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p3, p4, 1, c);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p1, p4, 1, c);
    });

    SetPressHandler([this] ()
    {
        mPressed = true;
    });

    SetReleaseHandler([this] ()
    {
        if (mPressed == true)
        {
            mPressed = false;

            int local_x = GetUISystem()->GetCursorPosX() - GetPos().mX;
            int local_y = GetUISystem()->GetCursorPosY() - GetPos().mY;

            if (0 <= local_x && local_x <= static_cast<int>(GetSize().mX) &&
                0 <= local_y && local_y <= static_cast<int>(GetSize().mY))
            {
                mClickHandler();
            }
        }
    });
}

void CUIButton::SetClickHandler(const std::function<void ()> &_handler)
{
    mClickHandler = _handler;
}

} // namepsace ui

} // namespace drash
