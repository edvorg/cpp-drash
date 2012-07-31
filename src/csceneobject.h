#ifndef CSCENEOBJECT_H
#define CSCENEOBJECT_H

#include <Box2D/Box2D.h>
#include "cvec2.h"

namespace drash
{

class CSceneObjectParams
{
public:
    bool mDynamic;
    CVec2 mPos;
    float mAngle;
    bool mFixedRotation;
    CSceneObjectParams();
};

class CSceneObject
{
public:
    friend class CScene;

    typedef CSceneObjectParams ParamsT;

    CSceneObject(void);
    virtual ~CSceneObject(void);

    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);

    virtual void Step( double _dt );

    void SetPos( const CVec2 &_pos );
    const CVec2 GetPos() const;

protected:
    b2Body* mBody;

private:
};

} // namespace drash

#endif // CSCENEOBJECT_H
