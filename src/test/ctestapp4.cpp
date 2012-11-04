#include "ctestapp4.h"

namespace drash
{

CTestApp4::CTestApp4()
{
}

bool CTestApp4::Init()
{
    if (CTestApp::Init() == false)
    {
        return false;
    }

    return true;
}

}// namespace drash
