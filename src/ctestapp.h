#ifndef CTESTAPP_H
#define CTESTAPP_H

#include "sceneobjects.h"

namespace drash
{

class CScene;

class CTestApp
{
public:
    CTestApp();
    virtual ~CTestApp();

    static CTestApp *StartApp( const char *_name );

    virtual bool Init( CScene *_scene, CCamera *_camera );
    virtual void Release();
    virtual void Update();
    virtual void Render();

    CScene *GetScene();
    CCamera *GetCamera();

private:
    CCamera *mCamera;
    CScene* mScene;
};

} // namespace drash

#endif // CTESTAPP_H
