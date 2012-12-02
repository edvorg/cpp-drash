#include "appeventprocessor.h"

namespace drash
{

CAppEventProcessor::CAppEventProcessor()
{
}

CAppEventProcessor::CAppEventProcessor(CAppEventProcessor::CallbackT _event_begin, CAppEventProcessor::CallbackT _event, CAppEventProcessor::CallbackT _event_end):
    mEventBeginCallback(_event_begin),
    mEventCallback(_event),
    mEventEndCallback(_event_end)
{
}

CAppEventProcessor::CAppEventProcessor(const CAppEventProcessor::FuncT &_begin_pressing,
                                       const CAppEventProcessor::FuncT &_pressing,
                                       const CAppEventProcessor::FuncT &_end_pressing):
    mBeginPressing(_begin_pressing),
    mPressing(_pressing),
    mEndPressing(_end_pressing)
{
}

CAppEventProcessor::CAppEventProcessor(const CAppEventProcessor &_src):
    mBeginPressing(_src.mBeginPressing),
    mPressing(_src.mPressing),
    mEndPressing(_src.mEndPressing)
{
}

} // namespace drash
