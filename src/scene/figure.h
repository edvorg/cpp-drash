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

#ifndef FIGURE_H
#define FIGURE_H

#include <functional>
#include <vector>
#include "../misc/vec2.h"

class b2Fixture;

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
    friend class CScene;
    friend class CSceneObject;

    CSceneObject *GetSceneObject() const;

    void SetVertices(const CVec2f *_vertices, unsigned int _count);
    const CVec2f *GetVertices() const;
    unsigned int EnumVertices() const;

    inline float GetZ() const;
    inline void SetZ(float _z);
    inline float GetDepth() const;
    inline void SetDepth(float _depth);
    float GetFriction()const;
    friend CLogger &operator <<(CLogger &_logger, const CFigure &_figure);

    bool TestPoint(const CVec2f &_xy) const;

    inline void AddDestroyHandler(const std::function<void (CFigure *)> & _handler);

protected:
private:
    CFigure() = default;

    b2Fixture *mFixture = nullptr;
    float mZ = 0;
    float mDepth = 1;
    float mMass = 1;
    int mInternalId = -1;
    bool mDead = false;

    std::vector<std::function<void (CFigure *)>> mDestroyHandlers;
};

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

inline void CFigure::AddDestroyHandler(const std::function<void (CFigure *)> & _handler)
{
    mDestroyHandlers.push_back(_handler);
}

}// namespace drash

#endif // FIGURE_H
