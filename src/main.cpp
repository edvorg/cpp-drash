#include <cstdlib>
#include <ctime>

#include "capp.h"

using namespace drash;

int main( int _argc, char *_argv[] )
{
    srand( time(NULL) );

    CApp app;
    CAppParams params;

    if ( app.Init(params) == true )
    {
        app.Run();
        app.Release();
    }

    return 0;
}
