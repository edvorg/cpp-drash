#ifndef DRASH_TEST_TEST4_H
#define DRASH_TEST_TEST4_H

#include "../app/app.h"
#include "../ui/uibutton.h"

namespace drash
{

namespace test
{

class CTest4 : public CApp
{
public:
    CTest4();
    virtual ~CTest4() override {}

    virtual void Release() override;

private:
    CUIButton mTestButton1;
};

} // namespace test
} // namespace drash

#endif // DRASH_TEST_TEST4_H
