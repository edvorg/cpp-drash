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

        class Test1 : public App {
        public:
            Test1(greng::Greng& greng);
            ~Test1();

        protected:
            virtual void Step(double _dt) override;
            virtual void Render() override;

            auto& GetCamera() const { return *camera; }

        private:
            void SetProcessors();
            void BeginSplit();
            void DetectNewSplitPoint(const Vec2f& _p1, const Vec2f& _p2,
                                     unsigned int _index, const Ray& _r);
            void ComputeIntersections();
            void EndSplit();
            void ExtrudeFigure(float _delta_depth);
            void CamViewProcessors();
            void CompleteFigure();
            void CreateGeometry();
            void DetachCurrentObject();
            void SelectFigure();

            std::vector<Vec2f> vertices;
            unsigned int templateCounter = 0;
            SceneObjectGeometry* currentTemplate = nullptr;
            SceneObject* currentObject = nullptr;
            Figure* currentFigure = nullptr;

            Vec2f camRotFirstClick;

            Vec3f center;
            Vec3f x;
            Vec3f y;
            Vec3f z;

            Vec3f axisDrawK;
            unsigned int axisOver = 0;
            Vec3f figureMoveFirstClick;
            unsigned int axisMoving = 0;

            Vec3f splitFigureMin;
            Vec3f splitFigureMax;
            bool splitMode = false;
            Plane splitPlane;
            Vec3f splitPlanePoint1;
            Vec3f splitPlanePoint2;
            Vec3f splitPlanePoint3;
            Vec3f splitPlanePoint4;
            Vec3f splitIntersection1;
            unsigned splitIntersection1Index = 0;
            Vec3f splitIntersection2;
            unsigned splitIntersection2Index = 0;
            unsigned int splitIntersectionsCount = 0;
            bool splitDepth = false;
            float splitFigureCenterZ = 0;

            greng::Camera* camera;
            greng::Camera* uiCamera;
            greng::PointLight light1;
        };

    } // namespace test

} // namespace drash

#endif // TESTEDITORAPP_H
