#ifndef CDRASHBODY_H
#define CDRASHBODY_H

#include "csolidbody.h"

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

    virtual void BeginContact( CSceneObject* _object );
    virtual void EndContact( CSceneObject* _object );

protected:
    CDrashBody();
    virtual ~CDrashBody();

    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);
};

} // namespace drash

#endif // CDRASHBODY_H
