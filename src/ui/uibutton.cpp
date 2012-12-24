#include "uibutton.h"

#include "uisystem.h"
#include "../misc/vec2.h"
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
        b2Color c(mPressed ? 0 : mTime, mPressed ? 0.0f : 1.0f - mTime, mPressed ? 1 : 0);
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
