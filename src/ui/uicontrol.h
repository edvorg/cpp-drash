#ifndef DRASH_UICONTROL_H
#define DRASH_UICONTROL_H

#include <functional>

namespace drash
{

class CUIControl final
{
public:
    friend class CUISystem;

    CUIControl() = default;
    CUIControl(const CUIControl &) = delete;
    CUIControl(CUIControl &&) = delete;
    CUIControl &operator =(const CUIControl &) = delete;
    CUIControl &operator =(CUIControl &&) = delete;

    inline void SetDestroyHandler(const std::function<void ()> &_handler);
    inline void SetPressHandler(const std::function<void ()> &_handler);

protected:
private:
    std::function<void ()> mDestroyHandler = [] () {};
    std::function<void ()> mPressHandler = [] () {};
};

inline void CUIControl::SetDestroyHandler(const std::function<void ()> &_handler)
{
    mDestroyHandler = _handler;
}

inline void CUIControl::SetPressHandler(const std::function<void ()> &_handler)
{
    mPressHandler = _handler;
}

} // namespace drash

#endif // DRASH_UICONTROL_H
