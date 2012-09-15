#pragma once
#ifndef CTESTAPP2_H
#define CTESTAPP2_H

#include "ctestapp.h"
namespace drash
{

class CTestApp2 : public CTestApp
{
public:
    CTestApp2();
    virtual ~CTestApp2();

    virtual bool Init( CScene *_scene, CCamera *_camera );
    virtual void Release();
    virtual void Update();
    virtual void Render();
};


}// namespace drash
#endif // CTESTAPP2_H
