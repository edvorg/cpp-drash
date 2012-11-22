#include "debugdrawsystem.h"

#include <GL/gl.h>

using drash::CDebugDrawSystem;

void CDebugDrawSystem::Draw(const drash::CCamera &_camera) const
{
    const drash::CScene::ObjectsT &objects = GetScene()->GetObjects();
    unsigned int count = GetScene()->EnumObjects();

    for (unsigned int i=0; i<count; i++)
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(-_camera.mPos.Get().x,
                     -_camera.mPos.Get().y,
                     -_camera.m_ZoomMax + _camera.GetZoom());
        glTranslatef(objects[i]->GetBody()->GetWorldCenter().x,
                     objects[i]->GetBody()->GetWorldCenter().y,
                     0);
        glRotatef( 180.0f / M_PI * objects[i]->GetBody()->GetAngle(), 0, 0, 1 );

        objects[i]->DrawDebug();
    }
}
