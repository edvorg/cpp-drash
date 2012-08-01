#include "ccontactlistener.h"
namespace drash{

CContactListener::CContactListener()
{
}

void CContactListener::BeginContact(b2Contact *contact)
{
}

void CContactListener::EndContact(b2Contact *contact)
{
}

void CContactListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
{
}

void CContactListener::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse)
{
}

}// namespace drash
