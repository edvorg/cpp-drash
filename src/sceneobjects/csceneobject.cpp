// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "csceneobject.h"

#include "figure.h"
#include "../diag/clogger.h"
#include "../diag/assert.h"
#include "explosion.h"
#include "../misc/graphics.h"

namespace drash
{

CSceneObject::CSceneObject(void):
    mPos([this] (const CVec3f &_new_value)
    {
        mBody->SetTransform(CVec2ToB2Vec2(_new_value), mBody->GetAngle());
    }),
    mAngle([this] (const float &_new_value)
    {
        mBody->SetTransform(mBody->GetWorldPoint(b2Vec2(0, 0)), _new_value);
    })
{
    mColor[0] = math::Rand<float>(0.35f, 0.9f, 0.01f);
    mColor[1] = math::Rand<float>( 0.35f, 0.9f, 0.01f);
    mColor[2] = math::Rand<float>(0.35f, 0.9f, 0.01f);
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

    mBody->SetTransform(CVec2ToB2Vec2(_params.mPos), _params.mAngle);
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

void CSceneObject::Step(double _dt)
{
    if (mPos.IsTargetSet())
    {
        mPos.Step(_dt);
        CVec2f lv = mPos.GetTarget().Vec2();
        lv -= mPos.Get().Vec2();
        lv /= mPos.GetTimeRemains();
        mBody->SetLinearVelocity(CVec2ToB2Vec2(lv));
    }
    else
    {
        mPos.Set(CVec3f(B2Vec2ToCVec2(mBody->GetWorldPoint(b2Vec2(0, 0))), mPos.Get().mZ));
    }

    if (mAngle.IsTargetSet())
    {
        mAngle.Step(_dt);
        float av = (mAngle.GetTarget() - mAngle.Get()) / mAngle.GetTimeRemains();
        mBody->SetAngularVelocity(av);
    }
    else
    {
        mAngle.Set(mBody->GetAngle());
    }
}

void CSceneObject::OnContactBegin(const CFigure *, const CFigure *)
{
}

void CSceneObject::OnContactPreSolve(const CFigure *, const CFigure *)
{
}

void CSceneObject::OnContactEnd(const CFigure *, const CFigure *)
{
}

void CSceneObject::OnBoom( const CExplosionParams &_boom )
{
    CVec2f dir(GetPos().Get().Vec2());
    dir -= _boom.mPos.Vec2();

    float k = drash::math::Min( dir.Length(), _boom.mStregth )/ _boom.mStregth;

    dir.Normalize();
    dir *= k * _boom.mStregth;

    ApplyLinearImpulse(dir, mPos.Get().Vec2());
}

void CSceneObject::DrawDebug() const
{
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
                    local_z = fg->GetZ();
                }

                DrawBody(&B2Vec2ToCVec2(*s->m_vertices), s->GetVertexCount(), mPos.Get().mZ+local_z, depth, diffuse);
            }
        }
        j++;
    }
}

CFigure *CSceneObject::CreateFigure(const CFigureParams &_params)
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
        s.Set( &CVec2ToB2Vec2(*_params.mVertices.begin()), _params.mVertices.size() );
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
    figure->mDepth = _params.mDepth;
    figure->mInternalId = mFiguresCount;

    f->SetUserData(figure);

    mFigures[mFiguresCount++] = figure;

    return figure;
}

void CSceneObject::DestroyFigure(CFigure *_figure)
{
    if (_figure->mInternalId >= static_cast<int>(mFiguresCountLimit) ||
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

    if (static_cast<unsigned int>(_figure->mInternalId) < --mFiguresCount)
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

void CSceneObject::ComputeBoundingBox()
{
    mBoundingBox.lowerBound = b2Vec2(FLT_MAX,FLT_MAX);
    mBoundingBox.upperBound = b2Vec2(-FLT_MAX,-FLT_MAX);
    b2Fixture* fixture = mBody->GetFixtureList();
    while (fixture != NULL)
    {
        mBoundingBox.Combine(mBoundingBox, fixture->GetAABB(0));
        fixture = fixture->GetNext();
    }
}

CLogger &operator <<(CLogger &_logger, const CSceneObject &_object)
{
    _logger<<"pos: "<<_object.mPos<<" angle: "<<_object.mAngle<<'\n';
    for (unsigned int i = 0; i < _object.EnumFigures(); i++)
    {
        _logger<<*_object.GetFigures()[i]<<'\b';
    }
    return _logger;
}

CSceneObjectGeometry *CSceneObject::GetGeometry() const
{
    CSceneObjectGeometry *geo = new CSceneObjectGeometry();
    std::vector<CFigureParams> res;
    for (unsigned int i = 0 ; i < mFiguresCount ; i++) {
        CFigureParams buff;
        CFigure* fig = mFigures[i];
        buff.mDepth = fig->GetDepth();
        buff.mFriction = fig->GetFriction();
        buff.mMass = fig->mMass;
        buff.mRestitution = fig->mFixture->GetRestitution();
        for (int i = 0 , count = fig->EnumVertices() ; i < count ; i++) {
            buff.mVertices.push_back(fig->GetVertices()[i]);
        }
        res.push_back(buff);
    }
    geo->mFigures = res;
    return geo;
}

} // namespace drash
