#ifndef SCREEN_H
#define SCREEN_H

#include "../appeventsystem/appeventsystem.h"
#include "../ui/uisystem.h"

/// base class for Screen

namespace drash
{

class CRoot;

class CScreen
{

public:
    CScreen() = delete;
    explicit CScreen(CRoot & _parent);
    virtual ~CScreen();

    virtual void Step(double _dt);
    virtual void Render();

    inline ui::CUISystem &GetUISystem();
    inline CAppEventSystem &GetEventSystem();
    inline CRoot & GetRoot();

protected:
private:
    ui::CUISystem mUISystem;
    CAppEventSystem mEventSystem;
    CRoot & mRoot;
};

inline ui::CUISystem &CScreen::GetUISystem()
{
    return mUISystem;
}

inline CAppEventSystem &CScreen::GetEventSystem()
{
    return mEventSystem;
}

inline CRoot & CScreen::GetRoot()
{
    return mRoot;
}

} // namespace drash

#endif // SCREEN_H
