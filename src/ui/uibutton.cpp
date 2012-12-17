#include "uibutton.h"

#include "uisystem.h"
#include "../misc/cvec2.h"
#include "../misc/math.h"
#include "../debugdrawsystem/debugdrawsystem.h"

namespace drash
{

namespace ui
{

void CUIButton::Connect(CUISystem *_system)
{
    CUIWidget::Connect(_system);

    SetStepHandler([this] (double _dt)
    {
        int local_x = GetUISystem()->GetCursorPosX() - GetX();
        int local_y = GetUISystem()->GetCursorPosY() - GetY();

        if (0 <= local_x && local_x <= static_cast<int>(GetWidth()) &&
            0 <= local_y && local_y <= static_cast<int>(GetHeight()))
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
        b2Color c(mPressed ? 0 : mTime, mPressed ? 0.0f : 1.0f - mTime, mPressed ? 1 : 0);
        CVec2 p1;
        CVec2 p2;
        CVec2 p3;
        CVec2 p4;
        GetUISystem()->UISpaceToScreenSpace(GetX(), GetY(), p1);
        GetUISystem()->UISpaceToScreenSpace(GetX(), GetY() + GetHeight(), p2);
        GetUISystem()->UISpaceToScreenSpace(GetX() + GetWidth(), GetY() + GetHeight(), p3);
        GetUISystem()->UISpaceToScreenSpace(GetX() + GetWidth(), GetY(), p4);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p1, p2, c);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p3, p2, c);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p3, p4, c);
        GetUISystem()->GetDebugDrawSystem()->DrawLine(p1, p4, c);
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

            int local_x = GetUISystem()->GetCursorPosX() - GetX();
            int local_y = GetUISystem()->GetCursorPosY() - GetY();

            if (0 <= local_x && local_x <= static_cast<int>(GetWidth()) &&
                0 <= local_y && local_y <= static_cast<int>(GetHeight()))
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
