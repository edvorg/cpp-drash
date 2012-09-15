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
    virtual ~CTestApp1();

    virtual bool Init( CScene *_scene, CCamera *_camera );
    virtual void Release();
    virtual void Update();
    virtual void Render();
};

} // namespace drash

#endif // CTESTAPP1_H
