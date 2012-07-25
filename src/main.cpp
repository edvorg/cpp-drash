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

    CSceneObject* obj1 = scene.CreateObject(params);
    CSceneObject* obj2 = scene.CreateObject(params);
    CSceneObject* obj3 = scene.CreateObject(params);

    scene.DestroyObject(obj1);

    return 0;
}
