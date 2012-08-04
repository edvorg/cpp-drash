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

class CCamera : public CSceneObject
{
public:
    friend class CScene;

    typedef CCameraParams ParamsT;

    CCamera( void );
    virtual ~CCamera( void );

    void SetZoom( float _zoom );
    float GetZoom() const;

    void SetZoomMax( float _max );
    float GetZoomMax();

    void SetZoomTarget( float _target );
    void SetZoomTargetSpeed( float _speed );
    float GetZoomTarget(void);
    float GetZoomTargetSpeed(void);
    void RemoveZoomTarget(void);

protected:
    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);

    virtual void Step(double _dt);

private:
    float mZoom;
    float mZoomMax;
    float mZoomTarget;
    float mZoomTargetSpeed;
    bool mZoomTargetSet;
};

}// namespace drash

#endif // COBJECTCAMERA_H
