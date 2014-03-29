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
#ifndef CSCENEOBJECT_H
#define CSCENEOBJECT_H

#include <functional>
#include <vector>
#include <map>
#include "../misc/animator.h"
#include "../misc/vec3.h"
#include "../misc/objectfactory.h"

class b2Body;

namespace drash {

    class CScene;
    class CFigureParams;
    class CFigure;
    class CExplosionParams;

    class CSceneObjectGeometry final {
      public:
        std::vector<CFigureParams> figures;
        std::vector<unsigned int> destructionGraph;

        void ComputeDestructionGraph(const float _accuracy);

      protected:
      private:
    };

    class CSceneObjectParams final {
      public:
        bool dynamic = true;
        CVec3f pos;
        float angle = 0;
        bool fixedRotation = false;
    };

    class CSceneObject final
        : public CObjectFactory<CSceneObject>::CFactoryProduct {
      public:
        friend class CScene;
        friend class CExplosion;

        static const unsigned int figuresCountLimit = 128;

        CSceneObject(void) = default;

        CFigure* CreateFigure(const CFigureParams& _params);
        void DestroyFigure(CFigure* _figure);
        inline CFigure* const* GetFigures() const;
        inline unsigned int EnumFigures() const;

        void SetDynamic(bool _dynamic);
        bool IsDynamic() const;
        void ApplyLinearImpulse(const CVec2f& _dir, const CVec2f& _pos);
        void SetLinearVelocity(const CVec2f& _vel);
        CVec2f GetLinearVelocity() const;
        void SetAngularVelocity(float _vel);
        float GetAngularVelocity() const;
        void SetFixedRotation(bool _fixed);
        void SetActive(bool _active);
        CVec2f GetWorldPoint(const CVec2f& _local_point) const;
        CVec2f GetMassCenter() const;

        void SetPos(const CVec3f _pos);
        inline CAnimator<CVec2f>& GetPosXY();
        inline CAnimator<float>& GetPosZ();
        inline const CVec3f& GetPos() const;
        inline CAnimator<float>& GetAngle();

        friend CLogger& operator<<(CLogger& _logger,
                                   const CSceneObject& _object);

        void DumpGeometry(CSceneObjectGeometry* _geometry) const;

        inline const std::vector<unsigned int>& GetDestructionGraph() const;

        inline void
        AddDestroyHandler(const std::function<void(CSceneObject*)>& _handler);
        inline void
        AddFigureDestroyHandler(const std::function<void(CFigure*)>& _handler);
        inline void AddContactBeginHandler(
            const std::function<void(CFigure*, CFigure*)>& _handler);
        inline void AddContactEndHandler(
            const std::function<void(CFigure*, CFigure*)>& _handler);

      protected:
      private:
        b2Body* body = nullptr;

        //////////////////////////////////////////////////////
        /// values, changed only be owning system (CScene) ///

        /// if flag is true, object will be destoyed with next CScene::Step()
        /// invokation
        bool dead = false;

        //////////////////////////////////////////////
        /// figures factory //////////////////////////

        CFigure* figures[figuresCountLimit];
        unsigned int figuresCount = 0;

        /// world space postition in physics world
        CVec3f pos = 0;
        CAnimator<CVec2f> posXYAnimator = pos;
        CAnimator<float> posZAnimator = pos.z;

        /// rotation angle in radians
        float angle = 0;
        CAnimator<float> angleAnimator = angle;

        float lifeTime = 0.0f;

        std::map<const CFigure*, const CFigure*> currentContacts;

        std::vector<unsigned int> destructionGraph;

        std::vector<std::function<void(CSceneObject*)> > destroyHandlers;
        std::vector<std::function<void(CFigure*)> > figureDestroyHandlers;
        std::vector<std::function<void(CFigure*, CFigure*)> >
        contactBeginHandlers;
        std::vector<std::function<void(CFigure*, CFigure*)> >
        contactEndHandlers;
    };

    inline CFigure* const* CSceneObject::GetFigures() const { return figures; }

    inline unsigned int CSceneObject::EnumFigures() const {
        return figuresCount;
    }

    inline CAnimator<CVec2f>& CSceneObject::GetPosXY() {
        return posXYAnimator;
    }

    inline CAnimator<float>& CSceneObject::GetPosZ() { return posZAnimator; }

    inline const CVec3f& CSceneObject::GetPos() const { return pos; }

    inline CAnimator<float>& CSceneObject::GetAngle() { return angleAnimator; }

    inline const std::vector<unsigned int>&
    CSceneObject::GetDestructionGraph() const {
        return destructionGraph;
    }

    inline void CSceneObject::AddDestroyHandler(
        const std::function<void(CSceneObject*)>& _handler) {
        destroyHandlers.push_back(_handler);
    }

    inline void CSceneObject::AddFigureDestroyHandler(
        const std::function<void(CFigure*)>& _handler) {
        figureDestroyHandlers.push_back(_handler);
    }

    inline void CSceneObject::AddContactBeginHandler(
        const std::function<void(CFigure*, CFigure*)>& _handler) {
        contactBeginHandlers.push_back(_handler);
    }

    inline void CSceneObject::AddContactEndHandler(
        const std::function<void(CFigure*, CFigure*)>& _handler) {
        contactEndHandlers.push_back(_handler);
    }

} // namespace drash

#endif // CSCENEOBJECT_H
