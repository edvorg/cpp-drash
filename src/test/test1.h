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

#pragma once
#ifndef TESTEDITORAPP_H
#define TESTEDITORAPP_H

#include "../app/app.h"
#include "../greng/pointlight.h"
#include "../misc/ray.h"
#include "../misc/plane.h"

namespace drash {

    namespace test {

        class CTest1 : public CApp {
        public:
            CTest1(greng::CGrengSystemsSet& greng);
            ~CTest1();

        protected:
            virtual void Step(double _dt) override;
            virtual void Render() override;

            auto& GetCamera() const { return *camera; }

        private:
            void SetProcessors();
            void BeginSplit();
            void DetectNewSplitPoint(const CVec2f& _p1, const CVec2f& _p2,
                                     unsigned int _index, const CRay& _r);
            void ComputeIntersections();
            void EndSplit();
            void ExtrudeFigure(float _delta_depth);
            void CamViewProcessors();
            void CompleteFigure();
            void CreateGeometry();
            void DetachCurrentObject();
            void SelectFigure();

            std::vector<CVec2f> vertices;
            unsigned int templateCounter = 0;
            CSceneObjectGeometry* currentTemplate = nullptr;
            CSceneObject* currentObject = nullptr;
            CFigure* currentFigure = nullptr;

            CVec2f camRotFirstClick;

            CVec3f center;
            CVec3f x;
            CVec3f y;
            CVec3f z;

            CVec3f axisDrawK;
            unsigned int axisOver = 0;
            CVec3f figureMoveFirstClick;
            unsigned int axisMoving = 0;

            CVec3f splitFigureMin;
            CVec3f splitFigureMax;
            bool splitMode = false;
            CPlane splitPlane;
            CVec3f splitPlanePoint1;
            CVec3f splitPlanePoint2;
            CVec3f splitPlanePoint3;
            CVec3f splitPlanePoint4;
            CVec3f splitIntersection1;
            unsigned splitIntersection1Index = 0;
            CVec3f splitIntersection2;
            unsigned splitIntersection2Index = 0;
            unsigned int splitIntersectionsCount = 0;
            bool splitDepth = false;
            float splitFigureCenterZ = 0;

            greng::CCamera* camera = nullptr;
            greng::CPointLight light1;
        };

    } // namespace test

} // namespace drash

#endif // TESTEDITORAPP_H
