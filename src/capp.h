#ifndef CAPP_H
#define CAPP_H


// TODO: wrap the sdl initialization code in CApp class
// realize CApp::Init(CAppParams& _params, CScene& _scene)
// realize CApp::Run()
// realize CApp::Release()
// realize dummy CAppParams class

#include "cscene.h"
#include <sys/time.h>
namespace drash{

class CAppParams{
public:
    CAppParams(){}
};


class CApp
{
public:
    CApp();

    bool init(const CAppParams & _params);
    void Release();
    void Run();


private:
    void Update();
    void Render();
    bool mInitialized;

    static const int mWidth = 800;
    static const int mHeight = 600;
    CScene mScene;

    timeval mTime;
    long mCurrTime;
    long mPrevTime;
};
}
#endif // CAPP_H
