// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

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

namespace drash
{

namespace test
{

class CTest1 : public CApp
{
public:
    virtual ~CTest1() override {}

    virtual bool Init() override;
    virtual void Step(double _dt) override;
    virtual void Render() override;
    virtual void Release() override;

private:
    void SetProcessors();
    void CamViewProcessors();
    void CompleteFigure();
    void CreateTemplate();
    void DetachCurrentObject();
    void SelectFigure();

    double mTime = 0;

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
};

} // namespace test

} // namespace drash

#endif // TESTEDITORAPP_H
