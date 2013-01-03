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

#ifndef DRASH_COLOR3_H
#define DRASH_COLOR3_H

namespace drash
{

constexpr float CColorDefValFloat(unsigned int _comp_index)
{
    return _comp_index < 3 ? 0.0f: 1.0f;
}

constexpr unsigned char CColorDefValUnsignedByte(unsigned int _comp_index)
{
    return _comp_index < 3 ? 0 : 255;
}

template<class T, constexpr T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
class CColor3
{
public:
    explicit CColor3() = default;
    explicit CColor3(const CColor3 &_rgb);
    explicit CColor3(const T &_rgb);
    explicit CColor3(const T &_r, const T &_g, const T &_b);

    CColor3 &Set(const T &_r, const T &_g, const T &_b);

    T mR = DEF_VAL_FUNC(0);
    T mG = DEF_VAL_FUNC(1);
    T mB = DEF_VAL_FUNC(2);
protected:
private:
};

/// some typedefs

typedef CColor3<float, CColorDefValFloat> CColor3f;
typedef CColor3<unsigned char, CColorDefValUnsignedByte> CColor3ub;

/// CColor3 implementation

template<class T, T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
CColor3<T, DEF_VAL_FUNC>::CColor3(const CColor3 &_rgb):
    mR(_rgb.mR),
    mG(_rgb.mG),
    mB(_rgb.mB)
{
}

template<class T, T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
CColor3<T, DEF_VAL_FUNC>::CColor3(const T &_rgb):
    mR(_rgb),
    mG(_rgb),
    mB(_rgb)
{
}

template<class T, T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
CColor3<T, DEF_VAL_FUNC>::CColor3(const T &_r, const T &_g, const T &_b):
    mR(_r),
    mG(_g),
    mB(_b)
{
}

template<class T, T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
CColor3<T, DEF_VAL_FUNC> &CColor3<T, DEF_VAL_FUNC>::Set(const T &_r, const T &_g, const T &_b)
{
    this->mR = _r;
    this->mG = _g;
    this->mB = _b;
    return *this;
}

} // namespace drash

#endif // DRASH_COLOR3_H
