// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012 Edward Knyshov, Yuriy Shatilin.

This file is part of the drash GPL Source Code (drash Source Code).

drash Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

drash Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with drash Source Code.  If not, see <http://www.gnu.org/licenses/>.

*/
// DRASH_LICENSE_END

#include "test1.h"

#include "../scene.h"
#include "app/appeventprocessor.h"
#include "../debugdrawsystem/camera.h"
#include "../sceneobjects/figure.h"
#include "../sceneobjects/sceneobject.h"

#include <sstream>

#include "../misc/plane.h"

namespace drash
{

namespace test
{

bool CTest1::Init()
{
    if ( CApp::Init() == false )
    {
        return false;
    }

    if (GetTemplateSystem().Load() == false)
    {
        return false;
    }

    GetDebugDrawSystem().GetActiveCam()->GetPos().Set(CVec3f(0, 0, 100));

    SetProcessors();

    return true;
}

void CTest1::Step(double _dt)
{
    CApp::Step(_dt);


    if (mCurrentFigure != nullptr && mCurrentObject != nullptr)
    {
        mCenter.Set(mCurrentFigure->GetVertices()[0], mCurrentObject->GetPos().Get().mZ + mCurrentFigure->GetZ());

        for (unsigned int i = 1; i < mCurrentFigure->EnumVertices(); i++)
        {
            mCenter.Vec2() += mCurrentFigure->GetVertices()[i];
        }

        mCenter.Vec2() /= CVec2f(mCurrentFigure->EnumVertices());

        mX = mCenter;
        mY = mCenter;
        mZ = mCenter;
        mX.mX += 10;
        mY.mY += 10;
        mZ.mZ += 10;

        CPlane xz(PlaneXZ);
        xz.SetPoint(mCenter);
        CPlane xy(PlaneXY);
        xy.SetPoint(mCenter);

        CVec3f r1;
        GetDebugDrawSystem().CastRay(GetCursorPos(), xz, r1);
        CVec3f r2;
        GetDebugDrawSystem().CastRay(GetCursorPos(), xy, r2);

        CVec2f dstz = r1;
        dstz -= mCenter.Vec2();
        CVec2f dstx(r1.mZ, r1.mY);
        dstx -= CVec2f(mCenter.mZ, mCenter.mY);
        CVec2f dsty(r2.mZ, r2.mX);
        dsty -= CVec2f(mCenter.mZ, mCenter.mX);

        mAxisOver = 0;
        mAxisDrawK.Set(1, 1, 1);

        if (dstz.Length() < 3)
        {
            mAxisDrawK.mZ *= 0.5;
            mAxisOver = 3;
        }
        else if (dstx.Length() < 3)
        {
            mAxisDrawK.mX *= 0.5;
            mAxisOver = 1;
        }
        else if (dsty.Length() < 3)
        {
            mAxisDrawK.mY *= 0.5;
            mAxisOver = 2;
        }
    }
}

void CTest1::Render()
{
    CApp::Render();

    if (mVertices.size())
    {
        for (int i = 0; i < (int)mVertices.size() - 1; i++)
        {
            GetDebugDrawSystem().DrawLine(mVertices[i], mVertices[i+1], b2Color(0, 1, 0));
        }
        GetDebugDrawSystem().DrawLine(mVertices[mVertices.size()-1], GetCursorPos(), b2Color(0, 1, 0));
        GetDebugDrawSystem().DrawLine(mVertices[0], GetCursorPos(), b2Color(0, 1, 0));
    }

    if (mCurrentFigure != nullptr && mCurrentObject != nullptr)
    {
        GetDebugDrawSystem().DrawLine(mCenter, mX, b2Color(1 * mAxisDrawK.mX, 0, 0));
        GetDebugDrawSystem().DrawLine(mCenter, mY, b2Color(0, 1 * mAxisDrawK.mY, 0));
        GetDebugDrawSystem().DrawLine(mCenter, mZ, b2Color(0, 0, 1 * mAxisDrawK.mZ));
    }
}

void CTest1::Release()
{
    GetTemplateSystem().Store();

    CApp::Release();
}

void CTest1::SetProcessors()
{
    GetEventSystem().SetMode(std::string("figure_movement"));

    GetEventSystem().SetProcessor("WHUP", CAppEventProcessor(
    [this] ()
    {
        if (mCurrentFigure != nullptr)
        {
            mCurrentFigure->SetDepth(mCurrentFigure->GetDepth() + 1.0f);
        }
    }));

    GetEventSystem().SetProcessor("WHDN", CAppEventProcessor(
    [this] ()
    {
        if (mCurrentFigure != nullptr)
        {
            mCurrentFigure->SetDepth(math::Max<float>(0, mCurrentFigure->GetDepth() - 1.0f));
        }
    }));

    GetEventSystem().SetProcessor("LB", CAppEventProcessor(
    [this] ()
    {
        if (mAxisOver == 0)
        {
            SelectFigure();
        }
        else
        {
            if (mAxisOver == 2)
            {
                CPlane xy(PlaneXY);
                xy.SetPoint(mCenter);
                GetDebugDrawSystem().CastRay(GetCursorPos(), xy, mFigureMoveFirstClick);
                mAxisMoving = mAxisOver;
            }
            else
            {
                CPlane xz(PlaneXZ);
                xz.SetPoint(mCenter);
                GetDebugDrawSystem().CastRay(GetCursorPos(), xz, mFigureMoveFirstClick);
                mAxisMoving = mAxisOver;
            }
        }
    },
    [this] ()
    {
        if (mAxisMoving == 1)
        {
            CPlane xz(PlaneXZ);
            xz.SetPoint(mCenter);
            CVec3f new_pos;
            GetDebugDrawSystem().CastRay(GetCursorPos(), xz, new_pos);

            CVec2f *v = new CVec2f[mCurrentFigure->EnumVertices()];
            for (unsigned int i = 0; i < mCurrentFigure->EnumVertices(); i++)
            {
                v[i].mX = mCurrentFigure->GetVertices()[i].mX + new_pos.mX - mFigureMoveFirstClick.mX;
                v[i].mY = mCurrentFigure->GetVertices()[i].mY;
            }
            mCurrentFigure->SetVertices(v, mCurrentFigure->EnumVertices());
            delete [] v;

            mFigureMoveFirstClick = new_pos;
        }
        else if (mAxisMoving == 2)
        {
            CPlane xy(PlaneXY);
            xy.SetPoint(mCenter);
            CVec3f new_pos;
            GetDebugDrawSystem().CastRay(GetCursorPos(), xy, new_pos);

            CVec2f *v = new CVec2f[mCurrentFigure->EnumVertices()];
            for (unsigned int i = 0; i < mCurrentFigure->EnumVertices(); i++)
            {
                v[i].mX = mCurrentFigure->GetVertices()[i].mX;
                v[i].mY = mCurrentFigure->GetVertices()[i].mY + new_pos.mY - mFigureMoveFirstClick.mY;
            }
            mCurrentFigure->SetVertices(v, mCurrentFigure->EnumVertices());
            delete [] v;

            mFigureMoveFirstClick = new_pos;
        }
        else if (mAxisMoving == 3)
        {
            CPlane xz(PlaneXZ);
            xz.SetPoint(mCenter);
            CVec3f new_pos;
            GetDebugDrawSystem().CastRay(GetCursorPos(), xz, new_pos);

            mCurrentFigure->SetZ(mCurrentFigure->GetZ() + new_pos.mZ - mFigureMoveFirstClick.mZ);

            mFigureMoveFirstClick = new_pos;
        }
    },
    [this] ()
    {
        mAxisMoving = 0;
    }));

    CamViewProcessors();

    ///

    GetEventSystem().SetMode(std::string("editor_mode"));

    GetEventSystem().SetProcessor("C-c", CAppEventProcessor(
    [this] ()
    {
        CreateTemplate();
    }));

    GetEventSystem().SetProcessor("C-d", CAppEventProcessor(
    [this] ()
    {
        DetachCurrentObject();
    }));

    GetEventSystem().SetProcessor("C-LB", CAppEventProcessor(
    [this] ()
    {
        mVertices.push_back(GetCursorPos());
    }));

    GetEventSystem().SetProcessor("LB", CAppEventProcessor(
    [this] ()
    {
        SelectFigure();
    }));

    GetEventSystem().SetProcessor("RB", CAppEventProcessor(
    [this] ()
    {
        CompleteFigure();
    }));

    CamViewProcessors();

    GetEventSystem().SetProcessor("C-q", CAppEventProcessor(
    [this] ()
    {
        this->Quit();
    }));
}

void CTest1::CamViewProcessors()
{
    GetEventSystem().SetProcessor("MB", CAppEventProcessor(
    [this] ()
    {
        mCamRotFirstClick = GetCursorPos();
    },
    [this] ()
    {
        CVec2f new_pos = GetCursorPos();

        CVec2f rot = GetDebugDrawSystem().GetActiveCam()->GetRotation().Get();
        rot.mY -= new_pos.mX - mCamRotFirstClick.mX;
        rot.mX += new_pos.mY - mCamRotFirstClick.mY;

        GetDebugDrawSystem().GetActiveCam()->GetRotation().Set(rot);

        mCamRotFirstClick = new_pos;
    }));

    GetEventSystem().SetProcessor("w", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        GetDebugDrawSystem().GetActiveCam()->Forward(5);
    }));

    GetEventSystem().SetProcessor("a", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        GetDebugDrawSystem().GetActiveCam()->Strafe(5);
    }));

    GetEventSystem().SetProcessor("s", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        GetDebugDrawSystem().GetActiveCam()->Forward(-5);
    }));

    GetEventSystem().SetProcessor("d", CAppEventProcessor(
    [this] () {},
    [this] ()
    {
        GetDebugDrawSystem().GetActiveCam()->Strafe(-5);
    }));

    GetEventSystem().SetProcessor("q", CAppEventProcessor(
    [this] ()
    {
        if (mCurrentTemplate == nullptr)
        {
            mCurrentTemplate = (--GetTemplateSystem().GetSceneObjectTemplates().end())->second;

            if (mCurrentObject != nullptr)
            {
                GetScene().DestroyObject(mCurrentObject);
            }

            CSceneObjectParams p;
            p.mDynamic = false;
            mCurrentObject = GetScene().CreateObject<CSceneObject>(*mCurrentTemplate, p);
            mCurrentObject->SetActive(false);
        }
        else
        {
            auto &t = GetTemplateSystem().GetSceneObjectTemplates();
            for (auto i = t.begin(); i != t.end(); i++)
            {
                if (mCurrentTemplate == i->second)
                {
                    if (i == t.begin())
                    {
                        break;
                    }

                    i--;

                    mCurrentTemplate = i->second;

                    if (mCurrentObject != nullptr)
                    {
                        GetScene().DestroyObject(mCurrentObject);
                    }

                    CSceneObjectParams p;
                    p.mDynamic = false;
                    mCurrentObject = GetScene().CreateObject<CSceneObject>(*mCurrentTemplate, p);
                    mCurrentObject->SetActive(false);

                    break;
                }
            }
        }
    }));

    GetEventSystem().SetProcessor("e", CAppEventProcessor(
    [this] ()
    {
        if (mCurrentTemplate == nullptr)
        {
            mCurrentTemplate = GetTemplateSystem().GetSceneObjectTemplates().begin()->second;

            if (mCurrentObject != nullptr)
            {
                GetScene().DestroyObject(mCurrentObject);
            }

            CSceneObjectParams p;
            p.mDynamic = false;
            mCurrentObject = GetScene().CreateObject<CSceneObject>(*mCurrentTemplate, p);
            mCurrentObject->SetActive(false);
        }
        else
        {
            auto &t = GetTemplateSystem().GetSceneObjectTemplates();
            for (auto i = t.begin(); i != t.end(); i++)
            {
                if (mCurrentTemplate == i->second)
                {
                    i++;

                    if (i == t.end())
                    {
                        break;
                    }

                    mCurrentTemplate = i->second;

                    if (mCurrentObject != nullptr)
                    {
                        GetScene().DestroyObject(mCurrentObject);
                    }

                    CSceneObjectParams p;
                    p.mDynamic = false;
                    mCurrentObject = GetScene().CreateObject<CSceneObject>(*mCurrentTemplate, p);
                    mCurrentObject->SetActive(false);

                    break;
                }
            }
        }
    }));
}

void CTest1::CompleteFigure()
{
    if (mCurrentTemplate != nullptr && mVertices.size() >= 3)
    {        
        mCurrentTemplate->mFigures.resize(mCurrentTemplate->mFigures.size()+1);

        mCurrentTemplate->mFigures.back().mVertices = mVertices;

        for (auto i = mCurrentTemplate->mFigures.back().mVertices.begin();
             i != mCurrentTemplate->mFigures.back().mVertices.end(); i++)
        {
            CPlane plane;
            plane.SetNormal(CVec3f(0, 0, 1));
            plane.SetPoint(CVec3f(0, 0, 0));

            CVec3f pos;

            GetDebugDrawSystem().CastRay(*i, plane, pos);

            *i = pos;
        }

        if (mCurrentObject != nullptr)
        {
            GetScene().DestroyObject(mCurrentObject);
            this->mCurrentObject = nullptr;
        }

        CSceneObjectParams p;
        p.mDynamic = false;
        mCurrentObject = GetScene().CreateObject<CSceneObject>(*mCurrentTemplate, p);
        mCurrentObject->SetActive(false);

        mVertices.clear();
    }
}

void CTest1::CreateTemplate()
{
    static const unsigned max_try_count = 10;
    unsigned int counter = 0;

    do
    {
        std::ostringstream is;
        is<<"new_template_"<<(mTemplateCounter++);
        this->mCurrentTemplate = GetTemplateSystem().CreateSceneObjectTemplate(is.str().c_str());
    }
    while (this->mCurrentTemplate == nullptr && counter < max_try_count);

    if (mCurrentObject != nullptr)
    {
        GetScene().DestroyObject(mCurrentObject);
        this->mCurrentObject = nullptr;
    }
}

void CTest1::DetachCurrentObject()
{
    if (mCurrentObject != nullptr)
    {
        mCurrentObject->SetDynamic(true);
        mCurrentObject->SetActive(true);

        if (mCurrentTemplate != nullptr)
        {
            CSceneObjectParams p;
            p.mDynamic = false;
            mCurrentObject = GetScene().CreateObject<CSceneObject>(*mCurrentTemplate, p);
            mCurrentObject->SetActive(false);
        }
    }
}

void CTest1::SelectFigure()
{
    mCurrentFigure = GetDebugDrawSystem().FindFigure(GetCursorPos());

    if (mCurrentFigure != nullptr)
    {
        GetEventSystem().SetMode("figure_movement");
    }
    else
    {
        GetEventSystem().SetMode("editor_mode");
    }
}

} // namespace test

} // namespace drash
