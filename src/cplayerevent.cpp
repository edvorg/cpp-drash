#include "cplayerevent.h"

namespace drash{

CPlayerEvent::CPlayerEvent(const PlayerAction &_action,const CVec2 _posMouse):
    mType(_action),
    mPosMouse(_posMouse)
{
}

void CPlayerEvent::SetPosMouse(const CVec2 &_pos)
{
    mPosMouse = _pos;
}

CVec2 CPlayerEvent::GetPosMouse() const
{
    return mPosMouse;
}


}// namespace drash
