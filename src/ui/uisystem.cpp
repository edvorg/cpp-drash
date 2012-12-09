#include "uisystem.h"

#include "uicontrol.h"
#include "uiwidget.h"

namespace drash
{

bool CUISystem::Init()
{
    return true;
}

void CUISystem::Release()
{
    for (unsigned int i = 0; i < mControlsCount; i++)
    {
        delete mControls[i];
        mControls[i] = nullptr;
    }
    mControlsCount = 0;
}

bool CUISystem::ConnectWidget(CUIWidget &_widget)
{
    if (_widget.mControl != nullptr)
    {
        this->DisconnectWidget(_widget);
    }

    _widget.mControl = this->CreateControl();
    _widget.SetControlHandlers();

    return _widget.mControl != nullptr;
}

void CUISystem::DisconnectWidget(CUIWidget &_widget)
{
    DestroyControl(_widget.mControl);
    _widget.mControl = nullptr;
}

CUIControl *CUISystem::CreateControl()
{
    if (mControlsCount == mControlsCountLimit)
    {
        return nullptr;
    }

    return mControls[mControlsCount++] = new CUIControl;
}

void CUISystem::DestroyControl(CUIControl *_control)
{
    for (unsigned int i = 0; i < mControlsCount; i++)
    {
        if (mControls[i] == _control)
        {
            _control->mDestroyHandler();
            delete _control;
            mControls[i] = nullptr;
            mControlsCount--;
            return;
        }
    }
}

} // namespace drash
