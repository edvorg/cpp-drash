#pragma once
#ifndef CTESTAPP3DRASHTEST_H
#define CTESTAPP3DRASHTEST_H

#include "ctestapp.h"

namespace drash
{

class CTestApp3 : public CTestApp
{
public:
    CTestApp3();

    virtual bool Init();
    virtual void Update();

private:
};

} // namespace drash

#endif // CTESTAPP3DRASHTEST_H
