#ifndef DRASH_APPEVENTPROCESSOR_H
#define DRASH_APPEVENTPROCESSOR_H

#include <functional>

namespace drash
{

class CAppEventProcessor
{
public:
    typedef std::function<void ()> FuncT;

    CAppEventProcessor();
    CAppEventProcessor(const FuncT &_begin,
                       const FuncT &_processing = [] () {},
                       const FuncT &_end = [] () {});
    CAppEventProcessor(const CAppEventProcessor &_src);

    inline void Begin();
    inline void Processing();
    inline void End();
protected:
private:

    FuncT mBegin = [] () {};
    FuncT mProcessing = [] () {};
    FuncT mEnd = [] () {};
};

inline void CAppEventProcessor::Begin()
{
    mBegin();
}

inline void CAppEventProcessor::Processing()
{
    mProcessing();
}

inline void CAppEventProcessor::End()
{
    mEnd();
}

} // namespace drash

#endif // DRASH_APPEVENTPROCESSOR_H
