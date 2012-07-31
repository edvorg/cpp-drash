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

    const CVec2 GetPos() const;

    void SetTarget( const CVec2 &_target );
    void SetTargetSpeed( const CVec2 &_speed );
    const CVec2 &GetTarget() const;
    const CVec2 &GetTargetSpeed() const;
    void RemoveTarget();

    void SetZoom( float _zoom );
    float GetZoom() const;

private:
    CVec2 mTarget;
    CVec2 mTargetSpeed;
    bool mTargetSet;
    float mZoom;
};

}// namespace drash
#endif // COBJECTCAMERA_H
