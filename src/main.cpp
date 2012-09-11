#include <cstdlib>
#include <ctime>

#include "capp.h"
#include "diag/clogger.h"
using namespace drash;

int main( int _argc, char *_argv[] )
{
    srand( time(NULL) );

    CApp app;
    CAppParams params;
    params.SetCommandLine(_argc,_argv);

    if ( app.Init(params) == true )
    {
        app.Run();
        app.Release();
    }

    return 0;
}
