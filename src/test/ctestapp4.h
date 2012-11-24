#ifndef CTESTAPP4_H
#define CTESTAPP4_H

#include "ctestapp.h"
#include "../diag/ctimer.h"

namespace drash
{

class CTestApp4 : public CTestApp
{
public:
    virtual bool Init() override;
    virtual void Step(double _dt) override;

private:
    CTimer mTimer;
};

}// namespace drash

#endif // CTESTAPP4_H
