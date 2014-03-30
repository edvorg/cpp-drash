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
#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <functional>
#include <vector>
#include <map>
#include "../misc/animator.h"
#include "../misc/vec3.h"
#include "../misc/objectfactory.h"

class b2Body;

namespace drash {

    class Scene;
    class FigureParams;
    class Figure;
    class ExplosionParams;

    class SceneObjectGeometry final {
    public:
        std::vector<FigureParams> figures;
        std::vector<unsigned int> destructionGraph;

        void ComputeDestructionGraph(const float _accuracy);

    protected:
    private:
    };

    class SceneObjectParams final {
    public:
        bool dynamic = true;
        Vec3f pos;
        float angle = 0;
        bool fixedRotation = false;
    };

    class SceneObject final
        : public ObjectFactory<SceneObject>::FactoryProduct {
    public:
        friend class Scene;
        friend class Explosion;

        static const unsigned int figuresCountLimit = 128;

        SceneObject(void) = default;

        Figure* CreateFigure(const FigureParams& _params);
        void DestroyFigure(Figure* _figure);
        inline Figure* const* GetFigures() const;
        inline unsigned int EnumFigures() const;

        void SetDynamic(bool _dynamic);
        bool IsDynamic() const;
        void ApplyLinearImpulse(const Vec2f& _dir, const Vec2f& _pos);
        void SetLinearVelocity(const Vec2f& _vel);
        Vec2f GetLinearVelocity() const;
        void SetAngularVelocity(float _vel);
        float GetAngularVelocity() const;
        void SetFixedRotation(bool _fixed);
        void SetActive(bool _active);
        Vec2f GetWorldPoint(const Vec2f& _local_point) const;
        Vec2f GetMassCenter() const;

        void SetPos(const Vec3f _pos);
        inline Animator<Vec2f>& GetPosXY();
        inline Animator<float>& GetPosZ();
        inline const Vec3f& GetPos() const;
        inline Animator<float>& GetAngle();

        friend Logger& operator<<(Logger& _logger,
                                   const SceneObject& _object);

        void DumpGeometry(SceneObjectGeometry* _geometry) const;

        inline const std::vector<unsigned int>& GetDestructionGraph() const;

        inline void
        AddDestroyHandler(const std::function<void(SceneObject*)>& _handler);
        inline void
        AddFigureDestroyHandler(const std::function<void(Figure*)>& _handler);
        inline void AddContactBeginHandler(
            const std::function<void(Figure*, Figure*)>& _handler);
        inline void AddContactEndHandler(
            const std::function<void(Figure*, Figure*)>& _handler);

    protected:
    private:
        b2Body* body = nullptr;

        //////////////////////////////////////////////////////
        /// values, changed only be owning system (Scene) ///

        /// if flag is true, object will be destoyed with next Scene::Step()
        /// invokation
        bool dead = false;

        //////////////////////////////////////////////
        /// figures factory //////////////////////////

        Figure* figures[figuresCountLimit];
        unsigned int figuresCount = 0;

        /// world space postition in physics world
        Vec3f pos = 0;
        Animator<Vec2f> posXYAnimator = pos;
        Animator<float> posZAnimator = pos.z;

        /// rotation angle in radians
        float angle = 0;
        Animator<float> angleAnimator = angle;

        float lifeTime = 0.0f;

        std::map<const Figure*, const Figure*> currentContacts;

        std::vector<unsigned int> destructionGraph;

        std::vector<std::function<void(SceneObject*)> > destroyHandlers;
        std::vector<std::function<void(Figure*)> > figureDestroyHandlers;
        std::vector<std::function<void(Figure*, Figure*)> >
        contactBeginHandlers;
        std::vector<std::function<void(Figure*, Figure*)> >
        contactEndHandlers;
    };

    inline Figure* const* SceneObject::GetFigures() const { return figures; }

    inline unsigned int SceneObject::EnumFigures() const {
        return figuresCount;
    }

    inline Animator<Vec2f>& SceneObject::GetPosXY() { return posXYAnimator; }

    inline Animator<float>& SceneObject::GetPosZ() { return posZAnimator; }

    inline const Vec3f& SceneObject::GetPos() const { return pos; }

    inline Animator<float>& SceneObject::GetAngle() { return angleAnimator; }

    inline const std::vector<unsigned int>&
    SceneObject::GetDestructionGraph() const {
        return destructionGraph;
    }

    inline void SceneObject::AddDestroyHandler(
        const std::function<void(SceneObject*)>& _handler) {
        destroyHandlers.push_back(_handler);
    }

    inline void SceneObject::AddFigureDestroyHandler(
        const std::function<void(Figure*)>& _handler) {
        figureDestroyHandlers.push_back(_handler);
    }

    inline void SceneObject::AddContactBeginHandler(
        const std::function<void(Figure*, Figure*)>& _handler) {
        contactBeginHandlers.push_back(_handler);
    }

    inline void SceneObject::AddContactEndHandler(
        const std::function<void(Figure*, Figure*)>& _handler) {
        contactEndHandlers.push_back(_handler);
    }

} // namespace drash

#endif // SCENEOBJECT_H
