#include "uicontrol.h"

namespace drash
{

void CUIControl::SetPos(int _x, int _y)
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
