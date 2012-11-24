#ifndef DRASH_APPEVENT_H
#define DRASH_APPEVENT_H

#include "../misc/cvec2.h"

namespace drash
{

enum EventType : unsigned int
{
    EventUnknown = 0,
    EventMouse = 1,
    EventKeyboard = 2,
};

enum EventKey : unsigned int
{
    EventKeyUnknown = 0,
    EventKeyW,
    EventKeyA,
    EventKeyS,
    EventKeyD,
    EventKeySpace,
};

class CAppEvent
{
public:
    CAppEvent();
    CAppEvent(const CAppEvent &_src);
    CAppEvent(EventType _type, EventKey _key);
    CAppEvent(EventType _type, float _x, float _y);

    inline EventType GetType() const;
    inline EventKey GetKey() const;
    inline const CVec2 &GetPos() const;

private:
    EventType mType = EventUnknown;
    EventKey mKey = EventKeyUnknown;
    CVec2 mPos = CVec2(0, 0);
};

inline EventType CAppEvent::GetType() const
{
    return mType;
}

inline EventKey CAppEvent::GetKey() const
{
    return mKey;
}

inline const CVec2 &CAppEvent::GetPos() const
{
    return mPos;
}

} // namespace drash

#endif // DRASH_APPEVENT_H
