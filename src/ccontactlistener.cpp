#include "ccontactlistener.h"
#include "csceneobject.h"
#include "clogger.h"

namespace drash
{

CContactListener::CContactListener()
{
}

void CContactListener::BeginContact(b2Contact * _contact)
{
    CSceneObject *obj1 = reinterpret_cast<CSceneObject*>(
                _contact->GetFixtureA()->GetBody()->GetUserData() );

    CSceneObject *obj2 = reinterpret_cast<CSceneObject*>(
                _contact->GetFixtureB()->GetBody()->GetUserData() );

    if ( obj1 == NULL || obj2 == NULL )
    {
        LOG_WARN("CContactListener::BeginContact(): can not get pointer for one of objects. Skipping");
        return;
    }

    if ( obj1 == obj2 )
    {
        LOG_WARN("CContactListener::BeginContact(): object A is equals to object B. Skipping");
        return;
    }

    obj1->BeginContact(obj2);
    obj2->BeginContact(obj1);
}

void CContactListener::PreSolve(b2Contact *_contact, const b2Manifold *_oldManifold)
{
}

void CContactListener::PostSolve(b2Contact * _contact, const b2ContactImpulse * _impulse)
{
}

void CContactListener::EndContact(b2Contact *_contact)
{
    CSceneObject *obj1 = reinterpret_cast<CSceneObject*>(
                _contact->GetFixtureA()->GetBody()->GetUserData() );

    CSceneObject *obj2 = reinterpret_cast<CSceneObject*>(
                _contact->GetFixtureB()->GetBody()->GetUserData() );

    if ( obj1 == NULL || obj2 == NULL )
    {
        LOG_WARN("CContactListener::BeginContact(): can not get pointer for one of objects. Skipping");
        return;
    }

    if ( obj1 == obj2 )
    {
        LOG_WARN("CContactListener::BeginContact(): object A is equals to object B. Skipping");
        return;
    }

    obj1->EndContact(obj2);
    obj2->EndContact(obj1);
}

}// namespace drash
