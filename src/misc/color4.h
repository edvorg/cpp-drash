#ifndef DRASH_COLOR4_H
#define DRASH_COLOR4_H

#include "color3.h"

namespace drash
{

template<class T, T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
class CColor4 : public CColor3<T, DEF_VAL_FUNC>
{
public:
    CColor4() = default;
    CColor4(const CColor4 &_rgba);
    CColor4(const CColor3<T, DEF_VAL_FUNC> &_rgb, const T &_a);
    CColor4(const T &_rgba);
    CColor4(const T &_r, const T &_g, const T &_b);
    CColor4(const T &_r, const T &_g, const T &_b, const T &_a);

    CColor4 &Set(const CColor3<T, DEF_VAL_FUNC> &_rgb, const T &_a);
    CColor4 &Set(const T &_r, const T &_g, const T &_b, const T &_a);

    /// conversion

    inline CColor3<T, DEF_VAL_FUNC> &Col3();
    inline const CColor3<T, DEF_VAL_FUNC> &Col3() const;

    T mA = DEF_VAL_FUNC(3);
protected:
private:
};

/// some typedefs

typedef CColor4<float, CColorDefValFloat> CColor4f;
typedef CColor4<unsigned char, CColorDefValUnsignedByte> CColor4ub;

/// CColor3 implementation

template<class T, T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
CColor4<T, DEF_VAL_FUNC>::CColor4(const CColor4 &_rgba):
    CColor3<T, DEF_VAL_FUNC>(_rgba),
    mA(_rgba.mA)
{
}

template<class T, T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
CColor4<T, DEF_VAL_FUNC>::CColor4(const CColor3<T, DEF_VAL_FUNC> &_rgb, const T &_a):
    CColor3<T, DEF_VAL_FUNC>(_rgb),
    mA(_a)
{
}

template<class T, T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
CColor4<T, DEF_VAL_FUNC>::CColor4(const T &_rgba):
    CColor3<T, DEF_VAL_FUNC>(_rgba),
    mA(_rgba)
{
}

template<class T, T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
CColor4<T, DEF_VAL_FUNC>::CColor4(const T &_r, const T &_g, const T &_b):
    CColor3<T, DEF_VAL_FUNC>(_r, _g, _b)
{
}

template<class T, T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
CColor4<T, DEF_VAL_FUNC>::CColor4(const T &_r, const T &_g, const T &_b, const T &_a):
    CColor3<T, DEF_VAL_FUNC>(_r, _g, _b),
    mA(_a)
{
}

template<class T, T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
CColor4<T, DEF_VAL_FUNC> &CColor4<T, DEF_VAL_FUNC>::Set(const CColor3<T, DEF_VAL_FUNC> &_rgb, const T &_a)
{
    this->mR = _rgb.mR;
    this->mG = _rgb.mG;
    this->mB = _rgb.mB;
    this->mA = _a;
    return *this;
}

template<class T, T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
CColor4<T, DEF_VAL_FUNC> &CColor4<T, DEF_VAL_FUNC>::Set(const T &_r, const T &_g, const T &_b, const T &_a)
{
    this->mR = _r;
    this->mG = _g;
    this->mB = _b;
    this->mA = _a;
    return *this;
}

template<class T, T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
inline CColor3<T, DEF_VAL_FUNC> &CColor4<T, DEF_VAL_FUNC>::Col3()
{
    return *this;
}

template<class T, T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
inline const CColor3<T, DEF_VAL_FUNC> &CColor4<T, DEF_VAL_FUNC>::Col3() const
{
    return *this;
}

} // namespace drash

#endif // DRASH_COLOR4_H
