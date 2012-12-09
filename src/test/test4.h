#ifndef DRASH_TEST_TEST4_H
#define DRASH_TEST_TEST4_H

#include "../app/app.h"
#include "../ui/uiwidget.h"

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
    CUIWidget mTestWidget1;
};

} // namespace test
} // namespace drash

#endif // DRASH_TEST_TEST4_H
