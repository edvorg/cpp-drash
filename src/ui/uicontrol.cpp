#include "uicontrol.h"

namespace drash
{

void CUIControl::SetPos(unsigned int _x, unsigned int _y)
{
    mX = _x;
    mY = _y;
}

void CUIControl::SetSize(unsigned int _w, unsigned int _h)
{
    mWidth = _w;
    mHeight = _h;
}

} // namespace drash
