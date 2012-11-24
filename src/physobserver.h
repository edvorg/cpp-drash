#ifndef PHYSOBSERVER_H
#define PHYSOBSERVER_H

#include <Box2D/Box2D.h>

namespace drash
{

class CPhysObserver : public b2ContactListener, public b2ContactFilter
{
public:
    CPhysObserver();

    virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) override;
    virtual void BeginContact( b2Contact * _contact ) override;
    virtual void PreSolve( b2Contact* _contact, const b2Manifold* _oldManifold ) override;
    virtual void EndContact( b2Contact * _contact ) override;
};

}

#endif // PHYSOBSERVER_H
