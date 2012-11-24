#ifndef CTESTAPP4_H
#define CTESTAPP4_H

#include "ctestapp.h"

namespace drash
{

class CTestApp4 : public CApp
{
public:
    virtual bool Init() override;
    virtual void Step(double _dt) override;

private:
    double mTime = 0;
};

}// namespace drash

#endif // CTESTAPP4_H
