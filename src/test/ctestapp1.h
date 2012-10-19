#pragma once
#ifndef CTESTAPP1_H
#define CTESTAPP1_H

#include "ctestapp.h"

namespace drash
{

class CTestApp1 : public CTestApp
{
public:
    CTestApp1();

    virtual bool Init();
    virtual void Update();
};

} // namespace drash

#endif // CTESTAPP1_H
