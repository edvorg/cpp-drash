#include "test4.h"

#include "../ui/uicontrol.h"

namespace drash
{

namespace test
{

CTest4::CTest4()
{
    GetUISystem().ConnectWidget(mTestWidget1);
}

void CTest4::Release()
{
    GetUISystem().DisconnectWidget(mTestWidget1);
}

} // namespace test
} // namespace drash
