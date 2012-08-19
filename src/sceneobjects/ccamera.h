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

    CCamera( void );
    virtual ~CCamera( void );

    CAnimatedParam<double> mZoom;

protected:
    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);

    virtual void Step(double _dt);

private:
};

}// namespace drash

#endif // COBJECTCAMERA_H
