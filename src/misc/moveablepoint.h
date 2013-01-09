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


    void Colculate();

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
