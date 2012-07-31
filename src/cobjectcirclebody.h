#ifndef COBJECTCIRCLEBODY_H
#define COBJECTCIRCLEBODY_H

#include "cobjectsolidbody.h"

namespace drash
{

class CCircleBodyParams : public CSceneObjectParams
{
public:
    CCircleBodyParams();

    float mFriction;
    float mRestitution;
    float mMass;
    float mRadius;
};

class CObjectCircleBody : public CObjectSolidBody
{
public:
    friend class CScene;

    typedef CCircleBodyParams ParamsT;

    CObjectCircleBody();
    virtual ~CObjectCircleBody();

protected:
    virtual bool Init( const ParamsT &_params );
    virtual void Release();
};

} // namespace drash

#endif // COBJECTCIRCLEBODY_H
