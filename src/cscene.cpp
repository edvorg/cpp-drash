#include "cscene.h"
#include "diag/clogger.h"

namespace drash
{

CScene::CScene(void):
    mWorld( b2Vec2( 0, 0 ) ),
    mInitialized(false),
    mCountPlayers(0),
    mCountObjects(0)
{
}

CScene::~CScene(void)
{
    if ( mInitialized == true )
    {
        LOG_WARN( "CScene::~CScene(): warning Release() called from destructor" );
        Release();
    }
}

bool CScene::Init(const CSceneParams &_params)
{
    if ( mInitialized == true )
    {
        LOG_WARN( "CScene::Init(): already initialized" );
        return false;
    }

    mWorld.SetContactListener(this);
    mWorld.SetAllowSleeping(true);
    mWorld.SetContinuousPhysics(false);
    mWorld.SetGravity(_params.mGravity);

    mInitialized = true;
    return true;
}

void CScene::Release(void)
{
    if ( mInitialized == false )
    {
        LOG_WARN( "CScene::Release(): already released" );
        return;
    }

    if (mCountObjects != 0)
    {
        LOG_WARN( "CScene::Release(): "<<
                  mCountObjects<<
                  " object(s) haven't been destroyed. Autorelease" );

        while(mCountObjects != 0)
        {
            DestroyObject< CSceneObject >( mObjects[0] );
        }
    }

    mInitialized = false;
}

void CScene::Step( double _dt )
{
    if ( mInitialized == false )
    {
        LOG_ERR( "CScene::Step(): this is not initialized" );
        return;
    }

    mWorld.Step( _dt, mVelocityIterations, mPositionIterations );

    for (unsigned int i = 0 ; i < mCountObjects ; ){
        if (mObjects[i]->IsDead()){
            DestroyObject<CSceneObject>(mObjects[i]);
        }else{
            mObjects[i]->Step(_dt);
            i++;
        }
    }
    BoomNow();
}

void CScene::BeginContact(b2Contact * _contact)
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

void CScene::PreSolve(b2Contact *_contact, const b2Manifold *_oldManifold)
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
    obj1->PreSolve(c);

    for ( unsigned int i=0; i<c.mPointCount; i++ )
    {
        c.mPoints[i] = obj2->GetBody()->GetLocalPoint( m.points[i] );
    }
    c.obj = obj1;
    obj2->PreSolve(c);
}

void CScene::PostSolve(b2Contact * _contact, const b2ContactImpulse * _impulse)
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
    obj1->PostSolve(c);

    for ( unsigned int i=0; i<c.mPointCount; i++ )
    {
        c.mPoints[i] = obj2->GetBody()->GetLocalPoint( m.points[i] );
    }
    c.obj = obj1;
    obj2->PostSolve(c);
}

void CScene::EndContact(b2Contact *_contact)
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

void CScene::SetDebugRenderer( CDebugRenderer *_renderer )
{
    mWorld.SetDebugDraw(_renderer);
}

void CScene::Draw(void)
{
    if ( mInitialized == false )
    {
        LOG_ERR( "CScene::Step(): this is not initialized" );
        return;
    }

    mWorld.DrawDebugData();
}

void CScene::OnPlayerEvent(const CPlayerEvent &_event, unsigned int _playerId)
{
    if (_playerId >= mCountPlayers){
        LOG_ERR("CScene::OnPlayerEvent() : Player with id = " <<
                _playerId << " no exist");

        return;
    }
    mPlayers[_playerId]->onEvent(_event);
}

int CScene::AddPlayer( const CPlayerParams &_params )
{
    if ( mCountPlayers == mPlayersMaxAmount )
    {
        LOG_ERR("CScene::AddPlayer() : Achieved maximum Amount of Players");
        return -1;
    }

    mPlayers[mCountPlayers] = CreateObject<CPlayer>(_params);
    return mCountPlayers++;
}

void CScene::AddRequestBoom(const CBoomParams _boom)
{
    mListBooms.push_back(_boom);
}

void CScene::BoomNow()
{
    for (unsigned int i = 0 ; i < mCountObjects ; i++){
        for (auto it = mListBooms.begin() ; it != mListBooms.end() ; it++){
            mObjects[i]->Boom(*it);
        }
    }
    mListBooms.clear();
}

} // namespace drash
