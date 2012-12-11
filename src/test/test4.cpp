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
        LOG_INFO("click 1");
    });

    mTestButton1.SetPos(100, 100);
    mTestButton1.SetSize(200, 30);

    mTestButton2.Connect(&GetUISystem());

    mTestButton2.SetClickHandler([] ()
    {
        LOG_INFO("click 2");
    });

    mTestButton2.SetPos(350, 100);
    mTestButton2.SetSize(30, 30);

    mTestSlider1.Connect(&GetUISystem());

    mTestSlider1.SetPos(430, 100);
    mTestSlider1.SetSize(200, 30);

    mTestSlider1.SetMin(-100);
    mTestSlider1.SetMax(200);
    mTestSlider1.SetValue(5);
    mTestSlider1.SetSliderWidth(100);

    mTestSlider1.SetValueHandler([] (float _value)
    {
        LOG_INFO(_value);
    });
}

void CTest4::Release()
{
}

} // namespace test
} // namespace drash
