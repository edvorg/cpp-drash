#ifndef CBOOMPARAMS_H
#define CBOOMPARAMS_H

#include "cvec2.h"

namespace drash{

class CBoomParams{
public:
    CVec2 mPos;
    float mStregth;
    CBoomParams():
        mPos(0.0f),
        mStregth(50.0f)
    {}

};

}// namespace drash

#endif // CBOOMPARAMS_H
