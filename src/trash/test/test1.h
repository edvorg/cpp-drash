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
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with drash Source Code. If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#pragma once
#ifndef TESTEDITORAPP_H
#define TESTEDITORAPP_H

#include "../app/app.h"
#include "../greng/pointlight.h"
#include "../misc/ray.h"
#include "../misc/plane.h"

namespace drash
{

namespace test
{

class CTest1 : public CApp
{
public:
    CTest1() = default;

protected:
    virtual bool Init() override;
    virtual void Step(double _dt) override;
    virtual void Render() override;
    virtual void Release() override;

    inline greng::CCamera *GetCamera() const;

private:
    void SetProcessors();
    void BeginSplit();
    void DetectNewSplitPoint(const CVec2f &_p1, const CVec2f &_p2, unsigned int _index, const CRay &_r);
    void ComputeIntersections();
    void EndSplit();
    void ExtrudeFigure(float _delta_depth);
    void CamViewProcessors();
    void CompleteFigure();
    void CreateGeometry();
    void DetachCurrentObject();
    void SelectFigure();

    std::vector<CVec2f> mVertices;
    unsigned int mTemplateCounter = 0;
    CSceneObjectGeometry *mCurrentTemplate = nullptr;
    CSceneObject *mCurrentObject = nullptr;
    CFigure *mCurrentFigure = nullptr;

    CVec2f mCamRotFirstClick;

    CVec3f mCenter;
    CVec3f mX;
    CVec3f mY;
    CVec3f mZ;

    CVec3f mAxisDrawK;
    unsigned int mAxisOver = 0;
    CVec3f mFigureMoveFirstClick;
    unsigned int mAxisMoving = 0;

    CVec3f mSplitFigureMin;
    CVec3f mSplitFigureMax;
    bool mSplitMode = false;
    CPlane mSplitPlane;
    CVec3f mSplitPlanePoint1;
    CVec3f mSplitPlanePoint2;
    CVec3f mSplitPlanePoint3;
    CVec3f mSplitPlanePoint4;
    CVec3f mSplitIntersection1;
    unsigned mSplitIntersection1Index = 0;
    CVec3f mSplitIntersection2;
    unsigned mSplitIntersection2Index = 0;
    unsigned int mSplitIntersectionsCount = 0;
    bool mSplitDepth = false;
    float mSplitFigureCenterZ = 0;

    greng::CCamera *mCamera = nullptr;
    greng::CPointLight mLight1;
};

inline greng::CCamera *CTest1::GetCamera() const
{
    return mCamera;
}

} // namespace test

} // namespace drash

#endif // TESTEDITORAPP_H
