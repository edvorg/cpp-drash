// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2014 Edward Knyshov, Yuriy Shatilin.

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
#include "scene.h"

#include <Box2D/Box2D.h>

namespace drash {

    void CSceneObjectGeometry::ComputeDestructionGraph(const float _accuracy) {
        destructionGraph.clear();
        destructionGraph.resize(figures.size() * figures.size());
        memset(&*destructionGraph.begin(), 0,
               sizeof(int) * destructionGraph.size());

        // TODO: optimize this (o(n^2) now)

        for (unsigned i = 0; i < figures.size(); i++) {
            for (unsigned j = 0; j < figures.size(); j++) {
                int cont = 0;

                for (unsigned int k = 0;
                     k < figures[i].vertices.size() && cont < 2; k++) {
                    for (unsigned int l = 0;
                         l < figures[j].vertices.size() && cont < 2; l++) {
                        CVec2f dist = figures[i].vertices[k];
                        dist -= figures[j].vertices[l];

                        unsigned int res = 0;

                        if (dist.Length() < _accuracy) {
                            cont++;
                        }

                        if (cont >= 2) {
                            res = 1;
                        }

                        destructionGraph[i * figures.size() + j] = res;
                        destructionGraph[j * figures.size() + i] = res;
                    }
                }
            }
        }
    }

    CFigure* CSceneObject::CreateFigure(const CFigureParams& _params) {
        if (figuresCount >= figuresCountLimit) {
            LOG_ERR("CSceneObject::CreateFigure(): figures count exceedes it's "
                    "limit ("
                    << figuresCountLimit << ")");
            return nullptr;
        }

        b2PolygonShape s;

        if (_params.vertices.size() == 0) {
            s.SetAsBox(1.0f, 1.0f);
        } else {
            s.Set(&CVec2ToB2Vec2(*_params.vertices.begin()),
                  _params.vertices.size());
        }

        b2MassData md;
        s.ComputeMass(&md, 1.0);

        b2FixtureDef fdef;
        fdef.density = _params.mass / md.mass;
        fdef.friction = _params.friction;
        fdef.isSensor = false;
        fdef.restitution = _params.restitution;
        fdef.shape = &s;
        fdef.userData = nullptr;

        b2Fixture* f = body->CreateFixture(&fdef);

        CFigure* figure = new CFigure;

        figure->fixture = f;
        figure->mass = _params.mass;
        figure->depth = _params.depth;
        figure->internalId = figuresCount;
        figure->z = _params.z;

        f->SetUserData(figure);

        figures[figuresCount++] = figure;

        return figure;
    }

    void CSceneObject::SetPos(const CVec3f _pos) {
        posXYAnimator.Set(_pos);
        posZAnimator.Set(_pos.z);
    }

    void CSceneObject::DestroyFigure(CFigure* _figure) {
        if (_figure->internalId >= static_cast<int>(figuresCountLimit) ||
            figures[_figure->internalId] != _figure) {
            LOG_ERR("CSceneObject::DestroyFigure(): something wrong with "
                    "figures creation logic");
            return;
        }

        for (auto& i : figureDestroyHandlers) {
            i(_figure);
        }
        for (auto& i : _figure->destroyHandlers) {
            i(_figure);
        }

        if (_figure->fixture != nullptr) {
            body->DestroyFixture(_figure->fixture);
            _figure->fixture = nullptr;
        } else {
            LOG_WARN("CSceneObject::DestroyFigure(): empty figure destoyed");
        }

        if (static_cast<unsigned int>(_figure->internalId) < --figuresCount) {
            figures[_figure->internalId] = figures[figuresCount];
            figures[_figure->internalId]->internalId = _figure->internalId;
            figures[figuresCount] = nullptr;
        } else {
            figures[_figure->internalId] = nullptr;
        }

        delete _figure;
    }

    void CSceneObject::SetDynamic(bool _dynamic) {
        body->SetType(_dynamic ? b2_dynamicBody : b2_kinematicBody);
    }

    bool CSceneObject::IsDynamic() const {
        return body->GetType() == b2_dynamicBody ? true : false;
    }

    void CSceneObject::ApplyLinearImpulse(const CVec2f& _dir,
                                          const CVec2f& _pos) {
        body->ApplyLinearImpulse(CVec2ToB2Vec2(_dir), CVec2ToB2Vec2(_pos),
                                 true);
    }

    void CSceneObject::SetLinearVelocity(const CVec2f& _vel) {
        body->SetLinearVelocity(CVec2ToB2Vec2(_vel));
    }

    CVec2f CSceneObject::GetLinearVelocity() const {
        return B2Vec2ToCVec2(body->GetLinearVelocity());
    }

    void CSceneObject::SetAngularVelocity(float _vel) {
        body->SetAngularVelocity(_vel);
    }

    float CSceneObject::GetAngularVelocity() const {
        return body->GetAngularVelocity();
    }

    void CSceneObject::SetFixedRotation(bool _fixed) {
        body->SetFixedRotation(_fixed);
    }

    void CSceneObject::SetActive(bool _active) { body->SetActive(_active); }

    CVec2f CSceneObject::GetWorldPoint(const CVec2f& _local_point) const {
        return B2Vec2ToCVec2(body->GetWorldPoint(CVec2ToB2Vec2(_local_point)));
    }

    CVec2f CSceneObject::GetMassCenter() const {
        return B2Vec2ToCVec2(body->GetWorldCenter());
    }

    CLogger& operator<<(CLogger& _logger, const CSceneObject& _object) {
        _logger << "pos: " << _object.pos << " angle: " << _object.angle
                << '\n';
        for (unsigned int i = 0; i < _object.EnumFigures(); i++) {
            _logger << *_object.GetFigures()[i] << '\b';
        }
        return _logger;
    }

    void CSceneObject::DumpGeometry(CSceneObjectGeometry* _geometry) const {
        _geometry->figures.resize(EnumFigures());

        for (unsigned int i = 0; i < EnumFigures(); i++) {
            _geometry->figures[i].depth = GetFigures()[i]->GetDepth();
            _geometry->figures[i].z = GetFigures()[i]->GetZ();
            _geometry->figures[i].friction = GetFigures()[i]->GetFriction();

            _geometry->figures[i]
                .vertices.resize(GetFigures()[i]->EnumVertices());

            for (unsigned int j = 0; j < GetFigures()[i]->EnumVertices(); j++) {
                _geometry->figures[i].vertices[j] =
                    GetFigures()[i]->GetVertices()[j];
            }
        }
    }

} // namespace drash
