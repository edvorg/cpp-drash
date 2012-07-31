#ifndef COBJECTCAMERA_H
#define COBJECTCAMERA_H

#include "csceneobject.h"

namespace drash
{

class CCameraParams : public CSceneObjectParams
{
public:
    CCameraParams();
};

class CObjectCamera : public CSceneObject
{
public:
    typedef CCameraParams ParamsT;

    CObjectCamera( void );
    virtual ~CObjectCamera( void );

    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);

    virtual void Step(double _dt);

    void SetPosTarget( const CVec2 &_target );
    void SetPosTargetSpeed( const CVec2 &_speed );
    const CVec2 &GetPosTarget() const;
    const CVec2 &GetPosTargetSpeed() const;
    void RemovePosTarget();

    void SetZoom( float _zoom );
    float GetZoom() const;

    void SetZoomTarget( float _target );
    void SetZoomTargetSpeed( float _speed );
    float GetZoomTarget(void);
    float GetZoomTargetSpeed(void);
    void RemoveZoomTarget(void);

private:
    CVec2 mTarget;
    CVec2 mTargetSpeed;
    bool mTargetSet;

    float mZoom;
    float mZoomTarget;
    float mZoomTargetSpeed;
    bool mZoomTargetSet;
};

}// namespace drash
#endif // COBJECTCAMERA_H
