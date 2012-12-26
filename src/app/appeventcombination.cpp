#include "appeventcombination.h"

#include <algorithm>
#include "../diag/logger.h"

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

bool CAppEventCombination::ContainsCombination(const CAppEventCombination &_c) const
{
    bool res = false;

    for (unsigned int i = 0; i < mCatchEvents.size(); i++)
    {
        if (mCatchEvents[i] == *_c.mCatchEvents.begin())
        {
            if (i + _c.mCatchEvents.size() <= mCatchEvents.size())
            {
                res = true;
                i++;

                for (unsigned int j = 1; j < _c.mCatchEvents.size(); j++, i++)
                {
                    if (mCatchEvents[i] != _c.mCatchEvents[j])
                    {
                        res = false;
                        break;
                    }
                }
            }

            break;
        }
    }

    return res;
}

bool CAppEventCombination::operator ==(const CAppEventCombination &_src) const
{
    if (mCatchEvents.size() != _src.mCatchEvents.size())
    {
        return false;
    }

    unsigned int last_elem = mCatchEvents.size()-1;

    if (mCatchEvents[0] != _src.mCatchEvents[0] ||
        mCatchEvents[last_elem] != _src.mCatchEvents[last_elem])
    {
        return false;
    }

    for (unsigned int i = 1; i < last_elem; i++)
    {
        if (mCatchEvents[i] != _src.mCatchEvents[i])
        {
            return false;
        }
    }

    return true;
}

bool CAppEventCombination::operator !=(const CAppEventCombination &_src) const
{
    return !(*this == _src);
}

CLogger &operator <<(CLogger &_logger, const CAppEventCombination &_c)
{
    auto i = _c.mCatchEvents.begin();

    if (i != _c.mCatchEvents.end())
    {
        _logger<<i->ToString().c_str();
        i++;

        while (i != _c.mCatchEvents.end())
        {
            _logger<<'-'<<i->ToString().c_str();
            i++;
        }
    }

    return _logger;
}

} // namespace drash
