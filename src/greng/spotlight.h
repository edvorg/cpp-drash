#ifndef GRENG_SPOTLIGHT_H
#define GRENG_SPOTLIGHT_H

namespace greng
{

class CSpotLight
{
public:
    CSpotLight() = default;

    drash::CVec3f mPosition = drash::CVec3f(0, 0, 0);
    drash::CVec3f mDirection = drash::CVec3f(0, 0, -1);
    drash::CColor3f mColor = drash::CColor3f(1, 1, 1);
};

} // namespace greng

#endif // GRENG_SPOTLIGHT_H
