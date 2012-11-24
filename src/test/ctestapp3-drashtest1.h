#pragma once
#ifndef CTESTAPP3DRASHTEST_H
#define CTESTAPP3DRASHTEST_H

#include "ctestapp.h"

namespace drash
{

class CTestApp3 : public CApp
{
public:
    virtual bool Init() override;
    virtual void Step(double _dt) override;

private:
};

} // namespace drash

#endif // CTESTAPP3DRASHTEST_H
