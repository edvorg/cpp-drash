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

int main( int _argc, char *_argv[] )
{
    srand(time(0));
    CApp app;
    CAppParams params;
    if (!app.init(params)){
        return 0;
    }
    app.Run();
    app.Release();

    return 0;
}
