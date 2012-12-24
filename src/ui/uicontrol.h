#ifndef DRASH_UICONTROL_H
#define DRASH_UICONTROL_H

#include <functional>
#include "../misc/vec2.h"

namespace drash
{

namespace ui
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
    inline void SetReleaseHandler(const std::function<void ()> &_handler);
    inline void SetStepHandler(const std::function<void (double _dt)> &_handler);
    inline void SetDrawHandler(const std::function<void ()> &_handler);

    inline void SetPos(const CVec2i &_pos);
    inline void SetSize(const CVec2ui &_size);
    inline const CVec2i &GetPos() const;
    inline const CVec2ui &GetSize() const;

protected:
private:
    std::function<void ()> mDestroyHandler = [] () {};
    std::function<void ()> mPressHandler = [] () {};
    std::function<void ()> mReleaseHandler = [] () {};
    std::function<void (double)> mStepHandler = [] (double) {};
    std::function<void ()> mDrawHandler = [] () {};

    CVec2i mPos;
    CVec2ui mSize;
};

inline void CUIControl::SetDestroyHandler(const std::function<void ()> &_handler)
{
    mDestroyHandler = _handler;
}

inline void CUIControl::SetPressHandler(const std::function<void ()> &_handler)
{
    mPressHandler = _handler;
}

inline void CUIControl::SetReleaseHandler(const std::function<void ()> &_handler)
{
    mReleaseHandler = _handler;
}

inline void CUIControl::SetStepHandler(const std::function<void (double _dt)> &_handler)
{
    mStepHandler = _handler;
}

inline void CUIControl::SetDrawHandler(const std::function<void ()> &_handler)
{
    mDrawHandler = _handler;
}

inline void CUIControl::SetPos(const CVec2i &_pos)
{
    mPos = _pos;
}

inline void CUIControl::SetSize(const CVec2ui &_size)
{
    mSize = _size;
}

inline const CVec2i &CUIControl::GetPos() const
{
    return mPos;
}

inline const CVec2ui &CUIControl::GetSize() const
{
    return mSize;
}

} // namepsace ui

} // namespace drash

#endif // DRASH_UICONTROL_H
