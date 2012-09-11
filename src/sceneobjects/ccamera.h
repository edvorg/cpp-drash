#ifndef COBJECTCAMERA_H
#define COBJECTCAMERA_H

#include "csceneobject.h"
#include "../canimatedparam.h"
#include "../canimatedparam.h"

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

    static const double m_ZoomMax;

    CCamera( void );
    virtual ~CCamera( void );

    double GetZoom() const { return mZoom.Get(); }
    double GetZoomTarget() const { return mZoom.GetTarget(); }
    void SetZoomTarget( double _target, double _time ) { mZoom.SetTarget( std::min( std::max( 1.0, _target ), m_ZoomMax ), _time, AnimationBehaviorSingle ); }

protected:    
    CAnimatedParam<double> mZoom;

    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);

    virtual void Step(double _dt);

private:
};

}// namespace drash

#endif // COBJECTCAMERA_H
