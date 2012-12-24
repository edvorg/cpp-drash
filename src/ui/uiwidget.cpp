#include "uiwidget.h"

#include "uisystem.h"
#include "uicontrol.h"

namespace drash
{

namespace ui
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

void CUIWidget::SetStepHandler(const std::function<void (double)> &_handler)
{
    if (mUIControl != nullptr)
    {
        mUIControl->SetStepHandler(_handler);
    }
}

void CUIWidget::SetDrawHandler(const std::function<void ()> &_handler)
{
    if (mUIControl != nullptr)
    {
        mUIControl->SetDrawHandler(_handler);
    }
}

void CUIWidget::SetPos(const CVec2i &_pos)
{
    if (mUIControl != nullptr)
    {
        mUIControl->SetPos(_pos);
    }
}

void CUIWidget::SetSize(const CVec2ui &_size)
{
    if (mUIControl != nullptr)
    {
        mUIControl->SetSize(_size);
    }
}

const CVec2i CUIWidget::GetPos() const
{
    if (mUIControl != nullptr)
    {
        return mUIControl->GetPos();
    }
    else
    {
        return CVec2i(0);
    }
}

const CVec2ui CUIWidget::GetSize() const
{
    if (mUIControl != nullptr)
    {
        return mUIControl->GetSize();
    }
    else
    {
        return CVec2ui(0);
    }
}

} // namepsace ui

} // namespace drash
