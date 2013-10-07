#ifndef ROOT_H
#define ROOT_H

#include "../greng/grengsystemsset.h"
#include <memory>

namespace drash
{

class CScreen;


// Main manager
// TODO: Contains interface AudioSystem and GamePrefences

class CRoot
{
public:
    CRoot();
    virtual ~CRoot();

    void Step(double _dt);
    void Render();

    using CSreenPtr = std::shared_ptr<CScreen>;

    greng::CGrengSystemsSet &GetGrengSystems();
    void SetScreen(const CSreenPtr &_ptr);

private:
    greng::CGrengSystemsSet mGrengSystems;
    CSreenPtr mScreen = nullptr;
};


inline greng::CGrengSystemsSet &CRoot::GetGrengSystems()
{
    return mGrengSystems;
}

inline void CRoot::SetScreen(const CSreenPtr &_ptr) {
    mScreen = _ptr;
}


} // namespace drash
#endif // ROOT_H
