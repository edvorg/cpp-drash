#ifndef DRASH_UISYSTEM_H
#define DRASH_UISYSTEM_H

namespace drash
{

class CDebugDrawSystem;
class CVec2;
class CUIControl;

class CUISystem final
{
public:
    constexpr static const unsigned int mControlsCountLimit = 10;

    CUISystem() = default;
    CUISystem(const CUISystem &) = delete;
    CUISystem(CUISystem &&) = delete;
    CUISystem &operator =(const CUISystem &) = delete;
    CUISystem &operator =(CUISystem &&) = delete;

    bool Init();
    void Release();

    CUIControl *CreateControl();
    void DestroyControl(CUIControl *_control);

    void SetAspectRatio(float _ratio);
    void SetWidth(unsigned int _width);

    bool ScreenSpaceToUISpace(const CVec2 &_from, unsigned int &_x, unsigned int &_y);
    bool UISpaceToScreenSpace(unsigned int _x, unsigned int _y, CVec2 &_v);

    void SetCursorPos(unsigned int _x, unsigned int _y);
    inline unsigned int GetCursorPosX() const;
    inline unsigned int GetCursorPosY() const;

    void BeginEvent();
    void EndEvent();

    void SetDebugDrawSystem(CDebugDrawSystem *_system);
    inline CDebugDrawSystem *GetDebugDrawSystem() const;

    void DebugDraw() const;

protected:
private:
    CUIControl *mControls[mControlsCountLimit];
    unsigned int mControlsCount = 0;

    float mAspectRatio = 1;
    unsigned int mWidth = 1024;
    unsigned int mHeight = 768;

    unsigned int mCursorX = 0;
    unsigned int mCursorY = 0;

    CUIControl *mPressedControl = nullptr;

    CDebugDrawSystem *mDebugDrawSystem = nullptr;
};

inline unsigned int CUISystem::GetCursorPosX() const
{
    return mCursorX;
}

inline unsigned int CUISystem::GetCursorPosY() const
{
    return mCursorY;
}

inline CDebugDrawSystem *CUISystem::GetDebugDrawSystem() const
{
    return mDebugDrawSystem;
}

} // namespace drash

#endif // DRASH_UISYSTEM_H
