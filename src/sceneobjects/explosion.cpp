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
    glPointSize(8);
    glBegin(GL_POINTS);
    glColor3f(1, 0, 0);
    glVertex2f(0, 0);
    glEnd();
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

