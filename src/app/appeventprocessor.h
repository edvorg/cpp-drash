#ifndef DRASH_APPEVENTPROCESSOR_H
#define DRASH_APPEVENTPROCESSOR_H

#include <functional>

namespace drash
{

class CAppEventProcessor
{
public:
    typedef std::function<void ()> FuncT;
    typedef void (*CallbackT)();

    CAppEventProcessor();
    CAppEventProcessor(CallbackT _event_begin,
                       CallbackT _event,
                       CallbackT _event_end);
    CAppEventProcessor(const FuncT &_begin_pressing,
                       const FuncT &_pressing = [] () {},
                       const FuncT &_end_pressing = [] () {});
    CAppEventProcessor(const CAppEventProcessor &_src);

    inline void BeginPressing();
    inline void Pressing();
    inline void EndPressing();
protected:
private:

    FuncT mBeginPressing = [] () {};
    FuncT mPressing = [] () {};
    FuncT mEndPressing = [] () {};
    CallbackT mEventBeginCallback = nullptr;
    CallbackT mEventCallback = nullptr;
    CallbackT mEventEndCallback = nullptr;
};

inline void CAppEventProcessor::BeginPressing()
{
    mEventBeginCallback == nullptr ? mBeginPressing() : mEventBeginCallback();
}

inline void CAppEventProcessor::Pressing()
{
    mEventCallback == nullptr ? mPressing() : mEventCallback();
}

inline void CAppEventProcessor::EndPressing()
{
    mEventEndCallback == nullptr ? mEndPressing() : mEventEndCallback();
}

} // namespace drash

#endif // DRASH_APPEVENTPROCESSOR_H
