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

#ifndef DRASH_COLOR3_H
#define DRASH_COLOR3_H

namespace drash {

    constexpr float ColorDefValFloat(unsigned int _comp_index) {
        return _comp_index < 3 ? 0.0f : 1.0f;
    }

    constexpr unsigned char ColorDefValUnsignedByte(unsigned int _comp_index) {
        return _comp_index < 3 ? 0 : 255;
    }

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    class Color3 {
    public:
        Color3() = default;
        Color3(const Color3& _rgb);
        Color3(const T& _rgb);
        Color3(const T& _r, const T& _g, const T& _b);

        Color3& Set(const T& _r, const T& _g, const T& _b);

        T r = DEF_VAL_FUNC(0);
        T g = DEF_VAL_FUNC(1);
        T b = DEF_VAL_FUNC(2);

    protected:
    private:
    };

    /// some typedefs

    typedef Color3<float, ColorDefValFloat> Color3f;
    typedef Color3<unsigned char, ColorDefValUnsignedByte> Color3ub;

    /// Color3 implementation

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    Color3<T, DEF_VAL_FUNC>::Color3(const Color3& _rgb)
        : r(_rgb.r), g(_rgb.g), b(_rgb.b) {}

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    Color3<T, DEF_VAL_FUNC>::Color3(const T& _rgb)
        : r(_rgb), g(_rgb), b(_rgb) {}

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    Color3<T, DEF_VAL_FUNC>::Color3(const T& _r, const T& _g, const T& _b)
        : r(_r), g(_g), b(_b) {}

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    Color3<T, DEF_VAL_FUNC>&
    Color3<T, DEF_VAL_FUNC>::Set(const T& _r, const T& _g, const T& _b) {
        this->r = _r;
        this->g = _g;
        this->b = _b;
        return *this;
    }

} // namespace drash

#endif // DRASH_COLOR3_H
