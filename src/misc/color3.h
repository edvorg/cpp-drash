#ifndef DRASH_COLOR3_H
#define DRASH_COLOR3_H

namespace drash
{

constexpr float CColorDefValFloat()
{
    return 0.0f;
}

constexpr unsigned char CColorDefValUnsignedByte()
{
    return 0;
}

template<class T, T (*DEF_VAL_FUNC) ()>
class CColor3
{
public:
    CColor3() = default;
    CColor3(const CColor3 &_rgb);
    CColor3(const T &_rgb);
    CColor3(const T &_r, const T &_g, const T &_b);

    CColor3 &Set(const T &_r, const T &_g, const T &_b);

    T mR = DEF_VAL_FUNC();
    T mG = DEF_VAL_FUNC();
    T mB = DEF_VAL_FUNC();
protected:
private:
};

/// some typedefs

typedef CColor3<float, CColorDefValFloat> CColor3f;
typedef CColor3<unsigned char, CColorDefValUnsignedByte> CColor3ub;

/// CColor3 implementation

template<class T, T (*DEF_VAL_FUNC) ()>
CColor3<T, DEF_VAL_FUNC>::CColor3(const CColor3 &_rgb):
    mR(_rgb.mR),
    mG(_rgb.mG),
    mB(_rgb.mB)
{
}

template<class T, T (*DEF_VAL_FUNC) ()>
CColor3<T, DEF_VAL_FUNC>::CColor3(const T &_rgb):
    mR(_rgb),
    mG(_rgb),
    mB(_rgb)
{
}

template<class T, T (*DEF_VAL_FUNC) ()>
CColor3<T, DEF_VAL_FUNC>::CColor3(const T &_r, const T &_g, const T &_b):
    mR(_r),
    mG(_g),
    mB(_b)
{
}

template<class T, T (*DEF_VAL_FUNC) ()>
CColor3<T, DEF_VAL_FUNC> &CColor3<T, DEF_VAL_FUNC>::Set(const T &_r, const T &_g, const T &_b)
{
    this->mR = _r;
    this->mG = _g;
    this->mB = _b;
    return *this;
}

} // namespace drash

#endif // DRASH_COLOR3_H
