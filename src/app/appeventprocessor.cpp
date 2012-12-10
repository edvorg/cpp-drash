#include "appeventprocessor.h"

namespace drash
{

CAppEventProcessor::CAppEventProcessor()
{
}

CAppEventProcessor::CAppEventProcessor(const CAppEventProcessor::FuncT &_begin,
                                       const CAppEventProcessor::FuncT &_processing,
                                       const CAppEventProcessor::FuncT &_end):
    mBegin(_begin),
    mProcessing(_processing),
    mEnd(_end)
{
}

CAppEventProcessor::CAppEventProcessor(const CAppEventProcessor &_src):
    mBegin(_src.mBegin),
    mProcessing(_src.mProcessing),
    mEnd(_src.mEnd)
{
}

} // namespace drash
