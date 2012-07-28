#ifndef CSCENEOBJECT_H
#define CSCENEOBJECT_H

#include <Box2D/Box2D.h>
#include "cvec2.h"

namespace drash
{

class CSceneObject
{
public:
    friend class CScene;

    CSceneObject(void);
    virtual ~CSceneObject(void);

    virtual void Release(void) = 0;

protected:
    b2Body* mBody;

private:
};

} // namespace drash

#endif // CSCENEOBJECT_H
