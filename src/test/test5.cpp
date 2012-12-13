#include "test5.h"

namespace drash
{

namespace test
{

bool CTest5::Init()
{
    if (CApp::Init() == false)
    {
        return false;
    }

    return true;
}

} // namespace test
} // namespace drash
