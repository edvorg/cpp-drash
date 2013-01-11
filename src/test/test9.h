#ifndef DRASH_TEST_TEST9_H
#define DRASH_TEST_TEST9_H

#include "../app/app.h"

namespace drash
{

namespace test
{

class CTest9 : public CApp
{
public:
    CTest9() = default;

private:
    virtual bool Init() override;
    bool InitCamera();
    bool InitLights();

    greng::CPointLight mLight1;
};

} // namespace test

} // namespace drash

#endif // DRASH_TEST_TEST9_H
