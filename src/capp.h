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

    /// if already initialized or init successfull, returns true
    bool Init( const CAppParams & _params );

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
    CCamera *mCamera;
};
}
#endif // CAPP_H
