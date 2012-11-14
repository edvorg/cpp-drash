#ifndef CTESTAPP4_H
#define CTESTAPP4_H

#include "ctestapp.h"
#include "../diag/ctimer.h"

namespace drash
{

class CTestApp4 : public CTestApp
{
public:
    CTestApp4();
    virtual bool Init();
    virtual void Update();

private:
    CTimer mTimer;
};

}// namespace drash

#endif // CTESTAPP4_H
