#ifndef DRASH_APPEVENTCOMBINATION_H
#define DRASH_APPEVENTCOMBINATION_H

#include "appevent.h"
#include <vector>

namespace drash
{

class CAppEventCombination
{
public:
    constexpr static const unsigned int mCatchEventsCountLimit = 3;

    void AddEvent(const CAppEvent &_e);
    void RemoveEvent(const CAppEvent &_e);
    inline unsigned int GetEventsCount() const;
    void Clear();

    bool operator ==(const CAppEventCombination &_src);
private:
protected:
    std::vector<CAppEvent> mCatchEvents;
};

inline unsigned int CAppEventCombination::GetEventsCount() const
{
    return mCatchEvents.size();
}

} // namespace drash

#endif // DRASH_APPEVENTCOMBINATION_H
