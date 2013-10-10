// DRASH_LICENSE_BEGIN
/*

  drash GPL Source Code
  Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

  This file is part of the drash GPL Source Code (drash Source Code).

  drash Source Code is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  drash Source Code is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with drash Source Code. If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "sceneobject.h"

#include "figure.h"
#include "../diag/logger.h"
#include "../diag/assert.h"
#include "scene.h"

#include <Box2D/Box2D.h>

namespace drash
{

void CSceneObjectGeometry::ComputeDestructionGraph(const float _accuracy)
{
    mDestructionGraph.clear();
    mDestructionGraph.resize(mFigures.size() * mFigures.size());
    memset(&*mDestructionGraph.begin(), 0, sizeof(int) * mDestructionGraph.size());

    // TODO: optimize this (o(n^2) now)

    for (unsigned i = 0; i < mFigures.size(); i++)
    {
        for (unsigned j = 0; j < mFigures.size(); j++)
        {
            int cont = 0;

            for (unsigned int k = 0; k < mFigures[i].mVertices.size() && cont < 2; k++)
            {
                for (unsigned int l = 0; l < mFigures[j].mVertices.size() && cont < 2; l++)
                {
                    CVec2f dist = mFigures[i].mVertices[k];
                    dist -= mFigures[j].mVertices[l];

                    unsigned int res = 0;

                    if (dist.Length() < _accuracy)
                    {
                        cont++;
                    }

                    if (cont >= 2)
                    {
                        res = 1;
                    }

                    mDestructionGraph[i * mFigures.size() + j] = res;
                    mDestructionGraph[j * mFigures.size() + i] = res;
                }
            }
        }
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

    for (auto & i : mFigureDestroyHandlers)
    {
        i(_figure);
    }
    for (auto & i : _figure->mDestroyHandlers)
    {
        i(_figure);
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
