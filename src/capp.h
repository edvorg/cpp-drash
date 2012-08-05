#ifndef CAPP_H
#define CAPP_H

#include "cscene.h"
#include "ctimer.h"
#include "ctestapp.h"

#include <vector>
#include <string>

namespace drash
{

class CAppParams
{
public:
    std::vector<std::string> mArgv;
    void SetCommandLine( unsigned int _argc, char *_argv[] );
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
    CTestApp *mTestApp;
};
}
#endif // CAPP_H
