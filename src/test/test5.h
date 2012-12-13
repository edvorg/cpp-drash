#ifndef DRASH_TEST_TEST5_H
#define DRASH_TEST_TEST5_H

#include "../app/app.h"

namespace drash
{

namespace test
{

class CTest5 : public drash::CApp
{
public:
    CTest5() = default;
    virtual bool Init();
};

} // namespace test
} // namespace drash

#endif // DRASH_TEST_TEST5_H
