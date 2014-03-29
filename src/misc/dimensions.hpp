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

#ifndef DIMENSIONS_HPP
#define DIMENSIONS_HPP

namespace drash {

    // TODO extend with more complex dimensions definition (maby by polygons)
    // dimensions definition using object radius
    class Dimensions {
    public:
        Dimensions() = default;
        Dimensions(float newX, float newY, float newRad);

        inline bool Intersect(const Dimensions & another) const;

    protected:
    private:
        float x = 0;
        float y = 0;
        float rad = 0;
    };

    inline bool Dimensions::Intersect(const Dimensions & another) const {
        float deltax = x - another.x;
        float deltay = y - another.y;
        float rads = rad + another.rad;
        return (deltax * deltax + deltay * deltay < rads * rads);
    }
}

#endif// DIMENSIONS_HPP
