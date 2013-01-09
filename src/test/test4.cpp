// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "test4.h"

#include "../ui/uicontrol.h"

namespace drash
{

namespace test
{

using namespace greng;

bool CTest4::Init()
{
    if (CApp::Init() == false)
    {
        return false;
    }

    mTestButton1.Connect(&GetUISystem());

    mTestButton1.SetClickHandler([] ()
    {
        LOG_INFO("click 1");
    });

    mTestButton1.SetPos(CVec2i(100, 100));
    mTestButton1.SetSize(CVec2ui(200, 30));

    mTestButton2.Connect(&GetUISystem());

    mTestButton2.SetClickHandler([] ()
    {
        LOG_INFO("click 2");
    });

    mTestButton2.SetPos(CVec2i(350, 100));
    mTestButton2.SetSize(CVec2ui(30, 30));

    mTestSlider1.Connect(&GetUISystem());

    mTestSlider1.SetPos(CVec2i(430, 100));
    mTestSlider1.SetSize(CVec2ui(200, 30));

    mTestSlider1.SetMin(-100);
    mTestSlider1.SetMax(200);
    mTestSlider1.SetValue(5);
    mTestSlider1.SetSliderWidth(100);

    mTestSlider1.SetValueHandler([] (float _value)
    {
        LOG_INFO(_value);
    });

    LOG_INFO("value is "<<mValueAnimator1);
    mValue = 33;
    LOG_INFO("value is "<<mValueAnimator1);

    mValueAnimator1 = 50;
    mValueAnimator2.SetTarget(100, 2.0, AnimatorBehavior::Bounce);


    mTestPoint.Set(0.0f,0.0f,0.0f);
    CCameraParams params;
    params.mPos.Set(10,10,10.0f);
    params.mRotation.Set(-M_PI/4, M_PI/4, 0);
    CCamera *camera = GetCameraManager().CreateCamera(params);

    if (camera == nullptr) {
        return false;
    }

    mPoint.SetCamera(camera);
    mPoint.SetCenter(mTestPoint);

    GetEventSystem().SetProcessor("LB", CAppEventProcessor(
    [this] () {
        mPoint.ClickBegin();
    },
    [this] () {
        mPoint.ClickPressing();
    },
    [this] () {
        mPoint.ClickEnd();
    }
    ));

    GetDebugRenderer().SetCamera(camera);
    return true;
}

void CTest4::Step(double _dt)
{
    CApp::Step(_dt);
    mPoint.SetCursorPos(GetCursorPos());
    mPoint.Step(_dt);
}

void CTest4::Render()
{
    CApp::Render();
    mPoint.Render(GetRenderer());
}

} // namespace test

} // namespace drash
