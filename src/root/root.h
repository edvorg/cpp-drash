#ifndef ROOT_H
#define ROOT_H

#include "../greng/grengsystemsset.h"
#include <memory>

namespace drash
{

class CScreen;

/// root system
/// TODO: add audio and prefs systems
class CRoot
{
public:
    using CScreenPtr = std::shared_ptr<CScreen>;

    CRoot();
    virtual ~CRoot();

    void Step(double _dt);
    void Render();

    greng::CGrengSystemsSet &GetGrengSystems();
    void SetScreen(const CScreenPtr &_ptr);
    CScreenPtr GetScreen();

private:
    greng::CGrengSystemsSet mGrengSystems;
    CScreenPtr mScreen = nullptr;
};

inline greng::CGrengSystemsSet &CRoot::GetGrengSystems()
{
    return mGrengSystems;
}

inline void CRoot::SetScreen(const CScreenPtr &_ptr)
{
    mScreen = _ptr;
}

inline CRoot::CScreenPtr CRoot::GetScreen()
{
    return mScreen;
}

} // namespace drash
#endif // ROOT_H
