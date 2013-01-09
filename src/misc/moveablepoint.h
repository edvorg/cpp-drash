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

    inline void SetCenter(const CVec3f &_center);

    void Render(greng::CRenderer &_render);

    inline void SetCamera(greng::CCamera * _camera);
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

};


inline void CMoveablePoint::SetCenter(const CVec3f &_center)
{
    mCenter = _center;

}

inline void CMoveablePoint::SetCamera(greng::CCamera *_camera)
{
    mCurrentCamera = _camera;
}

} // namespace drash
#endif // MOVEABLEPOINT_H
