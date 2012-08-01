#ifndef CCONTACTLISTENER_H
#define CCONTACTLISTENER_H

#include <Box2D/Box2D.h>

namespace drash{

class CContactListener : public b2ContactListener
{
public:
    CContactListener();

    virtual void BeginContact( b2Contact * _contact );

    virtual void PreSolve( b2Contact* _contact, const b2Manifold * _oldManifold );

    virtual void PostSolve( b2Contact * _contact, const b2ContactImpulse * _impulse);

    virtual void EndContact( b2Contact * _contact );
};

}// namespace drash
#endif // CCONTACTLISTENER_H
