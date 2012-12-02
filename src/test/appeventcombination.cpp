#include "appeventcombination.h"

namespace drash
{

void CAppEventCombination::AddEvent(const CAppEvent &_e)
{
    if (mCatchEvents.size() < mCatchEventsCountLimit)
    {
        mCatchEvents.push_back(_e);
        std::sort(mCatchEvents.begin(), mCatchEvents.end());
    }
}

bool CAppEventCombination::operator ==(const CAppEventCombination &_src)
{
    if (mCatchEvents.size() != _src.mCatchEvents.size())
    {
        return false;
    }

    for (unsigned int i = 0; i < mCatchEvents.size(); i++)
    {
        if (mCatchEvents[i] != _src.mCatchEvents[i])
        {
            return false;
        }
    }

    return true;
}

} // namespace drash
