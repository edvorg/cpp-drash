#include "explosion.h"

#include <cscene.h>
#include <GL/gl.h>

namespace drash
{

CExplosionParams::CExplosionParams():
    mStregth(1),
    mLifeTime(1),
    mRadius(-1)
{
}

CExplosion::CExplosion():
    CSceneObject(),
    mParams(),
    mTime(0)
{
}

bool CExplosion::Init(const GeometryT &_geometry, const ParamsT &_params )
{
    if ( !CSceneObject::Init(_geometry, _params) )
    {
        return false;
    }

    mParams = _params;

    SetDynamic(false);

    return true;
}

void CExplosion::Step(double _dt)
{
    mTime += _dt;

    if (mTime > mParams.mLifeTime)
    {
        GetScene()->DestroyObject(this);
    }

    for (unsigned int i=0; i<GetScene()->EnumObjects(); i++)
    {
        if (mParams.mRadius > 0.0f)
        {
            CVec2 dist = GetScene()->GetObjects()[i]->GetBody()->GetWorldCenter();
            dist -= this->GetBody()->GetWorldCenter();

            if (dist.LengthSquared() <= mParams.mRadius * mParams.mRadius)
            {
                GetScene()->GetObjects()[i]->OnBoom(mParams);
            }
        }
        else
        {
            GetScene()->GetObjects()[i]->OnBoom(mParams);
        }
    }
}

void CExplosion::DrawDebug() const
{
    float scale = (mParams.mLifeTime - mTime) / mParams.mLifeTime;

    glMatrixMode(GL_MODELVIEW);
    glScalef(scale, scale, scale);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);
    glBegin(GL_TRIANGLE_STRIP);
    static const unsigned int segments = 16;
    static const double delta = 2.0 * M_PI / segments;
    for (unsigned int i=0; i<segments+1; i++)
    {
        glColor4f(0, 1, 0, 0.5);
        glVertex2f(0, 0);
        glColor4f(0, 1, 0, 0.5);
        glVertex2f(mParams.mRadius * cos(i*delta), mParams.mRadius * sin(i*delta));
    }
    glEnd();
    glBegin(GL_LINES);
    glColor4f(1, 0, 0, 0.5);
    glVertex2f(-mParams.mRadius, 0);
    glColor4f(1, 0, 0, 0.5);
    glVertex2f(mParams.mRadius, 0);
    glColor4f(1, 0, 0, 0.5);
    glVertex2f(0, -mParams.mRadius);
    glColor4f(1, 0, 0, 0.5);
    glVertex2f(0, mParams.mRadius);
    glEnd();
    glEnable(GL_DEPTH_TEST);

    glDisable(GL_BLEND);
}

const CExplosionParams &CExplosion::GetParams() const
{
    return mParams;
}

float CExplosion::GetTime() const
{
    return mTime;
}

}// namespace drash

