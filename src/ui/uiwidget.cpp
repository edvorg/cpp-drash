#include "uiwidget.h"

#include "uisystem.h"
#include "uicontrol.h"

namespace drash
{

CUIWidget::~CUIWidget()
{
    this->Disconnect();
}

void CUIWidget::Connect(CUISystem *_system)
{
    mUISystem = _system;
    mUIControl = mUISystem->CreateControl();
    mUIControl->SetDestroyHandler([this] ()
    {
        mUIControl = nullptr;
        mUISystem = nullptr;
    });
}

void CUIWidget::Disconnect()
{
    if (mUIControl != nullptr)
    {
        mUISystem->DestroyControl(mUIControl);
        mUISystem = nullptr;
        mUIControl = nullptr;
    }
}

CUISystem *CUIWidget::GetUISystem() const
{
    return mUISystem;
}

void CUIWidget::SetPressHandler(const std::function<void ()> &_handler)
{
    if (mUIControl != nullptr)
    {
        mUIControl->SetPressHandler(_handler);
    }
}

void CUIWidget::SetReleaseHandler(const std::function<void ()> &_handler)
{
    if (mUIControl != nullptr)
    {
        mUIControl->SetReleaseHandler(_handler);
    }
}

void CUIWidget::SetDrawHandler(const std::function<void ()> &_handler)
{
    if (mUIControl != nullptr)
    {
        mUIControl->SetDrawHandler(_handler);
    }
}

void CUIWidget::SetPos(unsigned int _x, unsigned int _y)
{
    if (mUIControl != nullptr)
    {
        mUIControl->SetPos(_x, _y);
    }
}

void CUIWidget::SetSize(unsigned int _w, unsigned int _h)
{
    if (mUIControl != nullptr)
    {
        mUIControl->SetSize(_w, _h);
    }
}

unsigned int CUIWidget::GetX() const
{
    if (mUIControl)
    {
        return mUIControl->GetX();
    }

    return 0;
}

unsigned int CUIWidget::GetY() const
{
    if (mUIControl)
    {
        return mUIControl->GetY();
    }

    return 0;
}

unsigned int CUIWidget::GetWidth() const
{
    if (mUIControl)
    {
        return mUIControl->GetWidth();
    }

    return 0;
}

unsigned int CUIWidget::GetHeight() const
{
    if (mUIControl)
    {
        return mUIControl->GetHeight();
    }

    return 0;
}

} // namespace drash
