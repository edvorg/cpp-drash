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

#include "figure.h"
#include "sceneobject.h"
#include <Box2D/Box2D.h>

namespace drash {

    FigureParams::FigureParams() {}

    SceneObject* Figure::GetSceneObject() const {
        return static_cast<SceneObject*>(fixture->GetBody()->GetUserData());
    }

    void Figure::SetVertices(const Vec2f* _vertices, unsigned int _count) {
        if (fixture == nullptr || fixture->GetShape() == nullptr ||
            fixture->GetShape()->GetType() != b2Shape::e_polygon) {
            return;
        }

        static_cast<b2PolygonShape*>(fixture->GetShape())
            ->Set(&Vec2ToB2Vec2(*_vertices), _count);
        fixture->GetBody()->ResetMassData();
    }

    const Vec2f* Figure::GetVertices() const {
        if (fixture == nullptr || fixture->GetShape() == nullptr ||
            fixture->GetShape()->GetType() != b2Shape::e_polygon) {
            return nullptr;
        }

        return &B2Vec2ToCVec2(*static_cast<b2PolygonShape*>(fixture->GetShape())
                                   ->m_vertices);
    }

    unsigned int Figure::EnumVertices() const {
        if (fixture == nullptr || fixture->GetShape() == nullptr ||
            fixture->GetShape()->GetType() != b2Shape::e_polygon) {
            return 0;
        }

        return static_cast<b2PolygonShape*>(fixture->GetShape())
            ->GetVertexCount();
    }

    float Figure::GetFriction() const { return fixture->GetFriction(); }

    Logger& operator<<(Logger& _logger, const Figure& _figure) {
        if (_figure.EnumVertices()) {
            _logger << '{';
            for (unsigned int i = 0; i < _figure.EnumVertices(); i++) {
                _logger << _figure.GetVertices()[i] << ' ';
            }
            _logger << "} depth: " << _figure.depth
                    << " local_z: " << _figure.z;
        }
        return _logger;
    }

    bool Figure::TestPoint(const Vec2f& _xy) const {
        return fixture->TestPoint(Vec2ToB2Vec2(_xy));
    }

} // namespace drash
