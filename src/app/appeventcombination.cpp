#include "appeventcombination.h"

namespace drash
{

void CAppEventCombination::AddEvent(const CAppEvent &_e)
{
    if (mCatchEvents.size() < mCatchEventsCountLimit)
    {
        auto i = std::find(mCatchEvents.begin(), mCatchEvents.end(), _e);

        if (i != mCatchEvents.end())
        {
            return;
        }

        mCatchEvents.push_back(_e);
        std::sort(mCatchEvents.begin(), mCatchEvents.end());
    }
}

void CAppEventCombination::RemoveEvent(const CAppEvent &_e)
{
    auto i = std::find(mCatchEvents.begin(), mCatchEvents.end(), _e);

    if (i != mCatchEvents.end())
    {
        mCatchEvents.erase(i);
    }
}

void CAppEventCombination::Clear()
{
    mCatchEvents.clear();
}

bool CAppEventCombination::ContainsEvent(const CAppEvent &_e) const
{
    return std::find(mCatchEvents.begin(), mCatchEvents.end(), _e) != mCatchEvents.end();
}

bool CAppEventCombination::operator ==(const CAppEventCombination &_src) const
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
