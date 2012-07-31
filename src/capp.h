#ifndef CAPP_H
#define CAPP_H

#include "cscene.h"
#include "ctimer.h"

namespace drash
{

class CAppParams
{
public:
    CAppParams();
};

class CApp
{
public:
    CApp();

    bool init( const CAppParams & _params );
    void Release();
    void Run();


private:
    void Update();
    void Render();
    bool mInitialized;

    static const int mWidth = 800;
    static const int mHeight = 600;
    CScene mScene;
    CTimer mTimer;
    CDebugRenderer mDebugRenderer;
    CObjectCamera *mCamera;
};
}
#endif // CAPP_H
