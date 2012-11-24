#ifndef CTESTAPP4_H
#define CTESTAPP4_H

#include "app.h"

namespace drash
{

class CTestApp4 : public CApp
{
public:
    virtual ~CTestApp4() override {}

    virtual bool Init() override;
    virtual void Step(double _dt) override;

private:
    double mTime = 0;
};

}// namespace drash

#endif // CTESTAPP4_H
