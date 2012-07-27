#include "cvec2.h"

namespace drash
{

CVec2::CVec2():
    x(0),
    y(0)
{
}

CVec2::CVec2(float32 _val):b2Vec2(_val,_val)
{
}

CVec2::CVec2(float32 _x, float32 _y):b2Vec2(_x,_y)
{
}


}// namespace drash
