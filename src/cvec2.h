#ifndef CVEC2_H
#define CVEC2_H

namespace drash
{

class CVec2
{
public:
    CVec2();
    CVec2(float _val);
    CVec2(float _x, float _y);

    CVec2& set(float _x, float _y);

    float x;
    float y;
};

}// namespace drash

#endif // CVEC2_H
