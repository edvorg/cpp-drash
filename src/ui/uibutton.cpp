#include "uibutton.h"

namespace drash
{

void CUIButton::Connect(CUISystem *_system)
{
    CUIWidget::Connect(_system);
}

void CUIButton::SetClickHandler(const std::function<void ()> &_handler)
{
    mClickHandler = _handler;
}

} // namespace drash
