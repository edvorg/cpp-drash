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

template<class T, T (*DEF_VAL_FUNC) (unsigned int _comp_index)>
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
