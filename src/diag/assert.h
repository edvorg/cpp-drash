#ifndef ASSERT_H
#define ASSERT_H

#include <cassert>
#include "clogger.h"

namespace drash
{

#if defined(DRASH_DEBUG)
#define DRASH_ASSERT(cond) assert(cond)
#else
#define DRASH_ASSERT(cond) { if (!(cond)) LOG_ERR("Assertion failed: "<<#cond); }
#endif

}// namespace drash

#endif // ASSERT_H
