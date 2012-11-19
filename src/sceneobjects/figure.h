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

    CInterval mLayers;
};

class CFigure
{
public:
    friend class CSceneObject;

    typedef const b2Vec2 *b2Vec2ConstPtr;

    bool Init(const CFigureParams &_params);

    bool GetVertices(b2Vec2ConstPtr *_arr_ptr) const;
    unsigned int EnumVertices() const;

    CInterval &GetZ();
    const CInterval &GetZ() const;

protected:
private:
    b2Fixture *mFixture = nullptr;
    CInterval mZ = CInterval(0,0);
    float mMass = 1;
};

}// namespace drash

#endif // FIGURE_H
