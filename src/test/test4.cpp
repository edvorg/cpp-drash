// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2014 Edward Knyshov, Yuriy Shatilin.

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

namespace drash {

    namespace test {

        using namespace greng;

        Test4::Test4(greng::GrengSystemsSet& greng) : App(greng) {
            testButton1.Connect(&GetUISystem());

            testButton1.SetClickHandler([]() { LOG_INFO("click 1"); });

            testButton1.SetPos(Vec2i(100, 100));
            testButton1.SetSize(Vec2ui(200, 30));

            testButton2.Connect(&GetUISystem());

            testButton2.SetClickHandler([]() { LOG_INFO("click 2"); });

            testButton2.SetPos(Vec2i(350, 100));
            testButton2.SetSize(Vec2ui(30, 30));

            testSlider1.Connect(&GetUISystem());

            testSlider1.SetPos(Vec2i(430, 100));
            testSlider1.SetSize(Vec2ui(200, 30));

            testSlider1.SetMin(-100);
            testSlider1.SetMax(200);
            testSlider1.SetValue(5);
            testSlider1.SetSliderWidth(100);

            testSlider1.SetValueHandler([](float _value) { LOG_INFO(_value); });

            LOG_INFO("value is " << valueAnimator1);
            value = 33;
            LOG_INFO("value is " << valueAnimator1);

            valueAnimator1 = 50;
            valueAnimator2.SetTarget(100, 2.0, AnimatorBehavior::Bounce);

            testPoint.Set(0.0f, 0.0f, 0.0f);
            CameraParams params;
            params.pos.Set(10, 10, 10.0f);
            params.rotation.Set(-M_PI / 4, M_PI / 4, 0);
            Camera* camera =
                GetGrengSystems().GetCameraManager().CreateCamera(params);

            point.SetCamera(camera);
            point.SetCenter(testPoint);

            GetEventSystem().SetProcessor(
                "LB", AppEventProcessor([this]() { point.ClickBegin(); },
                                         [this]() { point.ClickPressing(); },
                                         [this]() { point.ClickEnd(); }));

            GetDebugRenderer().SetCamera(camera);
        }

        void Test4::Step(double _dt) {
            App::Step(_dt);
            point.SetCursorPos(GetCursorPos());
            point.Step(_dt);
        }

        void Test4::Render() {
            App::Render();
            point.Render(GetGrengSystems().GetRenderer());
        }

    } // namespace test

} // namespace drash
