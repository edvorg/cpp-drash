#include "cplayerevent.h"

namespace drash{

CPlayerEvent::CPlayerEvent(const PlayerAction &_action, const CVec2 _mousePos):
    mType(_action),
    mMousePos(_mousePos)
{
}

void CPlayerEvent::SetMousePos(const CVec2 &_pos)
{
    mMousePos = _pos;
}

CVec2 CPlayerEvent::GetMousePos() const
{
    return mMousePos;
}


}// namespace drash
