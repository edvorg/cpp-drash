#ifndef CCONTACTLISTENER_H
#define CCONTACTLISTENER_H

#include <Box2D/Box2D.h>

namespace drash{

class CContactListener : public b2ContactListener
{
public:
    CContactListener();

    virtual void BeginContact(b2Contact* contact);

    virtual void EndContact(b2Contact* contact);

    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

}// namespace drash
#endif // CCONTACTLISTENER_H
