#ifndef COBJECTRECTANGLE_H
#define COBJECTRECTANGLE_H

#include "csceneobject.h"

namespace drash
{

class CObjectSolidBody : public CSceneObject
{
public:
    CObjectSolidBody(void);
    virtual ~CObjectSolidBody(void);

    virtual bool Init(void);
    virtual void Release(void);
};

} // namespace drash

#endif // COBJECTRECTANGLE_H
