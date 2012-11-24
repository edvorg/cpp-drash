#pragma once
#ifndef CTESTAPP2_H
#define CTESTAPP2_H

#include "app.h"

namespace drash
{

class CTestApp2 : public CApp
{
public:
    virtual ~CTestApp2() override {};

    virtual bool Init() override;
};


}// namespace drash
#endif // CTESTAPP2_H
