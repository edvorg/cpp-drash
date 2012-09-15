#pragma once
#ifndef CDRASHBODY_H
#define CDRASHBODY_H

#include "csolidbody.h"
#include <vector>
#include "../diag/ctimer.h"

namespace drash
{

class CDrashBodyParams : public CSolidBodyParams
{
public:
    CDrashBodyParams();

    std::vector<CDrashBodyParams> mChilds;
    CVec2 mLocalPos;
    double mDestroyDelay;
};

class CDrashBody : public CSolidBody
{
public:
    friend class CScene;

    typedef CDrashBodyParams ParamsT;

    virtual void BeginContact( const CContact &_contact );
    virtual void EndContact( const CContact &_contact );

protected:
    CDrashBody();
    virtual ~CDrashBody();

    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);

    virtual void Step( double _dt );

    int mCounter;
    CVec2 mLastVelocity;
    float mLastAngularVelocity;
    ParamsT mParams;
    CTimer mTimer;
};

} // namespace drash

#endif // CDRASHBODY_H
