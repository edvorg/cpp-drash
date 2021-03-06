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

#ifndef DRASH_COLOR4_H
#define DRASH_COLOR4_H

#include "color3.h"

namespace drash {

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    class Color4 : public Color3<T, DEF_VAL_FUNC> {
    public:
        explicit Color4() = default;
        Color4(const Color4& _rgba);
        Color4(const Color3<T, DEF_VAL_FUNC>& _rgb, const T& _a);
        Color4(const T& _rgba);
        Color4(const T& _r, const T& _g, const T& _b);
        Color4(const T& _r, const T& _g, const T& _b, const T& _a);

        /// setters

        Color4& Set(const Color3<T, DEF_VAL_FUNC>& _rgb, const T& _a);
        Color4& Set(const T& _r, const T& _g, const T& _b, const T& _a);

        /// operators

        Color4& operator=(const Color4& _rgba);

        /// conversion

        inline Color3<T, DEF_VAL_FUNC>& Col3();
        inline const Color3<T, DEF_VAL_FUNC>& Col3() const;

        T a = DEF_VAL_FUNC(3);

    protected:
    private:
    };

    /// some typedefs

    typedef Color4<float, ColorDefValFloat> Color4f;
    typedef Color4<unsigned char, ColorDefValUnsignedByte> Color4ub;

    /// Color3 implementation

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    Color4<T, DEF_VAL_FUNC>::Color4(const Color4& _rgba)
        : Color3<T, DEF_VAL_FUNC>(_rgba), a(_rgba.a) {}

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    Color4<T, DEF_VAL_FUNC>::Color4(const Color3<T, DEF_VAL_FUNC>& _rgb,
                                    const T& _a)
        : Color3<T, DEF_VAL_FUNC>(_rgb), a(_a) {}

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    Color4<T, DEF_VAL_FUNC>::Color4(const T& _rgba)
        : Color3<T, DEF_VAL_FUNC>(_rgba), a(_rgba) {}

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    Color4<T, DEF_VAL_FUNC>::Color4(const T& _r, const T& _g, const T& _b)
        : Color3<T, DEF_VAL_FUNC>(_r, _g, _b) {}

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    Color4<T, DEF_VAL_FUNC>::Color4(const T& _r, const T& _g, const T& _b,
                                    const T& _a)
        : Color3<T, DEF_VAL_FUNC>(_r, _g, _b), a(_a) {}

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    Color4<T, DEF_VAL_FUNC>&
    Color4<T, DEF_VAL_FUNC>::Set(const Color3<T, DEF_VAL_FUNC>& _rgb,
                                 const T& _a) {
        this->r = _rgb.r;
        this->g = _rgb.g;
        this->b = _rgb.b;
        this->a = _a;
        return *this;
    }

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    Color4<T, DEF_VAL_FUNC>&
    Color4<T, DEF_VAL_FUNC>::Set(const T& _r, const T& _g, const T& _b,
                                 const T& _a) {
        this->r = _r;
        this->g = _g;
        this->b = _b;
        this->a = _a;
        return *this;
    }

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    Color4<T, DEF_VAL_FUNC>& Color4<T, DEF_VAL_FUNC>::
    operator=(const Color4& _rgba) {
        this->r = _rgba.r;
        this->g = _rgba.g;
        this->b = _rgba.b;
        this->a = _rgba.a;
        return *this;
    }

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    inline Color3<T, DEF_VAL_FUNC>& Color4<T, DEF_VAL_FUNC>::Col3() {
        return *this;
    }

    template <class T, T (*DEF_VAL_FUNC)(unsigned int _comp_index)>
    inline const Color3<T, DEF_VAL_FUNC>&
    Color4<T, DEF_VAL_FUNC>::Col3() const {
        return *this;
    }

} // namespace drash

#endif // DRASH_COLOR4_H
