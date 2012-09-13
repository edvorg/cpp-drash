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
    virtual ~CTestApp3();

    virtual bool Init(CScene *_scene, CCamera *_camera );
    virtual void Release();
    virtual void Update();
    virtual void Render();

private:
};

} // namespace drash

#endif // CTESTAPP3DRASHTEST_H
