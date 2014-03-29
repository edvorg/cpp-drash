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
#ifndef CSCENE_H
#define CSCENE_H

#include <Box2D/Box2D.h>
#include "../misc/vec3.h"
#include "../misc/objectfactory.h"

class b2World;

namespace drash {

    class CPhysObserver;
    class CSceneObjectGeometry;
    class CSceneObjectParams;
    class CSceneObject;
    class CJoint;

    class CSceneParams {
      public:
        CVec2f gravity = { 0, -9.8 };
    };

    class CScene final : public b2ContactListener,
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

        CScene(const CSceneParams& _params);
        ~CScene(void);

        /// must be called once in update cycle
        /// dt - seconds
        void Step(double _dt);

        // **************************************************
        // * working with objects ***************************

        CSceneObject* CreateObject(const CSceneObjectGeometry& _geometry,
                                   const CSceneObjectParams& _params);
        bool DestroyObject(CSceneObject* _obj);
        inline CSceneObject* const* GetObjects(void) const;
        inline unsigned int EnumObjects(void) const;
        void DestroyObjects(void);

        // **************************************************
        // * working with joints ****************************

        CJoint* CreateJoint(CSceneObject* _obj1, CSceneObject* _obj2,
                            const CVec3f& _anchor);
        CJoint* CreateJointDistance(CSceneObject* _obj1, CSceneObject* _obj2,
                                    const CVec3f& _anchor1,
                                    const CVec3f& _anchor2, float _length);
        CJoint* CreateJointRope(CSceneObject* _obj1, CSceneObject* _obj2,
                                const CVec3f& _anchor1, const CVec3f& _anchor2,
                                float _length);
        void DestroyJoint(CJoint* _joint);

        void SetGravity(const CVec2f& _g);

        inline void SetPaused(bool _paused);

      protected:
      private:
        void DestroyObjectImpl(CSceneObject* _obj);

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
        CObjectFactory<CSceneObject> objectsFactory;
        bool locked = false;
        bool paused = false;
    };

    inline CSceneObject* const* CScene::GetObjects(void) const {
        return objectsFactory.GetObjects();
    }

    inline unsigned int CScene::EnumObjects(void) const {
        return objectsFactory.EnumObjects();
    }

    inline void CScene::SetPaused(bool _paused) { paused = _paused; }

} // namespace drash

#endif // CSCENE_H
