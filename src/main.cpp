#include <stdio.h>

#include "cscene.h"
#include "clogger.h"

using namespace drash;

int main()
{
    CScene scene;

    CSceneParams sceneparams;
    sceneparams.mGravity.set(0, -10);
    scene.Init(sceneparams);

    CSceneObjectParams params;

    CSceneObject* obj = scene.CreateObject(params);

    for (;;)
    {
        scene.step(1);
        LOG_ERR("bla bla");
    }

    return 0;
}
