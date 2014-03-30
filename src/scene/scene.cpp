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

#include "scene.h"

#include "../diag/assert.h"
#include "../diag/logger.h"

#include "sceneobject.h"
#include "figure.h"
#include "joint.h"

#include <Box2D/Box2D.h>

namespace drash {

    Scene::Scene(const SceneParams& _params)
        : world(b2Vec2(0, 0)),
          objectsFactory(objectsCountLimit, "SceneObject") {
        world.SetContactFilter(this);
        world.SetContactListener(this);
        world.SetAllowSleeping(true);
        world.SetContinuousPhysics(false);
        world.SetGravity(Vec2ToB2Vec2(_params.gravity));
    }

    Scene::~Scene() {
        while (auto j = world.GetJointList()) {
            delete static_cast<Joint*>(j->GetUserData());
            j->SetUserData(nullptr);
            world.DestroyJoint(j);
        }

        DestroyObjects();
    }

    void Scene::Step(double _dt) {
        if (paused == true) {
            return;
        }

        locked = true;

        for (unsigned int i = 0; i < objectsFactory.EnumObjects();) {
            if (objectsFactory.GetObjects()[i]->dead) {
                DestroyObjectImpl(objectsFactory.GetObjects()[i]);
            } else {
                for (unsigned int j = 0;
                     j < objectsFactory.GetObjects()[i]->figuresCount; j++) {
                    if (objectsFactory.GetObjects()[i]->figures[j]->dead ==
                        true) {
                        // detaching figure

                        SceneObjectGeometry g;
                        g.figures.resize(1);
                        g.figures[0]
                            .vertices.resize(objectsFactory.GetObjects()[i]
                                                 ->figures[j]
                                                 ->EnumVertices());
                        g.figures[0].z =
                            objectsFactory.GetObjects()[i]->figures[j]->GetZ();
                        g.figures[0].depth = objectsFactory.GetObjects()[i]
                                                 ->figures[j]
                                                 ->GetDepth();
                        memcpy(&*g.figures[0].vertices.begin(),
                               objectsFactory.GetObjects()[i]
                                   ->figures[j]
                                   ->GetVertices(),
                               sizeof(Vec2f) * objectsFactory.GetObjects()[i]
                                                    ->figures[j]
                                                    ->EnumVertices());

                        SceneObjectParams p;
                        p.angle = objectsFactory.GetObjects()[i]->angle;
                        p.dynamic = true;
                        p.fixedRotation = false;
                        p.pos = objectsFactory.GetObjects()[i]->pos;

                        auto o = CreateObject(g, p);

                        o->SetLinearVelocity(objectsFactory.GetObjects()[i]
                                                 ->GetLinearVelocity());
                        o->SetAngularVelocity(objectsFactory.GetObjects()[i]
                                                  ->GetAngularVelocity());

                        objectsFactory.GetObjects()[i]->DestroyFigure(
                            objectsFactory.GetObjects()[i]->figures[j]);

                        // recompute destruction graph (TODO: optimize this)

                        SceneObjectGeometry ng;
                        objectsFactory.GetObjects()[i]->DumpGeometry(&ng);

                        ng.ComputeDestructionGraph(0.5);

                        objectsFactory.GetObjects()[i]->destructionGraph =
                            ng.destructionGraph;

                        // looking for connectivity

                        std::vector<int> used;
                        used.resize(ng.figures.size());
                        memset(&*used.begin(), 0,
                               sizeof(int) * ng.figures.size());
                        std::vector<int> comp;

                        std::function<void(unsigned int _v)> dfs;

                        dfs = [&](unsigned int _v) {
                            used[_v] = 1;
                            comp.push_back(_v);

                            for (unsigned int a = 0; a < ng.figures.size();
                                 a++) {
                                if (ng.destructionGraph
                                        [_v * ng.figures.size() + a] != 0) {
                                    if (used[a] == 0) {
                                        dfs(a);
                                    }
                                }
                            }
                        };

                        for (unsigned int a = 0; a < ng.figures.size(); a++) {
                            if (used[a] == 0) {
                                comp.clear();
                                dfs(a);

                                SceneObjectGeometry g1;
                                g1.figures.resize(comp.size());

                                for (unsigned int b = 0; b < comp.size(); b++) {
                                    g1.figures[b].vertices.resize(
                                        ng.figures[comp[b]].vertices.size());
                                    g1.figures[b].z = ng.figures[comp[b]].z;
                                    g1.figures[b].depth =
                                        ng.figures[comp[b]].depth;
                                    memcpy(
                                        &*g1.figures[b].vertices.begin(),
                                        &*ng.figures[comp[b]].vertices.begin(),
                                        sizeof(Vec2f) * ng.figures[comp[b]]
                                                             .vertices.size());
                                }

                                g1.ComputeDestructionGraph(0.5);

                                SceneObjectParams p1;
                                p1.angle =
                                    objectsFactory.GetObjects()[i]->angle;
                                p1.dynamic = true;
                                p1.fixedRotation = false;
                                p1.pos = objectsFactory.GetObjects()[i]->pos;

                                auto o = CreateObject(g1, p1);

                                o->SetLinearVelocity(
                                    objectsFactory.GetObjects()[i]
                                        ->GetLinearVelocity());
                                o->SetAngularVelocity(
                                    objectsFactory.GetObjects()[i]
                                        ->GetAngularVelocity());
                            }
                        }

                        DestroyObjectImpl(objectsFactory.GetObjects()[i]);

                        break;
                    }
                }

                auto o = objectsFactory.GetObjects()[i];

                o->lifeTime += _dt;

                bool phys_pos_changed = false;

                if (o->posXYAnimator.Step(_dt)) {
                    if (o->posXYAnimator.IsTargetSet() == true) {
                        Vec2f lv = o->posXYAnimator.GetTarget();
                        lv -= o->pos;
                        lv /= o->posXYAnimator.GetTimeRemains();
                        o->body->SetLinearVelocity(Vec2ToB2Vec2(lv));
                    } else {
                        o->body->SetLinearVelocity(b2Vec2(0, 0));
                        phys_pos_changed = true;
                    }
                } else {
                    o->pos.AsVec2() =
                        B2Vec2ToCVec2(o->body->GetWorldPoint(b2Vec2(0, 0)));
                }

                if (o->posZAnimator.Step(_dt)) {
                    for (auto i = o->currentContacts.begin();
                         i != o->currentContacts.end(); i++) {
                        const Figure* f1 = i->first;
                        const Figure* f2 = i->second;

                        float z1 = f1->GetZ() + f1->GetSceneObject()->GetPosZ();
                        float z2 = f2->GetZ() + f2->GetSceneObject()->GetPosZ();

                        if (math::Abs(z1 - z2) >
                            ((f1->GetDepth() + f2->GetDepth()) * 0.5f)) {
                            o->body->SetActive(false);
                            o->body->SetActive(true);
                        }
                    }
                }

                if (o->angleAnimator.Step(_dt)) {
                    if (o->angleAnimator.IsTargetSet()) {
                        float av = (o->angleAnimator.GetTarget() - o->angle) /
                                   o->angleAnimator.GetTimeRemains();
                        o->body->SetAngularVelocity(av);
                    } else {
                        o->body->SetAngularVelocity(0);
                        phys_pos_changed = true;
                    }
                } else {
                    o->angle = o->body->GetAngle();
                }

                if (phys_pos_changed) {
                    o->body->SetTransform(Vec2ToB2Vec2(o->pos), o->angle);
                }

                i++;
            }
        }

        locked = false;

        world.Step(_dt, velocityIterations, positionIterations);
    }

    SceneObject* Scene::CreateObject(const SceneObjectGeometry& _geometry,
                                       const SceneObjectParams& _params) {
        if (world.IsLocked()) {
            LOG_ERR("Scene::CreateObject(): world is locked now");
            return nullptr;
        }

        SceneObject* res = objectsFactory.CreateObject();

        if (res == nullptr) {
            return nullptr;
        }

        b2BodyDef bdef;
        bdef.position = Vec2ToB2Vec2(_params.pos);
        bdef.angle = _params.angle;
        bdef.active = true;
        bdef.awake = true;
        bdef.allowSleep = true;
        bdef.userData = res;
        bdef.angularDamping = 0;
        bdef.bullet = false;
        bdef.fixedRotation = _params.fixedRotation;
        bdef.linearDamping = 0;
        bdef.gravityScale = 1;
        bdef.type = _params.dynamic ? b2_dynamicBody : b2_kinematicBody;

        b2Body* b = world.CreateBody(&bdef);

        if (b == nullptr) {
            LOG_ERR("Scene::CreateObject(): something wrong with box2d");
            return nullptr;
        }

        res->body = b;
        res->pos = _params.pos;
        res->angle = _params.angle;
        res->destructionGraph = _geometry.destructionGraph;

        for (auto i = _geometry.figures.begin(), i_e = _geometry.figures.end();
             i != i_e; i++) {
            res->CreateFigure(*i);
        }

        return res;
    }

    bool Scene::DestroyObject(SceneObject* _obj) {
        if (objectsFactory.IsObject(_obj) == false) {
            LOG_ERR("Scene::DestroyObject(): invalid object taken");
            return false;
        }

        if (locked == false && world.IsLocked() == false) {
            DestroyObjectImpl(_obj);
        } else {
            _obj->dead = true;
        }

        return true;
    }

    void Scene::DestroyObjects(void) {
        while (objectsFactory.EnumObjects() != 0) {
            DestroyObjectImpl(objectsFactory.GetObjects()[0]);
        }
    }

    Joint* Scene::CreateJoint(SceneObject* _obj1, SceneObject* _obj2,
                                const Vec3f& _anchor) {
        b2WeldJointDef jdef;
        jdef.Initialize(_obj1->body, _obj2->body,
                        Vec2ToB2Vec2(_anchor.AsVec2()));

        Joint* res = new Joint;
        res->joint = world.CreateJoint(&jdef);
        ;
        res->joint->SetUserData(res);
        return res;
    }

    Joint* Scene::CreateJointDistance(SceneObject* _obj1,
                                        SceneObject* _obj2,
                                        const Vec3f& _anchor1,
                                        const Vec3f& _anchor2, float _length) {
        b2DistanceJointDef jdef;
        jdef.Initialize(_obj1->body, _obj2->body,
                        Vec2ToB2Vec2(_anchor1.AsVec2()),
                        Vec2ToB2Vec2(_anchor2.AsVec2()));
        jdef.length = _length;

        Joint* res = new Joint;
        res->joint = world.CreateJoint(&jdef);
        ;
        res->joint->SetUserData(res);
        return res;
    }

    Joint* Scene::CreateJointRope(SceneObject* _obj1, SceneObject* _obj2,
                                    const Vec3f& _anchor1,
                                    const Vec3f& _anchor2, float _length) {
        b2RopeJointDef jdef;
        jdef.maxLength = _length;
        jdef.localAnchorA = Vec2ToB2Vec2(_anchor1.AsVec2());
        jdef.localAnchorB = Vec2ToB2Vec2(_anchor2.AsVec2());
        jdef.bodyA = _obj1->body;
        jdef.bodyB = _obj2->body;
        jdef.type = e_ropeJoint;
        jdef.collideConnected = false;

        Joint* res = new Joint;
        res->joint = world.CreateJoint(&jdef);
        ;
        res->joint->SetUserData(res);
        return res;
    }

    void Scene::DestroyJoint(Joint* _joint) {
        for (auto j = world.GetJointList(); j != nullptr; j = j->GetNext()) {
            if (j->GetUserData() == _joint) {
                delete static_cast<Joint*>(j->GetUserData());
                world.DestroyJoint(j);
                return;
            }
        }
    }

    void Scene::SetGravity(const Vec2f& _g) {
        world.SetGravity(Vec2ToB2Vec2(_g));
    }

    void Scene::DestroyObjectImpl(SceneObject* _obj) {
        for (auto& i : _obj->destroyHandlers) {
            i(_obj);
        }

        _obj->body->SetActive(false);

        while (_obj->EnumFigures() != 0) {
            _obj->DestroyFigure(_obj->GetFigures()[0]);
        }
        _obj->body->SetUserData(nullptr);

        world.DestroyBody(_obj->body);

        _obj->body = nullptr;

        objectsFactory.DestroyObject(_obj);
    }

    bool Scene::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB) {
        if (fixtureA->GetUserData() == nullptr ||
            fixtureB->GetUserData() == nullptr ||
            fixtureA->GetBody()->GetUserData() == nullptr ||
            fixtureB->GetBody()->GetUserData() == nullptr) {
            return false;
        }

        SceneObject* o1 =
            static_cast<SceneObject*>(fixtureA->GetBody()->GetUserData());
        SceneObject* o2 =
            static_cast<SceneObject*>(fixtureB->GetBody()->GetUserData());

        Figure* f1 = static_cast<Figure*>(fixtureA->GetUserData());
        Figure* f2 = static_cast<Figure*>(fixtureB->GetUserData());

        float z1 = f1->GetZ() + o1->GetPosZ();
        float z2 = f2->GetZ() + o2->GetPosZ();

        return drash::math::Abs(z1 - z2) <
               (f1->GetDepth() * 0.5 + f2->GetDepth() * 0.5);
    }

    void Scene::BeginContact(b2Contact* _contact) {
        b2ContactListener::BeginContact(_contact);

        Figure* f1 =
            static_cast<Figure*>(_contact->GetFixtureA()->GetUserData());
        Figure* f2 =
            static_cast<Figure*>(_contact->GetFixtureB()->GetUserData());

        if (f1 == nullptr || f2 == nullptr) {
            LOG_WARN("ContactListener::BeginContact(): "
                     "can not get pointer for one of figures. Skipping");
            return;
        }

        if (f1 == f2 || f1->GetSceneObject() == f2->GetSceneObject()) {
            LOG_WARN("ContactListener::BeginContact(): "
                     "it's seems that pair of figures is part of same object");
            return;
        }

        for (auto& i : f1->GetSceneObject()->contactBeginHandlers) {
            i(f1, f2);
        }
        for (auto& i : f2->GetSceneObject()->contactBeginHandlers) {
            i(f2, f1);
        }

        // ---------------------------------------------------------------------------------------------
        f1->GetSceneObject()->currentContacts.insert(
            std::pair<const Figure*, const Figure*>(f2, f1));
        f2->GetSceneObject()->currentContacts.insert(
            std::pair<const Figure*, const Figure*>(f1, f2));
        // ^
        // | this code block required for correct object movement along Z axis
        // | look for SceneObject::() implementation
        // ---------------------------------------------------------------------------------------------

        Vec3f speed(f2->GetSceneObject()->GetLinearVelocity(), 0);
        speed.AsVec2() -= f1->GetSceneObject()->GetLinearVelocity();

        if (speed.Length() > 10) {
            if (f1->GetSceneObject()->IsDynamic() &&
                f1->GetSceneObject()->figuresCount > 1 &&
                f1->GetSceneObject()->lifeTime > 0.1) {
                f1->dead = true;
            }

            if (f2->GetSceneObject()->IsDynamic() &&
                f2->GetSceneObject()->figuresCount > 1 &&
                f2->GetSceneObject()->lifeTime > 0.1) {
                f2->dead = true;
            }
        }
    }

    void Scene::PreSolve(b2Contact* _contact,
                          const b2Manifold* _old_manifold) {
        b2ContactListener::PreSolve(_contact, _old_manifold);

        /*
        Figure *f1 =
        static_cast<Figure*>(_contact->GetFixtureA()->GetUserData());
        Figure *f2 =
        static_cast<Figure*>(_contact->GetFixtureB()->GetUserData());

        if ( f1 == nullptr || f2 == nullptr )
        {
            LOG_WARN( "ContactListener::BeginContact(): "
                      "can not get pointer for one of figures. Skipping" );
            return;
        }

        if ( f1 == f2 || f1->GetSceneObject() == f2->GetSceneObject() )
        {
            LOG_WARN( "ContactListener::BeginContact(): "
                      "it's seems that pair of figures is part of same object"
        );
            return;
        }
        */
    }

    void Scene::PostSolve(b2Contact* _contact,
                           const b2ContactImpulse* _impulse) {
        b2ContactListener::PostSolve(_contact, _impulse);
    }

    void Scene::EndContact(b2Contact* _contact) {
        b2ContactListener::EndContact(_contact);

        Figure* f1 =
            static_cast<Figure*>(_contact->GetFixtureA()->GetUserData());
        Figure* f2 =
            static_cast<Figure*>(_contact->GetFixtureB()->GetUserData());

        if (f1 == nullptr || f2 == nullptr) {
            LOG_WARN("ContactListener::BeginContact(): "
                     "can not get pointer for one of figures. Skipping");
            return;
        }

        if (f1 == f2 || f1->GetSceneObject() == f2->GetSceneObject()) {
            LOG_WARN("ContactListener::BeginContact(): "
                     "it's seems that pair of figures is part of same object");
            return;
        }

        for (auto& i : f1->GetSceneObject()->contactEndHandlers) {
            i(f1, f2);
        }
        for (auto& i : f2->GetSceneObject()->contactEndHandlers) {
            i(f2, f1);
        }

        // ---------------------------------------------------------------------------------------------
        auto f = f1->GetSceneObject()->currentContacts.find(f2);

        if (f != f1->GetSceneObject()->currentContacts.end()) {
            f1->GetSceneObject()->currentContacts.erase(f);
        }

        f = f2->GetSceneObject()->currentContacts.find(f1);

        if (f != f2->GetSceneObject()->currentContacts.end()) {
            f2->GetSceneObject()->currentContacts.erase(f);
        }
        // ^
        // | this code block required for correct object movement along Z axis
        // | look for Scene::Step() implementation
        // ---------------------------------------------------------------------------------------------
    }

    void Scene::SayGoodbye(b2Joint* _joint) {}

    void Scene::SayGoodbye(b2Fixture* _fixture) {}

} // namespace drash
