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

#include "test8.h"

#include "../levelmanager/leveldesc.h"
#include "../scene/figure.h"
#include "../greng/camera.h"

namespace drash {

    namespace test {

        CTest8::CTest8() : CApp() {
            InitUI();
            InitLevels();
            InitCamera();
            InitLights();
            GetGeometryManager().Load();
        }

        void CTest8::Render() { CApp::Render(); }

        CTest8::~CTest8() {
            GetGeometryManager().Store();
            mLevel1->Store("level1");
            mLevel2->Store("level2");
        }

        bool CTest8::InitUI() {
            GetUISystem().SetWidth(1024);

            mButton1.Connect(&GetUISystem());
            mButton1.SetPos(CVec2i(5, 30));
            mButton1.SetSize(CVec2ui(100, 20));

            mButton1.SetClickHandler([this]() {
                if (mLevel1 != nullptr) {
                    GetLevelManager().StartLevel(mLevel1);
                }
            });

            mButton2.Connect(&GetUISystem());
            mButton2.SetPos(CVec2i(5, 5));
            mButton2.SetSize(CVec2ui(100, 20));

            mButton2.SetClickHandler([this]() {
                if (mLevel2 != nullptr) {
                    GetLevelManager().StartLevel(mLevel2);
                }
            });

            return true;
        }

        bool CTest8::InitLevels() {
            mLevel1 = GetLevelManager().CreateLevel();
            mLevel2 = GetLevelManager().CreateLevel();

            mLevel1->Load("level1");
            mLevel2->Load("level2");
            return true;
            if (mLevel1 == nullptr || mLevel2 == nullptr) {
                return false;
            }

            CSceneObjectGeometry* g1 =
                GetGeometryManager().CreateGeometry("object1_geometry");
            CSceneObjectGeometry* g2 =
                GetGeometryManager().CreateGeometry("object2_geometry");

            if (g1 == nullptr || g2 == nullptr) {
                return false;
            }

            g1->mFigures.resize(1);
            g1->mFigures[0].mDepth = 1;
            g1->mFigures[0].mVertices.push_back(CVec2f(-1, 0));
            g1->mFigures[0].mVertices.push_back(CVec2f(1, 0));
            g1->mFigures[0].mVertices.push_back(CVec2f(0, 1));

            g2->mFigures.resize(1);
            g2->mFigures[0].mDepth = 1;
            g2->mFigures[0].mVertices.push_back(CVec2f(-1, -1));
            g2->mFigures[0].mVertices.push_back(CVec2f(1, -1));
            g2->mFigures[0].mVertices.push_back(CVec2f(1, 1));
            g2->mFigures[0].mVertices.push_back(CVec2f(-1, 1));

            CLevelObjectDesc* p1 =
                mLevel1->AddObject("object1_geometry", "obj1");
            CLevelObjectDesc* p2 =
                mLevel1->AddObject("object2_geometry", "obj2");

            if (p1 == nullptr || p2 == nullptr) {
                return false;
            }

            p2->mParams.mPos.Set(3, 0, 0);

            CLevelObjectDesc* p3 =
                mLevel2->AddObject("object1_geometry", "obj1");
            CLevelObjectDesc* p4 =
                mLevel2->AddObject("object2_geometry", "obj2");

            if (p3 == nullptr || p4 == nullptr) {
                return false;
            }

            p3->mParams.mPos.Set(3, 0, 0);

            return true;
        }

        bool CTest8::InitCamera() {
            greng::CCameraParams p;
            p.mPos.Set(0, 0, 10);
            p.mFov = M_PI / 6.0;
            auto c = GetGrengSystems().GetCameraManager().CreateCamera(p);
            GetDebugRenderer().SetCamera(c);

            return true;
        }

        bool CTest8::InitLights() {
            mLight1.mPosition.Set(0, 10, 0);
            GetDebugRenderer().SetLight(&mLight1);
            return true;
        }

    } // namespace test

} // namespace drash
