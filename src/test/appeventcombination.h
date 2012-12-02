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
    void Clear();

    bool operator ==(const CAppEventCombination &_src);
private:
protected:
    std::vector<CAppEvent> mCatchEvents;
};

} // namespace drash

#endif // DRASH_APPEVENTCOMBINATION_H
