#include "uiwidget.h"

#include "uicontrol.h"

namespace drash
{

void CUIWidget::SetControlHandlers()
{
    if (mControl != nullptr)
    {
        mControl->SetDestroyHandler([this] ()
        {
            this->mControl = nullptr;
        });
    }
}

void CUIWidget::SetPressHandler(const std::function<void ()> &_handler)
{
    if (mControl != nullptr)
    {
        mControl->SetPressHandler(_handler);
    }
}

void CUIWidget::SetDrawHandler(const std::function<void ()> &_handler)
{
    if (mControl != nullptr)
    {
        mControl->SetDrawHandler(_handler);
    }
}

} // namespace drash
