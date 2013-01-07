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

#include "sceneobject.h"

#include "figure.h"
#include "../diag/logger.h"
#include "../diag/assert.h"
#include "../explosion/explosion.h"
#include "../misc/graphics.h"
#include "scene.h"

#include <Box2D/Box2D.h>

namespace drash
{

bool CSceneObject::Init(const GeometryT &_geometry, const CSceneObject::ParamsT &_params )
{
    if ( mBody == nullptr )
    {
        return false;
    }

    mPosXYAnimator = _params.mPos;
	mPosZAnimator = _params.mPos.mZ;
    mAngleAnimator = _params.mAngle;

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
        f->SetUserData(nullptr);
        mBody->DestroyFixture(f);
    }
}

void CSceneObject::Step(double _dt)
{
    mLifeTime += _dt;

    for (unsigned int i = 0; i < mFiguresCount; i++)
    {
        if (mFigures[i]->mDead == true)
        {
            CSceneObjectGeometry g;
            g.mFigures.resize(1);
            g.mFigures[0].mVertices.resize(mFigures[i]->EnumVertices());
            g.mFigures[0].mZ = mFigures[i]->GetZ();
            g.mFigures[0].mDepth = mFigures[i]->GetDepth();
            memcpy(&*g.mFigures[0].mVertices.begin(), mFigures[i]->GetVertices(), sizeof(CVec2f) * mFigures[i]->EnumVertices());

            CSceneObjectParams p;
            p.mAngle = mAngle;
            p.mDynamic = true;
            p.mFixedRotation = false;
            p.mPos = mPos;

//            TODO: move this code to CScene
//            GetScene()->CreateObject<CSceneObject>(g, p);

            DestroyFigure(mFigures[i]);

            break;
        }
    }

    bool phys_pos_changed = false;

    if (mPosXYAnimator.Step(_dt))
    {
        if (mPosXYAnimator.IsTargetSet() == true)
        {
            CVec2f lv = mPosXYAnimator.GetTarget();
            lv -= mPos;
            lv /= mPosXYAnimator.GetTimeRemains();
            mBody->SetLinearVelocity(CVec2ToB2Vec2(lv));
        }
        else
        {
            phys_pos_changed = true;
        }
    }
    else
    {
        mPos.Vec2() = B2Vec2ToCVec2(mBody->GetWorldPoint(b2Vec2(0, 0)));
    }

    if (mPosZAnimator.Step(_dt))
    {
        for (auto i = mCurrentContacts.begin(); i != mCurrentContacts.end(); i++)
        {
            const CFigure *f1 = i->first;
            const CFigure *f2 = i->second;

            float z1 = f1->GetZ() + f1->GetSceneObject()->GetPosZ();
            float z2 = f2->GetZ() + f2->GetSceneObject()->GetPosZ();

            if (math::Abs(z1 - z2) > ((f1->GetDepth() + f2->GetDepth()) * 0.5f))
            {
                mBody->SetActive(false);
                mBody->SetActive(true);
            }
        }
    }

    if (mAngleAnimator.Step(_dt))
    {
        if (mAngleAnimator.IsTargetSet())
        {
            float av = (mAngleAnimator.GetTarget() - mAngle) / mAngleAnimator.GetTimeRemains();
            mBody->SetAngularVelocity(av);
        }
        else
        {
            phys_pos_changed = true;
        }
    }
    else
    {
        mAngle = mBody->GetAngle();
    }

    if (phys_pos_changed)
    {
        mBody->SetTransform(CVec2ToB2Vec2(mPos), mAngle);
    }
}

void CSceneObject::OnContactBegin(const CFigure *_f1, const CFigure *_f2)
{
    CVec3f speed(_f2->GetSceneObject()->GetLinearVelocity(), 0);
    speed.Vec2() -= _f1->GetSceneObject()->GetLinearVelocity();

    if (speed.LengthSquared() > 3000 &&/* mLifeTime > 1 &&*/ mFiguresCount > 1)
    {
        for (unsigned int i = 0; i < mFiguresCount; i++)
        {
            if (mFigures[i] == _f1)
            {
                mFigures[i]->mDead = true;

                return;
            }
        }
    }

    mCurrentContacts.insert(std::pair<const CFigure*, const CFigure*>(_f2, _f1));
}

void CSceneObject::OnContactPreSolve(const CFigure *, const CFigure *)
{
}

void CSceneObject::OnContactEnd(const CFigure *, const CFigure *_f2)
{
    auto f = mCurrentContacts.find(_f2);

    if (f != mCurrentContacts.end())
    {
        mCurrentContacts.erase(f);
    }
}

void CSceneObject::DrawDebug() const
{
    unsigned int j = 0;
    for ( b2Fixture *f = mBody->GetFixtureList(); f != nullptr; f = f->GetNext() )
    {
        if ( f->GetShape()->GetType() == b2Shape::e_polygon )
        {
            auto s = reinterpret_cast<b2PolygonShape*>( f->GetShape() );

            if (s)
            {
                float depth = 1;
                float local_z = 0;

                if (f->GetUserData() != nullptr)
                {
                    CFigure *fg = reinterpret_cast<CFigure*>(f->GetUserData());
                    depth = fg->GetDepth();
                    local_z = fg->GetZ();
                }

                DrawBody(&B2Vec2ToCVec2(*s->m_vertices),
                         s->GetVertexCount(),
                         mPos.mZ + local_z,
                         depth,
                         CColor4f(1.0f, 0.0f, 0.0f, 1.0f),
                         mAngle);
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
    fdef.userData = nullptr;

    b2Fixture *f = mBody->CreateFixture(&fdef);

    CFigure *figure = new CFigure;

    figure->mFixture = f;
    figure->mMass = _params.mMass;
    figure->mDepth = _params.mDepth;
    figure->mInternalId = mFiguresCount;
    figure->mZ = _params.mZ;

    f->SetUserData(figure);

    mFigures[mFiguresCount++] = figure;

    return figure;
}

void CSceneObject::SetPos(const CVec3f _pos)
{
    mPosXYAnimator.Set(_pos);
    mPosZAnimator.Set(_pos.mZ);
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

void CSceneObject::SetDynamic( bool _dynamic )
{
    mBody->SetType( _dynamic ? b2_dynamicBody : b2_kinematicBody );
}

bool CSceneObject::IsDynamic() const
{
    return mBody->GetType() == b2_dynamicBody ? true : false;
}

void CSceneObject::ApplyLinearImpulse( const CVec2f &_dir, const CVec2f &_pos )
{
    mBody->ApplyLinearImpulse(CVec2ToB2Vec2(_dir), CVec2ToB2Vec2(_pos));
}

void CSceneObject::SetLinearVelocity(const CVec2f &_vel)
{
    mBody->SetLinearVelocity(CVec2ToB2Vec2(_vel));
}

CVec2f CSceneObject::GetLinearVelocity() const
{
    return B2Vec2ToCVec2(mBody->GetLinearVelocity());
}

void CSceneObject::SetAngularVelocity(float _vel)
{
    mBody->SetAngularVelocity(_vel);
}

float CSceneObject::GetAngularVelocity() const
{
    return mBody->GetAngularVelocity();
}

void CSceneObject::SetFixedRotation( bool _fixed )
{
    mBody->SetFixedRotation(_fixed);
}

void CSceneObject::SetActive(bool _active)
{
    mBody->SetActive(_active);
}

CVec2f CSceneObject::GetWorldPoint(const CVec2f &_local_point) const
{
    return B2Vec2ToCVec2(mBody->GetWorldPoint(CVec2ToB2Vec2(_local_point)));
}

CVec2f CSceneObject::GetMassCenter() const
{
    return B2Vec2ToCVec2(mBody->GetWorldCenter());
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

void CSceneObject::DumpGeometry(CSceneObjectGeometry *_geometry) const
{
    _geometry->mFigures.resize(EnumFigures());

    for (unsigned int i = 0; i < EnumFigures(); i++)
    {
        _geometry->mFigures[i].mDepth = GetFigures()[i]->GetDepth();
        _geometry->mFigures[i].mZ = GetFigures()[i]->GetZ();
        _geometry->mFigures[i].mFriction = GetFigures()[i]->GetFriction();

        _geometry->mFigures[i].mVertices.resize(GetFigures()[i]->EnumVertices());

        for (unsigned int j = 0; j < GetFigures()[i]->EnumVertices(); j++)
        {
            _geometry->mFigures[i].mVertices[j] = GetFigures()[i]->GetVertices()[j];
        }
    }
}

} // namespace drash
