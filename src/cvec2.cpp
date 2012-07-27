#include "cvec2.h"

namespace drash
{

CVec2::CVec2():
    x(0),
    y(0)
{
}

CVec2::CVec2(float _val):
    x(_val),
    y(_val)
{
}

CVec2::CVec2(float _x, float _y):
    x(_x),
    y(_y)
{
}


CVec2 &CVec2::set(float _x, float _y)
{
    x = _x;
    y = _x;
}

}// namespace drash
