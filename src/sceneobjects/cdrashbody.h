#ifndef CDRASHBODY_H
#define CDRASHBODY_H

#include "csolidbody.h"
#include <vector>

namespace drash
{

class CDrashBodyStrip
{
public:
    std::vector<unsigned int> mIndices;
};

class CDrashBodyParams : public CSolidBodyParams
{
public:
    std::vector<CDrashBodyStrip> mStrips;
    std::vector<CVec2> mDrashVertices;
};

class CDrashBody : public CSolidBody
{
public:
    friend class CScene;

    typedef CDrashBodyParams ParamsT;

    virtual void BeginContact( CSceneObject* _object, const b2Manifold *_manifold );
    virtual void EndContact( CSceneObject* _object, const b2Manifold *_manifold );

protected:
    CDrashBody();
    virtual ~CDrashBody();

    virtual bool Init( const ParamsT &_params );
    virtual void Release(void);

    std::vector<CDrashBodyStrip> mStrips;
    std::vector<CVec2> mDrashVertices;
};

} // namespace drash

#endif // CDRASHBODY_H
