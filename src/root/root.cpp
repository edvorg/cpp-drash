#include "root.h"
#include "screen.h"

namespace drash {

CRoot::CRoot()
{

}

CRoot::~CRoot()
{

}

void CRoot::Step(double _dt)
{
    if (mScreen) {
        mScreen->Step(_dt);
    }
}

void CRoot::Render()
{
    if (mScreen) {
        mScreen->Render();
    }
}

} // namespace drash
