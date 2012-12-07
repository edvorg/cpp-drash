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

#include "figure.h"

#include "csceneobject.h"

namespace drash
{

CFigureParams::CFigureParams()
{
}

const b2Vec2 *CFigure::GetVertices() const
{
    if (mFixture == nullptr ||
        mFixture->GetShape() == nullptr ||
        mFixture->GetShape()->GetType() != b2Shape::e_polygon)
    {
        return nullptr;
    }

    return reinterpret_cast<b2PolygonShape*>(mFixture->GetShape())->m_vertices;
}

unsigned int CFigure::EnumVertices() const
{
    if (mFixture == nullptr ||
        mFixture->GetShape() == nullptr ||
        mFixture->GetShape()->GetType() != b2Shape::e_polygon)
    {
        return 0;
    }

    return reinterpret_cast<b2PolygonShape*>(mFixture->GetShape())->GetVertexCount();
}

CFigure::CFigure()
{
}

}// namespace drash
