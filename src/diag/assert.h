#ifndef ASSERT_H
#define ASSERT_H

#include <cassert>

namespace drash
{

#if defined(DRASH_DEBUG)
#define DRASH_ASSERT(cond) assert(cond)
#else
#define DRASH_ASSERT(cond)
#endif

}// namespace drash

#endif // ASSERT_H
