#include "uibutton.h"

#include "uisystem.h"
#include "../misc/cvec2.h"
#include "../subsystem/debugdrawsystem.h"

namespace drash
{

void CUIButton::Connect(CUISystem *_system)
{
    CUIWidget::Connect(_system);

    SetDrawHandler([this] ()
    {
        b2Color c(mPressed ? 0 : 1, mPressed ? 1 : 0, 0);
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

            int local_x = static_cast<int>(GetUISystem()->GetCursorPosX()) - GetX();
            int local_y = static_cast<int>(GetUISystem()->GetCursorPosY()) - GetY();

            if (0 <= local_x && local_x <= GetWidth() &&
                0 <= local_y && local_y <= GetHeight())
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

} // namespace drash
