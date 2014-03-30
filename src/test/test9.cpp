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

#include "test9.h"

#include "../greng/camera.h"

namespace drash {

    namespace test {

        CTest9::CTest9(greng::CGrengSystemsSet& greng)
            : CApp(greng),
              camera{ GetGrengSystems().GetCameraManager().CreateCamera({}) },
              point1{ GetGrengSystems().GetRenderer(), *camera } {
            InitCamera();
            InitLights();
            InitRotationablePoint();
        }

        void CTest9::Step(double _dt) {
            CApp::Step(_dt);

            point1.SetCursorPos(GetCursorPos());
            point1.Step(_dt);
        }

        void CTest9::Render() {
            CApp::Render();

            point1.Render();
        }

        bool CTest9::InitCamera() {
            camera->GetPos().Set({ 10, 10, 10 });
            camera->GetFov().Set(M_PI / 6.0);
            camera->LookAt(CVec3f(0));

            if (camera == nullptr) {
                return false;
            }

            GetDebugRenderer().SetCamera(camera);

            return true;
        }

        bool CTest9::InitLights() {
            light1.position.Set(0, 10, 0);
            GetDebugRenderer().SetLight(&light1);

            return true;
        }

        bool CTest9::InitRotationablePoint() {
            point1.SetPoint(CVec3f(0));

            GetEventSystem().SetProcessor(
                "LB",
                CAppEventProcessor([this]() { point1.RotateBegin(); }, []() {},
                                   [this]() { point1.RotateEnd(); }));

            return true;
        }

    } // namespace test

} // namespace drash
