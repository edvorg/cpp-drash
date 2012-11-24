#pragma once
#ifndef CTESTAPP1_H
#define CTESTAPP1_H

#include "ctestapp.h"

namespace drash
{

class CTestApp1 : public CTestApp
{
public:
    virtual bool Init() override;
    virtual void Step(double _dt) override;

private:
    double mTime = 0;
};

} // namespace drash

#endif // CTESTAPP1_H
