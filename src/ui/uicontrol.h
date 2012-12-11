#ifndef DRASH_UICONTROL_H
#define DRASH_UICONTROL_H

#include <functional>

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

    void SetPos(int _x, int _y);
    void SetSize(unsigned int _w, unsigned int _h);
    inline int GetX() const;
    inline int GetY() const;
    inline unsigned int GetWidth() const;
    inline unsigned int GetHeight() const;

protected:
private:
    std::function<void ()> mDestroyHandler = [] () {};
    std::function<void ()> mPressHandler = [] () {};
    std::function<void ()> mReleaseHandler = [] () {};
    std::function<void (double)> mStepHandler = [] (double) {};
    std::function<void ()> mDrawHandler = [] () {};

    int mX = 0;
    int mY = 0;
    unsigned int mWidth = 100;
    unsigned int mHeight = 20;
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

inline int CUIControl::GetX() const
{
    return mX;
}

inline int CUIControl::GetY() const
{
    return mY;
}

inline unsigned int CUIControl::GetWidth() const
{
    return mWidth;
}

inline unsigned int CUIControl::GetHeight() const
{
    return mHeight;
}

} // namepsace ui

} // namespace drash

#endif // DRASH_UICONTROL_H
