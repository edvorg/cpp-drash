#include "test4.h"

#include "../ui/uicontrol.h"

namespace drash
{

namespace test
{

CTest4::CTest4()
{
    mTestButton1.Connect(&GetUISystem());
    mTestButton1.SetClickHandler([] ()
    {
        LOG_INFO("click");
    });
}

void CTest4::Release()
{
    mTestButton1.Disconnect();
}

} // namespace test
} // namespace drash
