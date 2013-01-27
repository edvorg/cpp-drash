#ifndef DRASH_TEST_TEST10_H
#define DRASH_TEST_TEST10_H

#include "../app/app.h"
#include "../greng/pointlight.h"
#include "../greng/spotlight.h"

namespace drash
{

namespace test
{

class CTest10 : public CApp
{
public:
    CTest10() = default;

protected:
private:
    virtual bool Init() override;
    virtual void Step(double _dt) override;
    virtual void Render() override;
    bool InitCamera();
    bool InitObjects();
    bool InitLights();
    bool InitProcessors();

    greng::CCamera *mCamera = nullptr;
    greng::CPointLight mLightPoint1;
    greng::CSpotLight mSpotLight;
    CSceneObject *mLamp1Object = nullptr;
    CSceneObject *mWallTopObject = nullptr;
};

} // namespace test

} // namespace drash

#endif // DRASH_TEST_TEST10_H
