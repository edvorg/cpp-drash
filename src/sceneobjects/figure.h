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

}// namespace drash

#endif // FIGURE_H
