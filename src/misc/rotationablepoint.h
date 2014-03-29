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

#ifndef DRASH_ROTATIONABLEPOINT_H
#define DRASH_ROTATIONABLEPOINT_H

namespace greng
{

class CRenderer;
class CCamera;

}

namespace drash
{

class CRotationablePoint
{
public:
    CRotationablePoint() = default;

    inline void SetRenderer(greng::CRenderer *_renderer);
    inline void SetCamera(greng::CCamera *_camera);

    bool Init();
    void Step(double);
    void Render();
    void Release();

    inline void SetPoint(const CVec3f &_point);
    inline void SetRotation(const CVec3f &_rotation);

    inline void SetCursorPos(const CVec2f &_pos);
    void RotateBegin();
    void RotateEnd();

    float radius = 2;

    inline void SetAxisOX(const bool _val);
    inline void SetAxisOY(const bool _val);
    inline void SetAxisOZ(const bool _val);

    inline const CVec3f& GetRotation() const;

private:
    CVec3f mRotationDelta;
    CVec2f mCursorPos = CVec2f(0);

    greng::CRenderer *mRenderer = nullptr;
    greng::CCamera *mCamera = nullptr;

    CVec3f mPoint = CVec3f(0);
    CVec3f mRotation = CVec3f(0);

    unsigned int mAxisOvered = 0;
    unsigned int mAxisRotating = 0;


    bool mAxisOX = true;
    bool mAxisOY = true;
    bool mAxisOZ = true;
};

inline void CRotationablePoint::SetRenderer(greng::CRenderer *_renderer)
{
    mRenderer = _renderer;
}

inline void CRotationablePoint::SetCamera(greng::CCamera *_camera)
{
    mCamera = _camera;
}

inline void CRotationablePoint::SetPoint(const CVec3f &_point)
{
    mPoint = _point;
}

inline void CRotationablePoint::SetRotation(const CVec3f &_rotation)
{
    mRotation = _rotation;
}

inline void CRotationablePoint::SetCursorPos(const CVec2f &_pos)
{
    mCursorPos = _pos;
}

inline void CRotationablePoint::SetAxisOX(const bool _val)
{
    mAxisOX = _val;
}

inline void CRotationablePoint::SetAxisOY(const bool _val)
{
    mAxisOY = _val;
}

inline void CRotationablePoint::SetAxisOZ(const bool _val)
{
    mAxisOZ = _val;
}

inline const CVec3f& CRotationablePoint::GetRotation() const{
    return mRotation;
}

} // namespace drash

#endif // DRASH_ROTATIONABLEPOINT_H
