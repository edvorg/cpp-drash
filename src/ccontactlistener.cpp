#include "ccontactlistener.h"
namespace drash{

CContactListener::CContactListener()
{
}

void CContactListener::BeginContact(b2Contact * _contact)
{
}

void CContactListener::EndContact(b2Contact *contact)
{
}

void CContactListener::PreSolve(b2Contact *_contact, const b2Manifold *_oldManifold)
{
}

void CContactListener::PostSolve(b2Contact * _contact, const b2ContactImpulse * _impulse)
{
}

}// namespace drash
