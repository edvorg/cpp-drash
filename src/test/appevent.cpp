#include "appevent.h"

namespace drash
{

CAppEvent::CAppEvent()
{
}

CAppEvent::CAppEvent(const CAppEvent &_src):
    mType(_src.mType),
    mKey(_src.mKey),
    mButton(_src.mButton),
    mPos(_src.mPos)
{
}

CAppEvent::CAppEvent(EventType _type, EventKey _key)
{
    if (_type == EventKeyboard)
    {
        mType = _type;
        mKey = _key;
    }
}

CAppEvent::CAppEvent(EventType _type, EventButton _button, float _x, float _y)
{
    if (_type == EventMouse)
    {
        mType = _type;
        mButton = _button;
        mPos.Set(_x, _y);
    }
}

} // namespace drash
