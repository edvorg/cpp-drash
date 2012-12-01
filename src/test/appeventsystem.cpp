#include "appeventsystem.h"

namespace drash
{

void CAppEventSystem::SetProcessor(const char *_combination, const CAppEventProcessor &_processor)
{
}

void CAppEventSystem::Process()
{
}

void CAppEventSystem::PressEvent(const CAppEvent &_event)
{
    auto i = std::find(mEvents.begin(), mEvents.end(), _event);

    if (i == mEvents.end())
    {
        mEvents.push_back(_event);
    }
}

void CAppEventSystem::ReleaseEvent(const CAppEvent &_event)
{
    auto i = std::find(mEvents.begin(), mEvents.end(), _event);

    if (i != mEvents.end())
    {
        mEvents.erase(i);
    }
}

} // namespace drash
