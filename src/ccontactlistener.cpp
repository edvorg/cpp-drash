#include "ccontactlistener.h"
#include "sceneobjects.h"
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

    b2WorldManifold m;
    _contact->GetWorldManifold(&m);

    CContact c;
    c.mPointCount = _contact->GetManifold()->pointCount;

    for ( unsigned int i=0; i<c.mPointCount; i++ )
    {
        c.mPoints[i] = obj1->GetBody()->GetLocalPoint( m.points[i] );
    }
    c.obj = obj2;
    obj1->BeginContact(c);

    for ( unsigned int i=0; i<c.mPointCount; i++ )
    {
        c.mPoints[i] = obj2->GetBody()->GetLocalPoint( m.points[i] );
    }
    c.obj = obj1;
    obj2->BeginContact(c);
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
        return;
    }

    if ( obj1 == obj2 )
    {
        LOG_WARN("CContactListener::BeginContact(): object A is equals to object B. Skipping");
        return;
    }

    b2WorldManifold m;
    _contact->GetWorldManifold(&m);

    CContact c;
    c.mPointCount = _contact->GetManifold()->pointCount;

    for ( unsigned int i=0; i<c.mPointCount; i++ )
    {
        c.mPoints[i] = obj1->GetBody()->GetLocalPoint( m.points[i] );
    }
    c.obj = obj2;
    obj1->EndContact(c);

    for ( unsigned int i=0; i<c.mPointCount; i++ )
    {
        c.mPoints[i] = obj2->GetBody()->GetLocalPoint( m.points[i] );
    }
    c.obj = obj1;
    obj2->EndContact(c);
}

}// namespace drash
