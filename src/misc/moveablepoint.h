// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2013 Edward Knyshov, Yuriy Shatilin.

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

#ifndef MOVEABLEPOINT_H
#define MOVEABLEPOINT_H

#include "vec3.h"
#include "../greng/camera.h"
#include "../greng/renderer.h"
namespace drash{

class CMoveablePoint
{
public:

    CMoveablePoint();
    CMoveablePoint(CVec3f _point, greng::CCamera *_camera);

    void SetCenter(const CVec3f &_center);
    void SetCamera(greng::CCamera * _camera);
    inline CVec3f GetCenter()const;
    void Step(double);
    void Render(greng::CRenderer &_render);

    void SetCursorPos(const CVec2f &_pos);

    void ClickBegin();
    void ClickPressing();
    void ClickEnd();
private:

    greng::CCamera * mCurrentCamera = nullptr;


    void Calculate();

    greng::CCamera * GetCamera();

    CVec3f mCenter;
    CVec3f mX;
    CVec3f mY;
    CVec3f mZ;

    CVec3f mAxisDrawK;
    unsigned int mAxisOver = 0;
    CVec3f mFirstClick;
    unsigned int mAxisMoving = 0;


    bool mMoving = false;

    CVec2f mCursorPos;

    float mLineSize = 1.0f;
};

inline CVec3f CMoveablePoint::GetCenter() const{
    return mCenter;
}

} // namespace drash
#endif // MOVEABLEPOINT_H
