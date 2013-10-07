#include "root.h"
#include "screen.h"

namespace drash
{

CRoot::CRoot()
{
    mGrengSystems.Init();
}

CRoot::~CRoot()
{
    mScreen = CScreenPtr(nullptr);
    mGrengSystems.Release();
}

void CRoot::Step(double _dt)
{
    if (mScreen)
    {
        mScreen->Step(_dt);
    }
}

void CRoot::Render()
{
    if (mScreen)
    {
        mScreen->Render();
    }
}

} // namespace drash
