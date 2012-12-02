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
    CAppEventProcessor(const FuncT &_begin_pressing,
                       const FuncT &_pressing,
                       const FuncT &_end_pressing);
    CAppEventProcessor(const CAppEventProcessor &_src);

    inline void BeginPressing();
    inline void Pressing();
    inline void EndPressing();
protected:
private:

    FuncT mBeginPressing = [] () {};
    FuncT mPressing = [] () {};
    FuncT mEndPressing = [] () {};
};

inline void CAppEventProcessor::BeginPressing()
{
    mBeginPressing();
}

inline void CAppEventProcessor::Pressing()
{
    mPressing();
}

inline void CAppEventProcessor::EndPressing()
{
    mEndPressing();
}

} // namespace drash

#endif // DRASH_APPEVENTPROCESSOR_H
