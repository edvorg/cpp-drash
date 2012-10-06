#include "csceneobject.h"
#include "../diag/clogger.h"
#include "cboom.h"
#include <GL/gl.h>

namespace drash
{

CSceneObjectParams::CSceneObjectParams():
    mDynamic(true),
    mPos(0),
    mAngle(0),
    mFixedRotation(false)
{
}

CFigureParams::CFigureParams():
    mFriction(1),
    mRestitution(0),
    mMass(1),
    mVertices()
{
}

CSceneObject::CSceneObject(void):
    mBody(NULL),
    mScene(NULL),
    mDead(false)
{
}

CSceneObject::~CSceneObject(void)
{
    Release();
}

bool CSceneObject::Init( const CSceneObject::ParamsT &_params )
{
    if ( mBody == NULL )
    {
        return false;
    }

    mPos.Set(_params.mPos);
    mAngle.Set(_params.mAngle);

    mBody->SetTransform( _params.mPos, _params.mAngle );
    mBody->SetActive(true);
    mBody->SetAwake(true);
    mBody->SetSleepingAllowed(true);
    mBody->SetUserData(this);
    mBody->SetAngularDamping(0);
    mBody->SetBullet(false);
    mBody->SetFixedRotation( _params.mFixedRotation );
    mBody->SetLinearDamping(0);
    mBody->SetGravityScale(1.0f);
    mBody->SetType( _params.mDynamic ?
                    b2_dynamicBody :
                    b2_kinematicBody );

    return true;
}

void CSceneObject::Release()
{
    while ( b2Fixture *f = mBody->GetFixtureList() )
    {
        delete reinterpret_cast<CInterval*>( f->GetUserData() );
        f->SetUserData(NULL);
        mBody->DestroyFixture(f);
    }
}

void CSceneObject::Step( double _dt )
{
    mPos.Step(_dt);
    mAngle.Step(_dt);

    if ( mBody->GetType() == b2_kinematicBody )
    {
        if ( mPos.IsTargetSet() || mAngle.IsTargetSet() )
        {
            mBody->SetTransform( mPos.Get(), mAngle.Get() );
        }
        else
        {
            mPos.Set( mBody->GetWorldCenter() );
            mAngle.Set( mBody->GetAngle() );
        }
    }
    else if ( mBody->GetType() == b2_dynamicBody )
    {
        mPos.Set( mBody->GetWorldCenter() );
        mAngle.Set( mBody->GetAngle() );
    }
}

void CSceneObject::OnContactBegin( const CContact &_contact )
{
}

void CSceneObject::OnContactEnd( const CContact &_contact )
{
}

void CSceneObject::OnBoom( const CBoomParams &_boom )
{
}

static const float g_LayerWidth = 1.0f;

void CSceneObject::DrawSide( const CVec2 &_v1, const CVec2 &_v2, const CInterval &_interval, const b2Color &_diffuse )
{
    CVec2 dp = mBody->GetWorldPoint( _v1 ) -
               mBody->GetWorldPoint( _v2 );
    dp.Normalize();
    CVec2 localx(1, 0);

    float dot = dp.x * localx.x + dp.y * localx.y;
    dot += 1.0;
    dot /= 4.0f;

    glColor3f( _diffuse.r * ( 0.5 + dot ),
               _diffuse.g * ( 0.5 + dot ),
               _diffuse.b * ( 0.5 + dot ) );

    glVertex3f( _v1.x,
                _v1.y,
                _interval.GetMax() * g_LayerWidth + g_LayerWidth / 2.0f );
    glVertex3f( _v1.x,
                _v1.y,
                _interval.GetMin() * g_LayerWidth - g_LayerWidth / 2.0f );
    glVertex3f( _v2.x,
                _v2.y,
                _interval.GetMax() * g_LayerWidth + g_LayerWidth / 2.0f );

    glVertex3f( _v2.x,
                _v2.y,
                _interval.GetMax() * g_LayerWidth + g_LayerWidth / 2.0f );
    glVertex3f( _v1.x,
                _v1.y,
                _interval.GetMin() * g_LayerWidth - g_LayerWidth / 2.0f );
    glVertex3f( _v2.x,
                _v2.y,
                _interval.GetMin() * g_LayerWidth - g_LayerWidth / 2.0f );
}

void CSceneObject::DrawDebug()
{
    glCullFace(GL_NONE);
    unsigned int j = 0;
    for ( b2Fixture *f = mBody->GetFixtureList(); f != NULL; f = f->GetNext() )
    {
        if ( f->GetShape()->GetType() == b2Shape::e_polygon )
        {
            auto s = reinterpret_cast<b2PolygonShape*>( f->GetShape() );

            if (s)
            {
                static const b2Color diffuse(0.1875,0.375,0.75);
                const CInterval interval = f->GetUserData() ?
                                            (CInterval&)(*f->GetUserData()) :
                                            CInterval(-1, 1);

                glEnable(GL_DEPTH_TEST);

                glBegin(GL_TRIANGLE_FAN);
                glColor3f( 0.5 * diffuse.r,
                           0.5 * diffuse.g,
                           0.5 * diffuse.b );
                for ( unsigned int i = 0; i < s->GetVertexCount(); i++ )
                {
                    glVertex3f( s->GetVertex(i).x,
                                s->GetVertex(i).y,
                                interval.GetMax() * g_LayerWidth + g_LayerWidth / 2.0f );
                }
                glEnd();

                glBegin(GL_TRIANGLE_FAN);
                glColor3f( 0.5 * diffuse.r,
                           0.5 * diffuse.g,
                           0.5 * diffuse.b );
                for ( unsigned int i = 0; i < s->GetVertexCount(); i++ )
                {
                    glVertex3f( s->GetVertex(i).x,
                                s->GetVertex(i).y,
                                interval.GetMin() * g_LayerWidth - g_LayerWidth / 2.0f );
                }
                glEnd();

                glBegin(GL_TRIANGLES);
                for ( unsigned int i = 0; i < s->GetVertexCount()-1; i++ )
                {
                    DrawSide( s->GetVertex(i),
                              s->GetVertex(i+1),
                              interval,
                              diffuse );
                }
                DrawSide( s->GetVertex( s->GetVertexCount() - 1),
                          s->GetVertex(0),
                          interval,
                          diffuse );
                glEnd();
            }
        }
        j++;
    }
}

const b2Body *CSceneObject::GetBody() const
{
    return mBody;
}

CScene *CSceneObject::GetScene()
{
    return mScene;
}

const CScene *CSceneObject::GetScene() const
{
    return mScene;
}

void CSceneObject::SetDead()
{
    mDead = true;
}

bool CSceneObject::IsDead() const
{
    return mDead;
}

void CSceneObject::SetDynamic( bool _dynamic )
{
    mBody->SetType( _dynamic ? b2_dynamicBody : b2_kinematicBody );
}

void CSceneObject::CreateFigure( const CFigureParams &_params )
{
    b2PolygonShape s;

    if ( _params.mVertices.size() == 0 )
    {
        s.SetAsBox( 1.0f, 1.0f );
    }
    else
    {
        s.Set( &*_params.mVertices.begin(), _params.mVertices.size() );
    }

    b2MassData md;
    s.ComputeMass( &md, 1.0 );

    b2FixtureDef fdef;
    fdef.density = _params.mMass / md.mass;
    fdef.friction = _params.mFriction;
    fdef.isSensor = false;
    fdef.restitution = _params.mRestitution;
    fdef.shape = &s;
    fdef.userData = NULL;

    b2Fixture *f = mBody->CreateFixture(&fdef);
    f->SetUserData( new CInterval( _params.mLayers ) );
}

void CSceneObject::ApplyLinearImpulse( const CVec2 &_dir, const CVec2 &_pos )
{
    mBody->ApplyLinearImpulse( _dir, _pos, true );
}

void CSceneObject::SetLinearVelocity( const CVec2 &_vel )
{
    mBody->SetLinearVelocity(_vel);
}

void CSceneObject::SetAngularVelocity( float _vel )
{
    mBody->SetAngularVelocity(_vel);
}

void CSceneObject::SetFixedRotation(bool _fixed)
{
    mBody->SetFixedRotation(_fixed);
}

void CSceneObject::SetBullet(bool _bullet)
{
    mBody->SetBullet(_bullet);
}

void CSceneObject::SetActive(bool _active)
{
    mBody->SetActive(_active);
}

} // namespace drash
