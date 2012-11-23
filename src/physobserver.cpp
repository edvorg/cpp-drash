#include "physobserver.h"

#include "misc/math.h"
#include "sceneobjects.h"

using drash::CPhysObserver;

CPhysObserver::CPhysObserver()
{
}

bool CPhysObserver::ShouldCollide( b2Fixture *fixtureA, b2Fixture *fixtureB )
{
    if (fixtureA->GetUserData() == nullptr ||
		fixtureB->GetUserData() == nullptr ||
		fixtureA->GetBody()->GetUserData() == nullptr ||
		fixtureB->GetBody()->GetUserData() == nullptr)
    {
        return false;
    }

    CSceneObject *o1 = reinterpret_cast<CSceneObject*>(fixtureA->GetBody()->GetUserData());
    CSceneObject *o2 = reinterpret_cast<CSceneObject*>(fixtureB->GetBody()->GetUserData());

    CFigure *f1 = reinterpret_cast<CFigure*>( fixtureA->GetUserData() );
    CFigure *f2 = reinterpret_cast<CFigure*>( fixtureB->GetUserData() );

    float z1 = f1->GetZ() + o1->GetZ().Get();
    float z2 = f1->GetZ() + o2->GetZ().Get();

    return fabs(z1 - z2) < (f1->GetDepth() * 0.5 + f2->GetDepth() * 0.5);
}

void CPhysObserver::BeginContact( b2Contact * _contact )
{
    CSceneObject *obj1 = reinterpret_cast<CSceneObject*>(
                _contact->GetFixtureA()->GetBody()->GetUserData() );

    CSceneObject *obj2 = reinterpret_cast<CSceneObject*>(
                _contact->GetFixtureB()->GetBody()->GetUserData() );

    if ( obj1 == NULL || obj2 == NULL )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "can not get pointer for one of objects. Skipping" );
        return;
    }

    if ( obj1 == obj2 )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "object A is equals to object B. Skipping" );
        return;
    }

    b2WorldManifold m;
    _contact->GetWorldManifold(&m);

    CContact c;
    c.mPointCount = _contact->GetManifold()->pointCount;

    for ( unsigned int i=0; i<c.mPointCount; i++ )
    {
        c.mPoints[i] = obj1->mBody->GetLocalPoint( m.points[i] );
    }
    c.obj = obj2;
    obj1->OnContactBegin(c);

    for ( unsigned int i=0; i<c.mPointCount; i++ )
    {
        c.mPoints[i] = obj2->mBody->GetLocalPoint( m.points[i] );
    }
    c.obj = obj1;
    obj2->OnContactBegin(c);
}

void CPhysObserver::PreSolve( b2Contact *_contact, const b2Manifold *_oldManifold )
{
    CSceneObject *obj1 = reinterpret_cast<CSceneObject*>(
                _contact->GetFixtureA()->GetBody()->GetUserData() );

    CSceneObject *obj2 = reinterpret_cast<CSceneObject*>(
                _contact->GetFixtureB()->GetBody()->GetUserData() );

    if ( obj1 == NULL || obj2 == NULL )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "can not get pointer for one of objects. Skipping" );
        return;
    }

    if ( obj1 == obj2 )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "object A is equals to object B. Skipping" );
        return;
    }

    b2WorldManifold m;
    _contact->GetWorldManifold(&m);

    CContact c;
    c.mPointCount = _contact->GetManifold()->pointCount;

    for ( unsigned int i=0; i<c.mPointCount; i++ )
    {
        c.mPoints[i] = obj1->mBody->GetLocalPoint( m.points[i] );
    }
    c.obj = obj2;
    obj1->OnContactPreSolve(c);

    for ( unsigned int i=0; i<c.mPointCount; i++ )
    {
        c.mPoints[i] = obj2->mBody->GetLocalPoint( m.points[i] );
    }
    c.obj = obj1;
    obj2->OnContactPreSolve(c);
}

void CPhysObserver::EndContact( b2Contact *_contact )
{
    CSceneObject *obj1 = reinterpret_cast<CSceneObject*>(
                         _contact->GetFixtureA()->GetBody()->GetUserData() );

    CSceneObject *obj2 = reinterpret_cast<CSceneObject*>(
                         _contact->GetFixtureB()->GetBody()->GetUserData() );

    if ( obj1 == NULL || obj2 == NULL )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "one of objects has NULL user data pointer (CSceneObject)" );
        return;
    }

    if ( obj1 == obj2 )
    {
        LOG_WARN( "CContactListener::BeginContact(): "
                  "object A is equals to object B. Skipping" );
        return;
    }

    b2WorldManifold m;
    _contact->GetWorldManifold(&m);

    CContact c;
    c.mPointCount = _contact->GetManifold()->pointCount;

    for ( unsigned int i = 0; i < c.mPointCount; i++ )
    {
        c.mPoints[i] = obj1->mBody->GetLocalPoint( m.points[i] );
    }
    c.obj = obj2;
    obj1->OnContactEnd(c);

    for ( unsigned int i = 0; i < c.mPointCount; i++ )
    {
        c.mPoints[i] = obj2->mBody->GetLocalPoint( m.points[i] );
    }
    c.obj = obj1;
    obj2->OnContactEnd(c);
}
