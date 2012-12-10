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
        this->Disconnect();
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

} // namespace drash
