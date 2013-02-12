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
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#pragma once
#ifndef CSCENEOBJECT_H
#define CSCENEOBJECT_H

class b2Body;

namespace drash
{

class CScene;
class CFigureParams;
class CFigure;
class CExplosionParams;

class CSceneObjectGeometry final
{
public:
    std::vector<CFigureParams> mFigures;
    std::vector<unsigned int> mDestructionGraph;

    void ComputeDestructionGraph(const float _accuracy);

protected:
private:
};

class CSceneObjectParams final
{
public:
    bool mDynamic = true;
    CVec3f mPos;
    float mAngle = 0;
    bool mFixedRotation = false;
};

class CSceneObject final : public CObjectFactory<CSceneObject>::CFactoryProduct
{
public:
    friend class CPhysObserver;
    friend class CScene;
    friend class CExplosion;

    static const unsigned int mFiguresCountLimit = 128;

    CSceneObject(void) = default;

    CFigure *CreateFigure(const CFigureParams &_params);
    void DestroyFigure(CFigure *_figure);
    inline CFigure * const *GetFigures() const;
    inline unsigned int EnumFigures() const;

    void SetDynamic(bool _dynamic);
    bool IsDynamic() const;
    void ApplyLinearImpulse( const CVec2f &_dir, const CVec2f &_pos );
    void SetLinearVelocity(const CVec2f &_vel);
    CVec2f GetLinearVelocity() const;
    void SetAngularVelocity(float _vel);
    float GetAngularVelocity() const;
    void SetFixedRotation( bool _fixed );
    void SetActive(bool _active);
    CVec2f GetWorldPoint(const CVec2f &_local_point) const;
    CVec2f GetMassCenter() const;

    void SetPos(const CVec3f _pos);
    inline CAnimator<CVec2f> &GetPosXY();
    inline CAnimator<float> &GetPosZ();
    inline const CVec3f &GetPos() const;
    inline CAnimator<float> &GetAngle();

    friend CLogger &operator <<(CLogger &_logger, const CSceneObject &_object);

    void DumpGeometry(CSceneObjectGeometry *_geometry) const;

    inline const std::vector<unsigned int> & GetDestructionGraph() const;

protected:
private:
    bool Init();
    void Step(double _dt);
    void Release(void);

    void OnContactBegin(const CFigure *_f1, const CFigure *_f2);
    void OnContactPreSolve(const CFigure *, const CFigure *);
    void OnContactEnd(const CFigure *, const CFigure *_f2);

    b2Body* mBody = nullptr;

    //////////////////////////////////////////////////////
    /// values, changed only be owning system (CScene) ///

    /// if flag is true, object will be destoyed with next CScene::Step() invokation
    bool mDead = false;

    //////////////////////////////////////////////
    /// figures factory //////////////////////////

    CFigure *mFigures[mFiguresCountLimit];
    unsigned int mFiguresCount = 0;

    /// world space postition in physics world
    CVec3f mPos = 0;
    CAnimator<CVec2f> mPosXYAnimator = mPos;
    CAnimator<float> mPosZAnimator = mPos.mZ;

    /// rotation angle in radians
    float mAngle = 0;
    CAnimator<float> mAngleAnimator = mAngle;

    float mLifeTime = 0.0f;

    std::map<const CFigure*, const CFigure*> mCurrentContacts;

    std::vector<unsigned int> mDestructionGraph;
};

inline CFigure * const *CSceneObject::GetFigures() const
{
    return mFigures;
}

inline unsigned int CSceneObject::EnumFigures() const
{
    return mFiguresCount;
}

inline CAnimator<CVec2f> &CSceneObject::GetPosXY()
{
    return mPosXYAnimator;
}

inline CAnimator<float> &CSceneObject::GetPosZ()
{
    return mPosZAnimator;
}

inline const CVec3f &CSceneObject::GetPos() const
{
    return mPos;
}

inline CAnimator<float> &CSceneObject::GetAngle()
{
    return mAngleAnimator;
}

inline const std::vector<unsigned int> &CSceneObject::GetDestructionGraph() const
{
    return mDestructionGraph;
}

} // namespace drash

#endif // CSCENEOBJECT_H
