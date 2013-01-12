// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2013 Edward Knyshov, Yuriy Shatilin.

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

#include "debugrenderer.h"

#include "../scene/scene.h"
#include "../scene/sceneobject.h"
#include "../scene/figure.h"
#include "../greng/vertex.h"
#include "../greng/meshmanager.h"
#include "../greng/texturemanager.h"
#include "../greng/renderer.h"
#include "../greng/camera.h"

namespace greng
{
    class CMesh;
}

namespace drash
{

bool CDebugRenderer::Init()
{
    Release();

    if (InitTextures() == false)
    {
        return false;
    }

    return true;
}

void CDebugRenderer::Release()
{
}

void CDebugRenderer::Render() const
{
    if (mCamera == nullptr)
    {
        LOG_ERR("CDebugRenderer::Render(): mCamera is not set");
        return;
    }

    if (mShaderProgram == nullptr)
    {
        LOG_ERR("CDebugRenderer::Render(): mShaderProgram is not set");
        return;
    }

    if (mLight == nullptr)
    {
        LOG_ERR("CDebugRenderer::Render(): mLight is not set");
        return;
    }

    unsigned int ic = GetScene()->EnumObjects();

    for (unsigned int i = 0; i < ic; i++)
    {
        CSceneObject *o = GetScene()->GetObjects()[i];
        unsigned int jc = o->EnumFigures();

        for (unsigned int j = 0; j < jc; j++)
        {
            CFigure *f = o->GetFigures()[j];
            unsigned int kc = f->EnumVertices();

            std::vector<greng::CVertex> mv;
            std::vector<unsigned int> mi;

            greng::CMesh *m = nullptr;

            if (kc >= 3)
            {
                CVec3f min(f->GetVertices()[0], f->GetZ() - 0.5f * f->GetDepth());
                CVec3f max(f->GetVertices()[0], f->GetZ() + 0.5f * f->GetDepth());

                for (unsigned int k = 1; k < kc; k++)
                {
                    min.mX = math::Min<float>(min.mX, f->GetVertices()[k].mX);
                    min.mY = math::Min<float>(min.mY, f->GetVertices()[k].mY);
                    max.mX = math::Max<float>(max.mX, f->GetVertices()[k].mX);
                    max.mY = math::Max<float>(max.mY, f->GetVertices()[k].mY);
                }

                max.Vec2() -= min.Vec2();

                mv.resize(kc * 2 + kc * 4);

                // front and back faces

                for (unsigned int k = 0; k < kc; k++)
                {
                    mv[k].mPos = CVec3f(f->GetVertices()[k], max.mZ);
                    mv[kc + k].mPos = CVec3f(f->GetVertices()[k], min.mZ);

                    mv[k].mUV = mv[k].mPos;
                    mv[kc + k].mUV = mv[kc + k].mPos;

                    mv[k].mUV -= min.Vec2();
                    mv[kc + k].mUV -= min.Vec2();

                    mv[k].mUV *= mTexCoordsScale;
                    mv[kc + k].mUV *= mTexCoordsScale;
                }

                for (unsigned int k = 2; k < kc; k++)
                {
                    mi.push_back(0);
                    mi.push_back(k - 1);
                    mi.push_back(k);
                }

                for (unsigned int k = 2; k < kc; k++)
                {
                    mi.push_back(kc);
                    mi.push_back(kc + k);
                    mi.push_back(kc + k - 1);
                }

                // sides

                for (unsigned int k = 1; k < kc; k++)
                {
                    mv[2 * kc + (k - 1) * 4 + 0].mPos = CVec3f(f->GetVertices()[k - 1], max.mZ);
                    mv[2 * kc + (k - 1) * 4 + 1].mPos = CVec3f(f->GetVertices()[k - 1], min.mZ);
                    mv[2 * kc + (k - 1) * 4 + 2].mPos = CVec3f(f->GetVertices()[k], min.mZ);
                    mv[2 * kc + (k - 1) * 4 + 3].mPos = CVec3f(f->GetVertices()[k], max.mZ);

                    CVec2f tmp = f->GetVertices()[k - 1];
                    tmp -= f->GetVertices()[k];

                    mv[2 * kc + (k - 1) * 4 + 0].mUV.Set(0, 0);
                    mv[2 * kc + (k - 1) * 4 + 1].mUV.Set(0, math::Abs(max.mZ - min.mZ) * mTexCoordsScale);
                    mv[2 * kc + (k - 1) * 4 + 2].mUV.Set(tmp.Length() * mTexCoordsScale, math::Abs(max.mZ - min.mZ) * mTexCoordsScale);
                    mv[2 * kc + (k - 1) * 4 + 3].mUV.Set(tmp.Length() * mTexCoordsScale, 0);

                    mi.push_back(2 * kc + (k - 1) * 4 + 0);
                    mi.push_back(2 * kc + (k - 1) * 4 + 1);
                    mi.push_back(2 * kc + (k - 1) * 4 + 2);
                    mi.push_back(2 * kc + (k - 1) * 4 + 2);
                    mi.push_back(2 * kc + (k - 1) * 4 + 3);
                    mi.push_back(2 * kc + (k - 1) * 4 + 0);
                }
                mv[2 * kc + (kc - 1) * 4 + 0].mPos = CVec3f(f->GetVertices()[kc - 1], max.mZ);
                mv[2 * kc + (kc - 1) * 4 + 1].mPos = CVec3f(f->GetVertices()[kc - 1], min.mZ);
                mv[2 * kc + (kc - 1) * 4 + 2].mPos = CVec3f(f->GetVertices()[0], min.mZ);
                mv[2 * kc + (kc - 1) * 4 + 3].mPos = CVec3f(f->GetVertices()[0], max.mZ);

                CVec2f tmp = f->GetVertices()[kc - 1];
                tmp -= f->GetVertices()[0];

                mv[2 * kc + (kc - 1) * 4 + 0].mUV.Set(0, 0);
                mv[2 * kc + (kc - 1) * 4 + 1].mUV.Set(0, math::Abs(max.mZ - min.mZ) * mTexCoordsScale);
                mv[2 * kc + (kc - 1) * 4 + 2].mUV.Set(tmp.Length() * mTexCoordsScale, math::Abs(max.mZ - min.mZ) * mTexCoordsScale);
                mv[2 * kc + (kc - 1) * 4 + 3].mUV.Set(tmp.Length() * mTexCoordsScale, 0);

                mi.push_back(2 * kc + (kc - 1) * 4 + 0);
                mi.push_back(2 * kc + (kc - 1) * 4 + 1);
                mi.push_back(2 * kc + (kc - 1) * 4 + 2);
                mi.push_back(2 * kc + (kc - 1) * 4 + 2);
                mi.push_back(2 * kc + (kc - 1) * 4 + 3);
                mi.push_back(2 * kc + (kc - 1) * 4 + 0);

                m = GetMeshManager()->CreateMeshFromVertices(&mv[0], mv.size(), &mi[0], mi.size());
            }

            if (m != nullptr)
            {
                GetMeshManager()->ComputeNormals(m);

                CMatrix4f rot;
                MatrixRotationZ(rot, o->GetAngle());

                CMatrix4f trans;
                MatrixTranslation(trans, o->GetPos());

                CMatrix4f model;
                MatrixMultiply(trans, rot, model);

                CMatrix4f model_view;
                MatrixMultiply(GetCamera()->GetViewMatrix(), model, model_view);

                GetRenderer()->RenderMesh(m,
                                         0,
                                         mTexture1,
                                         mShaderProgram,
                                         &model,
                                         nullptr,
                                         &model_view,
                                         &mCamera->GetProjectionMatrix(),
                                         mLight);

                GetMeshManager()->DestroyMesh(m);
            }
        }
    }
}

CFigure *CDebugRenderer::FindFigure(const greng::CCamera *_camera, const CVec2f &_pos) const
{
    if (_camera == nullptr)
    {
        return nullptr;
    }

    CFigure *res = nullptr;

    unsigned int i = 0;
    bool brk = false;
    float z_nearest = 0;

    for (i = 0; i < GetScene()->EnumObjects(); i++)
    {
        CSceneObject *cur_obj = GetScene()->GetObjects()[i];

        for (unsigned int j = 0; j < cur_obj->EnumFigures(); j++)
        {
            CFigure *cur_fgr = cur_obj->GetFigures()[j];

            CPlane plane;
            plane.SetNormal(CVec3f(0, 0, 1));
            plane.SetPoint(CVec3f(0, 0, cur_obj->GetPosZ() + cur_fgr->GetZ()));

            CVec3f pos;
            _camera->CastRay(_pos, plane, pos);

            if (cur_fgr->TestPoint(pos))
            {
                res = cur_fgr;
                pos -= _camera->GetPos().Get();
                z_nearest = pos.LengthSquared();
                brk = true;
            }

            if (brk == true)
            {
                break;
            }
        }

        if (brk == true)
        {
            break;
        }
    }

    for (; i < GetScene()->EnumObjects(); i++)
    {
        CSceneObject *cur_obj = GetScene()->GetObjects()[i];

        for (unsigned int j = 0; j < cur_obj->EnumFigures(); j++)
        {
            CFigure *cur_fgr = cur_obj->GetFigures()[j];

            CPlane plane;
            plane.SetNormal(CVec3f(0, 0, 1));
            plane.SetPoint(CVec3f(0, 0, cur_obj->GetPosZ() + cur_fgr->GetZ()));

            CVec3f pos;
            _camera->CastRay(_pos, plane, pos);

            if (cur_fgr->TestPoint(pos))
            {
                pos -= _camera->GetPos().Get();

                float z = pos.LengthSquared();

                if (z_nearest > z)
                {
                    res = cur_fgr;
                    z_nearest = z;
                }
            }
        }
    }

    return res;
}

bool CDebugRenderer::InitTextures()
{
    mTexture1 = GetTextureManager()->CreateTextureFromFile("assets/wall2_diffuse.png");

    if (mTexture1 == nullptr)
    {
        return false;
    }

    return true;
}

} // namespace drash
