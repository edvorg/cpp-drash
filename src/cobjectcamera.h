#ifndef COBJECTCAMERA_H
#define COBJECTCAMERA_H

#include "csceneobject.h"

namespace drash
{

class CCameraParams : public CSceneObjectParams{
public:
    CCameraParams();
};

class CObjectCamera : public CSceneObject
{
public:
    typedef CCameraParams ParamsT;

    CObjectCamera( void );
    virtual ~CObjectCamera( void );

    virtual bool Init(const ParamsT &_params);
    virtual void Release(void);
};

}// namespace drash
#endif // COBJECTCAMERA_H
