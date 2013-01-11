#ifndef DRASH_ROTATIONABLEPOINT_H
#define DRASH_ROTATIONABLEPOINT_H

#include "../misc/vec3.h"

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

private:
    CVec2f mCursorPos = CVec2f(0);

    greng::CRenderer *mRenderer = nullptr;
    greng::CCamera *mCamera = nullptr;

    CVec3f mPoint = CVec3f(0);
    CVec3f mRotation = CVec3f(0);

    unsigned int mAxisOvered = 0;
    unsigned int mAxisRotating = 0;
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

} // namespace drash

#endif // DRASH_ROTATIONABLEPOINT_H
