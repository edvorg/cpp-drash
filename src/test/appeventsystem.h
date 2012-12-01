#ifndef DRASH_APPEVENTSYSTEM_H
#define DRASH_APPEVENTSYSTEM_H

#include "appevent.h"
#include <list>

namespace drash
{

class CAppEventProcessor;

class CAppEventSystem
{
public:
    void SetProcessor(const char *_combination, const CAppEventProcessor &_processor);

    void Process();

    void PressEvent(const CAppEvent &_event);
    void ReleaseEvent(const CAppEvent &_event);

protected:
private:
    std::list<CAppEvent> mEvents;
};

} // namespace drash

#endif // DRASH_APPEVENTSYSTEM_H
