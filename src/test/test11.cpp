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

#include "test11.hpp"

#include "../greng/camera.h"
#include "../scene/sceneobject.h"
#include "../scene/figure.h"

namespace drash {

    namespace test {

        CTest11::CTest11() {
            camera = GetGrengSystems().GetCameraManager().CreateCamera({});
            camera->GetPos() = {-10,10,10};            
            camera->GetPos().SetTarget({10,-10,10}, 10, AnimatorBehavior::Bounce);
            camera->SetOrtho(true);
            camera->GetOrthoWidth().Set(10);
            camera->GetOrthoWidth().SetTarget(2, 5, AnimatorBehavior::Bounce);
            
            GetDebugRenderer().SetCamera(camera);
            GetDebugRenderer().SetLight(&light);
            GetDebugRenderer().SetTexCoordsScale(0.3);

            CFigureParams f;
            f.vertices.push_back({-1, 0});
            f.vertices.push_back({0, 1});
            f.vertices.push_back({1, 0});
            f.depth = 1;
            
            CSceneObjectGeometry g;
            g.figures.push_back(f);
            
            CSceneObjectParams p;
            p.dynamic = false;
            
            GetScene().CreateObject(g, p);

            GetEventSystem().SetProcessor("SPC", CAppEventProcessor([this] {
                        camera->SetOrtho(!camera->IsOrtho());                    
                    }));
        }

        void CTest11::Step(double _dt) {
            CApp::Step(_dt);
            camera->LookAt({0, 0, 0});
            light.position = camera->GetPos();
        }

        void CTest11::Render() {
            CApp::Render();
        }

    } // namespace test

} // namespace drash










