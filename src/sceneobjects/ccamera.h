#pragma once
#ifndef COBJECTCAMERA_H
#define COBJECTCAMERA_H

#include "csceneobject.h"
#include "../misc/canimatedparam.h"

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

    static const float m_ZoomMax;

    CCamera( void );
    virtual ~CCamera( void );

    double GetZoom() const;
    double GetZoomTarget() const;
    void SetZoomTarget( float _target, double _time );

protected:    
    CAnimatedParam<float> mZoom;

    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);

    virtual void Step( double _dt );

private:
};

}// namespace drash

#endif // COBJECTCAMERA_H
