#include "csceneobject.h"

#include "../diag/clogger.h"
#include "../diag/assert.h"
#include "explosion.h"
#include "../misc/graphics.h"
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

CSceneObject::CSceneObject(void):
    mBody(NULL),
    mScene(NULL),
    mDead(false),
    mInternalId(-1)
{
    mColor[0] = Randf( 0.15f, 0.8f, 0.01f );
    mColor[1] = Randf( 0.15f, 0.8f, 0.01f );
    mColor[2] = Randf( 0.15f, 0.8f, 0.01f );
}

CSceneObject::~CSceneObject(void)
{
    Release();
}

bool CSceneObject::Init(const GeometryT &_geometry, const CSceneObject::ParamsT &_params )
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

    for ( auto i = _geometry.mFigures.begin(), i_e = _geometry.mFigures.end(); i != i_e; i++ )
    {
        CreateFigure(*i);
    }

    return true;
}

void CSceneObject::Release()
{
    while ( b2Fixture *f = mBody->GetFixtureList() )
    {
        delete reinterpret_cast<CFigure*>( f->GetUserData() );
        f->SetUserData(NULL);
        mBody->DestroyFixture(f);
    }
}

void CSceneObject::Step( double _dt )
{
    if ( mBody->GetType() == b2_kinematicBody )
    {
        if ( mPos.IsTargetSet() )
        {
            mPos.Step(_dt);
            CVec2 lv = mPos.GetTarget();
            lv -= mPos.Get();
            lv /= mPos.GetTimeRemains();
            mBody->SetLinearVelocity(lv);
        }
        else
        {
            mPos.Set( mBody->GetWorldCenter() );
        }

        if ( mAngle.IsTargetSet() )
        {
            mAngle.Step(_dt);
            float av = ( mAngle.GetTarget() - mAngle.Get() ) / mAngle.GetTimeRemains();
            mBody->SetAngularVelocity(av);
        }
        else
        {
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

void CSceneObject::OnContactPreSolve( const CContact &_contact )
{
}

void CSceneObject::OnContactEnd( const CContact &_contact )
{
}

void CSceneObject::OnBoom( const CExplosionParams &_boom )
{
    CVec2 dir( GetBody()->GetWorldCenter() );
    dir -= _boom.mPos;

    float k = std::min( dir.Length(), _boom.mStregth )/ _boom.mStregth;

    dir.Normalize();
    dir *= k * _boom.mStregth;

    ApplyLinearImpulse( dir, mPos.Get() );
}

void CSceneObject::DrawDebug() const
{
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    unsigned int j = 0;
    for ( b2Fixture *f = mBody->GetFixtureList(); f != NULL; f = f->GetNext() )
    {
        if ( f->GetShape()->GetType() == b2Shape::e_polygon )
        {
            auto s = reinterpret_cast<b2PolygonShape*>( f->GetShape() );

            if (s)
            {
                b2Color diffuse( mColor[0], mColor[1], mColor[2] );

                float depth = 1;
                float local_z = 0;

                if (f->GetUserData() != nullptr)
                {
                    CFigure *fg = reinterpret_cast<CFigure*>(f->GetUserData());
                    depth = fg->GetDepth();
                    local_z = fg->GetZet();
                }

                DrawBody(s->m_vertices, s->GetVertexCount(), mZ+local_z, depth, diffuse);
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

void CSceneObject::SetDynamic( bool _dynamic )
{
    mBody->SetType( _dynamic ? b2_dynamicBody : b2_kinematicBody );
}

CSceneObject::CFigurePtr CSceneObject::CreateFigure( const CFigureParams &_params )
{
    if (mFiguresCount >= mFiguresCountLimit)
    {
        LOG_ERR("CSceneObject::CreateFigure(): figures count exceedes it's limit ("<<mFiguresCountLimit<<")");
        return nullptr;
    }

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

    CFigure *figure = new CFigure;

    figure->mFixture = f;
    figure->mMass = _params.mMass;
    figure->mZet = _params.mZet;
    figure->mDepth = _params.mDepth;
    figure->mInternalId = mFiguresCount;

    f->SetUserData(figure);

    mFigures[mFiguresCount++] = figure;

    return figure;
}

void CSceneObject::DestroyFigure(CFigure *_figure)
{
    if (_figure->mInternalId >= mFiguresCountLimit ||
        mFigures[_figure->mInternalId] != _figure)
    {
        LOG_ERR("CSceneObject::DestroyFigure(): something wrong with figures creation logic");
        return;
    }

    if (_figure->mFixture != nullptr)
    {
        mBody->DestroyFixture(_figure->mFixture);
        _figure->mFixture = nullptr;
    }
    else
    {
        LOG_WARN("CSceneObject::DestroyFigure(): empty figure destoyed");
    }

    if (_figure->mInternalId < --mFiguresCount)
    {
        mFigures[_figure->mInternalId] = mFigures[mFiguresCount];
        mFigures[_figure->mInternalId]->mInternalId = _figure->mInternalId;
        mFigures[mFiguresCount] = nullptr;
    }
    else
    {
        mFigures[_figure->mInternalId] = nullptr;
    }

    delete _figure;
}

const CSceneObject::FiguresT &CSceneObject::GetFigures()
{
    return mFigures;
}

unsigned int CSceneObject::EnumFigures() const
{
    return mFiguresCount;
}

void CSceneObject::ApplyLinearImpulse( const CVec2 &_dir, const CVec2 &_pos )
{
    mBody->ApplyLinearImpulse( _dir, _pos );
}

void CSceneObject::SetLinearVelocity( const CVec2 &_vel )
{
    mBody->SetLinearVelocity(_vel);
}

void CSceneObject::SetAngularVelocity( float _vel )
{
    mBody->SetAngularVelocity(_vel);
}

void CSceneObject::SetFixedRotation( bool _fixed )
{
    mBody->SetFixedRotation(_fixed);
}

void CSceneObject::SetActive( bool _active )
{
    mBody->SetActive(_active);
}

void CSceneObject::SetPos(const CVec2 &_pos)
{
    mBody->SetTransform( _pos, mBody->GetAngle() );
    mPos.Set(_pos);
}

void CSceneObject::SetPosTarget( const CVec2 &_target, double _time, const AnimationBehavior &_behavior )
{
    mPos.SetTarget( _target, _time, _behavior );
}

const CAnimatedParam<CVec2> &CSceneObject::GetPos() const
{
    return mPos;
}

void CSceneObject::SetAngle(float _angle)
{
    mBody->SetTransform( mBody->GetWorldCenter(), _angle );
    mAngle.Set(_angle);
}

void CSceneObject::SetAngleTarget( float _target, double _time, const AnimationBehavior &_behavior )
{
    mAngle.SetTarget( _target, _time, _behavior );
}

const CAnimatedParam<float> &CSceneObject::GetAngle() const
{
    return mAngle;
}

void CSceneObject::SetZ(float _z)
{
    mZ = _z;
}

float CSceneObject::GetZ() const
{
    return mZ;
}

void CSceneObject::DumpGeometry(CSceneObject::GeometryT &_geometry) const
{
    _geometry.mFigures.clear();

    for (auto f=mBody->GetFixtureList(); f!=nullptr; f = f->GetNext())
    {
        CFigureParams figure;
        figure.mFriction = f->GetFriction();
        figure.mRestitution = f->GetRestitution();
        b2MassData md;
        f->GetMassData(&md);
        figure.mMass = md.mass;

        if (f->GetShape() != nullptr && f->GetShape()->GetType() == b2Shape::e_polygon)
        {
            b2PolygonShape *s = reinterpret_cast<b2PolygonShape*>(f->GetShape());

            for (int i=0; i<s->GetVertexCount(); i++)
            {
                figure.mVertices.push_back(s->GetVertex(i));
            }
        }

        figure.mDepth = 1;
        figure.mZet = 0;

        if (f->GetUserData() != nullptr)
        {
            CFigure *fg = reinterpret_cast<CFigure*>(f->GetUserData());

            figure.mDepth = fg->GetDepth();
            figure.mZet = fg->GetZet();
        }

        _geometry.mFigures.push_back(std::move(figure));
    }
}

void CSceneObject::DumpParams(CSceneObject::ParamsT &_params) const
{
    _params.mDynamic = (mBody->GetType() == b2_dynamicBody);
    _params.mPos = mBody->GetWorldCenter();
    _params.mAngle = mBody->GetAngle();
    _params.mFixedRotation = mBody->IsFixedRotation();
}

} // namespace drash
