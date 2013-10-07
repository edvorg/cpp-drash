#ifndef SCREEN_H
#define SCREEN_H

#include "../appeventsystem/appeventsystem.h"
#include "../ui/uisystem.h"

namespace drash
{

class CRoot;

class CScreen
{

public:
    CScreen() = delete;
    explicit CScreen(CRoot & _parent);
    virtual ~CScreen();

    virtual void Step(double _dt) = 0;
    virtual void Render() = 0;

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
