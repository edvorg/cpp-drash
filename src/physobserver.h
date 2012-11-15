#ifndef PHYSOBSERVER_H
#define PHYSOBSERVER_H

#include <Box2D/Box2D.h>

namespace drash
{

class CPhysObserver : public b2ContactListener, public b2ContactFilter
{
public:
    CPhysObserver();

    virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
    virtual void BeginContact( b2Contact * _contact );
    virtual void PreSolve( b2Contact* _contact, const b2Manifold* _oldManifold );
    virtual void EndContact( b2Contact * _contact );
};

}

#endif // PHYSOBSERVER_H
