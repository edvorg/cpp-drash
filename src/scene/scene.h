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

#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <Box2D/Box2D.h>
#include "../misc/vec3.h"
#include "../misc/objectfactory.h"

class b2World;

namespace drash {

    class PhysObserver;
    class SceneObjectGeometry;
    class SceneObjectParams;
    class SceneObject;
    class Joint;

    class SceneParams {
    public:
        Vec2f gravity = { 0, -9.8 };
    };

    class Scene final : public b2ContactListener,
                         public b2ContactFilter,
                         public b2DestructionListener {
    public:
        // **************************************************
        // * static scene configuration *********************

        static const int velocityIterations = 5;
        static const int positionIterations = 2;
        static const unsigned int objectsCountLimit = 10000;
        static const unsigned int subsystemsCountLimit = 5;

        // **************************************************
        // * main routines **********************************

        Scene(const SceneParams& _params);
        ~Scene(void);

        /// must be called once in update cycle
        /// dt - seconds
        void Step(double _dt);

        // **************************************************
        // * working with objects ***************************

        SceneObject* CreateObject(const SceneObjectGeometry& _geometry,
                                   const SceneObjectParams& _params);
        bool DestroyObject(SceneObject* _obj);
        inline SceneObject* const* GetObjects(void) const;
        inline unsigned int EnumObjects(void) const;
        void DestroyObjects(void);

        // **************************************************
        // * working with joints ****************************

        Joint* CreateJoint(SceneObject* _obj1, SceneObject* _obj2,
                            const Vec3f& _anchor);
        Joint* CreateJointDistance(SceneObject* _obj1, SceneObject* _obj2,
                                    const Vec3f& _anchor1,
                                    const Vec3f& _anchor2, float _length);
        Joint* CreateJointRope(SceneObject* _obj1, SceneObject* _obj2,
                                const Vec3f& _anchor1, const Vec3f& _anchor2,
                                float _length);
        void DestroyJoint(Joint* _joint);

        void SetGravity(const Vec2f& _g);

        inline void SetPaused(bool _paused);

    protected:
    private:
        void DestroyObjectImpl(SceneObject* _obj);

        virtual bool ShouldCollide(b2Fixture* fixtureA,
                                   b2Fixture* fixtureB) override;
        virtual void BeginContact(b2Contact* _figure) override;
        virtual void PreSolve(b2Contact* _contact,
                              const b2Manifold* _old_manifold) override;
        virtual void PostSolve(b2Contact* _contact,
                               const b2ContactImpulse* _impulse) override;
        virtual void EndContact(b2Contact* _figure) override;
        virtual void SayGoodbye(b2Joint* _joint) override;
        virtual void SayGoodbye(b2Fixture* _fixture) override;

        b2World world;
        ObjectFactory<SceneObject> objectsFactory;
        bool locked = false;
        bool paused = false;
    };

    inline SceneObject* const* Scene::GetObjects(void) const {
        return objectsFactory.GetObjects();
    }

    inline unsigned int Scene::EnumObjects(void) const {
        return objectsFactory.EnumObjects();
    }

    inline void Scene::SetPaused(bool _paused) { paused = _paused; }

} // namespace drash

#endif // SCENE_H
