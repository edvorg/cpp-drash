#include "figure.h"

namespace drash
{

CFigureParams::CFigureParams():
    mFriction(1),
    mRestitution(0),
    mMass(1),
    mVertices()
{
}

bool CFigure::GetVertices(CFigure::b2Vec2ConstPtr *_arr_ptr) const
{
    if (mFixture == nullptr ||
        mFixture->GetShape() == nullptr ||
        mFixture->GetShape()->GetType() != b2Shape::e_polygon ||
        reinterpret_cast<b2PolygonShape*>(mFixture->GetShape())->GetVertexCount() <= 0)
    {
        *_arr_ptr = nullptr;
        return false;
    }

    *_arr_ptr = &(reinterpret_cast<b2PolygonShape*>(mFixture->GetShape())->GetVertex(0));
    return true;
}

unsigned int CFigure::EnumVertices() const
{
    if (mFixture == nullptr ||
        mFixture->GetShape() == nullptr ||
        mFixture->GetShape()->GetType() == b2Shape::e_polygon)
    {
        return 0;
    }

    return reinterpret_cast<b2PolygonShape*>(mFixture->GetShape())->GetVertexCount();
}

CInterval &CFigure::GetZ()
{
    return mZ;
}

const CInterval &CFigure::GetZ() const
{
    return mZ;
}

CFigure::CFigure()
{
}

}// namespace drash
