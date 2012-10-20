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

    double GetZoom() const;
    double GetZoomTarget() const;
    void SetZoomTarget( float _target, double _time );

    static const float m_ZoomMax;

protected:        
    CCamera( void );

    bool Init( const ParamsT &_params );

    virtual void Step( double _dt );

    virtual void OnBoom( const CBoomParams &_boom );

private:
    CAnimatedParam<float> mZoom;
    int mShake;
    CVec2 mLastPos;
};

}// namespace drash

#endif // COBJECTCAMERA_H
