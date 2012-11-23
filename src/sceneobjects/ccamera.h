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

protected:        
    CCamera( void );

    bool Init( const GeometryT &_geometry, const ParamsT &_params );

private:
};

}// namespace drash

#endif // COBJECTCAMERA_H
