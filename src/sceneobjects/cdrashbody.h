#ifndef CDRASHBODY_H
#define CDRASHBODY_H

#include "csolidbody.h"
#include <vector>

namespace drash
{

class CDrashBodyParams : public CSolidBodyParams
{
public:
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

    bool mDestroy;
    CVec2 mLastSpeed;
};

} // namespace drash

#endif // CDRASHBODY_H
