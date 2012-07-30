#include <SDL/SDL.h>
#include <GL/gl.h>
#include <Box2D/Box2D.h>
#include <math.h>
#include "cdebugrenderer.h"
#include "clogger.h"
#include "cscene.h"
#include "cobjectsolidbody.h"
#include "cobjectcirclebody.h"
#include <time.h>
#include <sys/time.h>
#include "capp.h"

using namespace drash;

bool Init( CScene& _scene );
void Run( CScene& _scene );
void Render( CScene& _scene );
void Release( CScene &_scene );

int main( int _argc, char *_argv[] )
{

    CApp app;
    CAppParams params;
    if (!app.init(params)){
        return 0;

    }
    app.Run();
    app.Release();

    return 0;
}
