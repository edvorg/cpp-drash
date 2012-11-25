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
#include "../misc/math.h"
#include "../misc/cvec2.h"

namespace drash
{

class CFigureParams
{
public:
    CFigureParams();

    float mFriction;
    float mRestitution;
    float mMass;

    /// if size of mVertices is NULL, creates a box with (1, 1) dimentions
    std::vector<CVec2> mVertices;

    float mDepth = 1;
    float mZ = 0;
};

class CFigure
{
public:
    friend class CSceneObject;

    typedef const b2Vec2 *b2Vec2ConstPtr;

    bool GetVertices(b2Vec2ConstPtr *_arr_ptr) const;
    unsigned int EnumVertices() const;

    inline float GetZ() const;
    inline void SetZ(float _z);
    inline float GetDepth() const;
    inline void SetDepth(float _depth);

    inline bool TestPoint(const CVec2 &_point);

protected:
private:
    CFigure();

    b2Fixture *mFixture = nullptr;
    float mZ = 0;
    float mDepth = 1;
    float mMass = 1;
    unsigned int mInternalId = 0;
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

inline bool CFigure::TestPoint(const CVec2 &_point)
{
    return mFixture->TestPoint(_point);
}

}// namespace drash

#endif // FIGURE_H
