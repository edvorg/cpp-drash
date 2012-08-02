#ifndef COBJECTRECTANGLE_H
#define COBJECTRECTANGLE_H

#include "csceneobject.h"

namespace drash
{

class CSolidBodyParams : public CSceneObjectParams
{
public:
    float mFriction;
    float mRestitution;
    float mMass;

    /// if mVertices is NULL, creates a box with (1, 1) dimentions
    CVec2 *mVertices;

    /// size of mVertices array if present
    unsigned int mVerticesCount;

    CSolidBodyParams();
};

class CSolidBody : public CSceneObject
{
public:
    friend class CScene;

    typedef CSolidBodyParams ParamsT;

protected:
    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);

    CSolidBody(void);
    virtual ~CSolidBody(void);
};

} // namespace drash

#endif // COBJECTRECTANGLE_H