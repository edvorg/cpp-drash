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

#ifndef FIGURE_H
#define FIGURE_H

#include <vector>
#include "../misc/vec2.h"

namespace drash
{

class CSceneObject;

class CFigureParams
{
public:
    CFigureParams();

    float mFriction = 1;
    float mRestitution = 0;
    float mMass = 1;

    /// if size of mVertices is nullptr, creates a box with (1, 1) dimentions
    std::vector<CVec2f> mVertices;

    float mDepth = 1;
    float mZ = 0;
};

class CFigure
{
public:
    friend class CSceneObject;
    friend class CDebugDrawSystem;

    inline CSceneObject *GetSceneObject() const;

    void SetVertices(const CVec2f *_vertices, unsigned int _count);
    const CVec2f *GetVertices() const;
    unsigned int EnumVertices() const;

    inline float GetZ() const;
    inline void SetZ(float _z);
    inline float GetDepth() const;
    inline void SetDepth(float _depth);
    inline float GetFriction()const;
    friend CLogger &operator <<(CLogger &_logger, const CFigure &_figure);

protected:
private:
    CFigure();

    b2Fixture *mFixture = nullptr;
    float mZ = 0;
    float mDepth = 1;
    float mMass = 1;
    int mInternalId = -1;
    bool mDead = false;
};

inline float CFigure::GetFriction() const
{
    return mFixture->GetFriction();
}

inline CSceneObject *CFigure::GetSceneObject() const
{

    return reinterpret_cast<CSceneObject*>(mFixture->GetBody()->GetUserData());
}

inline float CFigure::GetZ() const
{
    return mZ;
}

inline void CFigure::SetZ(float _z)
{
    mZ = _z;
}

inline float CFigure::GetDepth() const
{
    return mDepth;
}

inline void CFigure::SetDepth(float _depth)
{
    mDepth = _depth;
}

}// namespace drash

#endif // FIGURE_H
