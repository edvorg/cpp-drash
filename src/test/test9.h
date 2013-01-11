#ifndef DRASH_TEST_TEST9_H
#define DRASH_TEST_TEST9_H

#include "../app/app.h"
#include "../misc/rotationablepoint.h"

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
    virtual void Step(double _dt) override;
    virtual void Render() override;
    bool InitCamera();
    bool InitLights();
    bool InitRotationablePoint();

    greng::CPointLight mLight1;
    greng::CCamera *mCamera = nullptr;

    CRotationablePoint mPoint1;
};

} // namespace test

} // namespace drash

#endif // DRASH_TEST_TEST9_H
