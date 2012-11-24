#include "test.h"

#include "ctestapp1.h"
#include "ctestapp2.h"
#include "ctestapp3-drashtest1.h"
#include "ctestapp4.h"

namespace drash
{

CApp *StartApp( const char *_name )
{
    if ( strcmp(_name, "test_basic" ) == 0 ) return new CApp;
    if ( strcmp(_name, "test1" ) == 0 ) return new CTestApp1();
    if ( strcmp(_name, "test2") == 0) return new CTestApp2();
    if ( strcmp(_name, "test3") == 0) return new CTestApp3();
    if ( strcmp(_name, "test4") == 0) return new CTestApp4();
    return NULL;
}

}// namespace drash
