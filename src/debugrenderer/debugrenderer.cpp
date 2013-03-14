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
#include "../greng/camera.h"
#include "../greng/grengsystemsset.h"
#include "../misc/plane.h"
#include "../misc/ray.h"
#include "../levelmanager/leveldesc.h"
#include "../templates/templatesystem.h"

namespace greng
{
    class CMesh;
}

namespace drash
{

bool CDebugRenderer::Init()
{
    Release();

    if (InitTextures() == false ||
        InitShaders() == false)
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

    if (mLight == nullptr && mSpotLight1 == nullptr)
    {
        LOG_ERR("CDebugRenderer::Render(): neither mLight nor mSpotLight1 is set");
        return;
    }

    unsigned int ic = mScene->EnumObjects();

    for (unsigned int i = 0; i < ic; i++)
    {
        CSceneObject *o = mScene->GetObjects()[i];
        unsigned int jc = o->EnumFigures();

        for (unsigned int j = 0; j < jc; j++)
        {
            CFigure *f = o->GetFigures()[j];

            std::vector<greng::CVertex> mv;
            std::vector<unsigned int> mi;

            greng::CMesh *m = CreateMesh(f->GetVertices(),
                                         f->EnumVertices(),
                                         f->GetZ(),
                                         f->GetDepth());

            if (m != nullptr)
            {
                mGrengSystems->GetMeshManager().ComputeNormals(m);
                mGrengSystems->GetMeshManager().ComputeTangentSpace(m);

                CMatrix4f rot;
                MatrixRotationZ(rot, o->GetAngle());

                CMatrix4f trans;
                MatrixTranslation(trans, o->GetPos());

                CMatrix4f model;
                MatrixMultiply(trans, rot, model);

                CMatrix4f model_view;
                MatrixMultiply(GetCamera()->GetViewMatrix(), model, model_view);

                greng::CTexture *textures[2] = { mTexture1Diffuse, mTexture1Normal };

                mGrengSystems->GetRenderer().RenderMesh(m,
                                         0,
                                         textures,
                                         2,
                                         mLight == nullptr ? mShaderProgram1 : mShaderProgram2,
                                         &model,
                                         nullptr,
                                         &model_view,
                                         &mCamera->GetProjectionMatrix(),
                                         mLight,
                                         mSpotLight1,
                                         &mCamera->GetPos().Get());

                mGrengSystems->GetMeshManager().DestroyMesh(m);
            }

            for (unsigned int k = 0; k < jc; k++)
            {
                auto compute_centroid = [] (const CVec2f * _vertices, unsigned int _count) -> CVec2f
                {
                    CVec2f res;

                    if (_count != 0)
                    {
                        res = _vertices[0];

                        for (unsigned int l = 1; l < _count; l++)
                        {
                            res += _vertices[l];
                        }

                        res /= _count;
                    }

                    return res;
                };

                if (o->GetDestructionGraph().size() == jc * jc)
                {
                    if (o->GetDestructionGraph()[j * jc + k] != 0)
                    {
                        CVec3f c1(o->GetWorldPoint(compute_centroid(f->GetVertices(),
                                                                    f->EnumVertices())),
                                  o->GetPosZ() + f->GetZ());
                        CVec3f c2(o->GetWorldPoint(compute_centroid(o->GetFigures()[k]->GetVertices(),
                                                                    o->GetFigures()[k]->EnumVertices())),
                                  o->GetPosZ() + o->GetFigures()[k]->GetZ());



                        mGrengSystems->GetRenderer().DrawLine(mCamera,
                                                              c1,
                                                              c2,
                                                              2,
                                                              CColor4f(1, 0, 0, 1),
                                                              false);
                    }
                    else if (o->GetDestructionGraph()[k * jc + j] != 0)
                    {
                        CVec3f c1(o->GetWorldPoint(compute_centroid(f->GetVertices(),
                                                                    f->EnumVertices())),
                                  o->GetPosZ() + f->GetZ());
                        CVec3f c2(o->GetWorldPoint(compute_centroid(o->GetFigures()[k]->GetVertices(),
                                                                    o->GetFigures()[k]->EnumVertices())),
                                  o->GetPosZ() + o->GetFigures()[k]->GetZ());



                        mGrengSystems->GetRenderer().DrawLine(mCamera,
                                                              c1,
                                                              c2,
                                                              2,
                                                              CColor4f(1, 0, 0, 1),
                                                              false);
                    }
                }
            }
        }
    }
}

void CDebugRenderer::RenderObject(const CSceneObjectGeometry &_geometry, const CSceneObjectParams &_params)
{
    for (unsigned int i = 0; i < _geometry.mFigures.size(); i++)
    {
        greng::CMesh *m = CreateMesh(&_geometry.mFigures[i].mVertices[0],
                                     _geometry.mFigures[i].mVertices.size(),
                                     _geometry.mFigures[i].mZ,
                                     _geometry.mFigures[i].mDepth);

        if (m != nullptr)
        {
            mGrengSystems->GetMeshManager().ComputeNormals(m);
            mGrengSystems->GetMeshManager().ComputeTangentSpace(m);

            CMatrix4f rot;
            MatrixRotationZ(rot, _params.mAngle);

            CMatrix4f trans;
            MatrixTranslation(trans, _params.mPos);

            CMatrix4f model;
            MatrixMultiply(trans, rot, model);

            CMatrix4f model_view;
            MatrixMultiply(GetCamera()->GetViewMatrix(), model, model_view);

            greng::CTexture *textures[2] = { mTexture1Diffuse, mTexture1Normal };

            mGrengSystems->GetRenderer().RenderMesh(m,
                                     0,
                                     textures,
                                     2,
                                     mLight == nullptr ? mShaderProgram1 : mShaderProgram2,
                                     &model,
                                     nullptr,
                                     &model_view,
                                     &mCamera->GetProjectionMatrix(),
                                     mLight,
									 mSpotLight1,
								     &mCamera->GetPos().Get());

            mGrengSystems->GetMeshManager().DestroyMesh(m);
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

    for (i = 0; i < mScene->EnumObjects(); i++)
    {
        CSceneObject *cur_obj = mScene->GetObjects()[i];

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

    for (; i < mScene->EnumObjects(); i++)
    {
        CSceneObject *cur_obj = mScene->GetObjects()[i];

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

CSceneObject *CDebugRenderer::FindObject(const greng::CCamera *_camera, const CVec2f &_pos) const
{
    if (_camera == nullptr)
    {
        return nullptr;
    }

    CSceneObject *res = nullptr;

    unsigned int i = 0;
    bool brk = false;
    float z_nearest = 0;

    for (i = 0; i < mScene->EnumObjects(); i++)
    {
        CSceneObject *cur_obj = mScene->GetObjects()[i];

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
                res = cur_obj;
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

    for (; i < mScene->EnumObjects(); i++)
    {
        CSceneObject *cur_obj = mScene->GetObjects()[i];

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
                    res = cur_obj;
                    z_nearest = z;
                }
            }
        }
    }

    return res;
}

CLevelObjectDesc *CDebugRenderer::FindObject(const greng::CCamera * _camera,
                                             const CVec2f & _pos,
                                             std::function<CLevelObjectDesc * (unsigned int)> _object_getter,
                                             unsigned int _objects_count)
{
    CVec4f v(0, 0, -1, 1);
    CVec4f res;
    MatrixMultiply(_camera->GetAntiRotationMatrix(), v, res);

    res.Normalize();

    LOG_INFO("view "<<res.mX<<' '<<res.mY<<' '<<res.mZ);

    CVec3f center(0);
    float size = 0;

    auto compute_dummy = [this, & center, & size] (CLevelObjectDesc * _desc)
    {
        CVec3f min(0, 0, 0);
        CVec3f max(0, 0, 0);

        center = 0;
        size = 0;

        CSceneObjectGeometry * _geometry = mTemplateSystem->FindTemplate(_desc->mGeometryName);

        if (_geometry == nullptr)
        {
            return;
        }

        for (auto i = _geometry->mFigures.begin(); i != _geometry->mFigures.end(); i++)
        {
            for (auto j = i->mVertices.begin(); j != i->mVertices.end(); j++)
            {
                max = CVec3f(*j, 0);
                min = CVec3f(*j, 0);
            }
        }

        for (auto i = _geometry->mFigures.begin(); i != _geometry->mFigures.end(); i++)
        {
            for (auto j = i->mVertices.begin(); j != i->mVertices.end(); j++)
            {
                max.mX = math::Max(max.mX, j->mX);
                max.mY = math::Max(max.mY, j->mY);
                max.mZ = math::Max(max.mZ, math::Abs(i->mDepth));

                min.mX = math::Min(min.mX, j->mX);
                min.mY = math::Min(min.mY, j->mY);
                min.mZ = math::Min(min.mZ, - math::Abs(i->mDepth));
            }
        }

        center = max;
        center += min;
        center *= 0.5f;

        center += _desc->mParams.mPos;

        max -= min;

        size = max.Length() * 0.5f * 0.8f;
    };

    for (unsigned int i = 0; i < _objects_count; i++)
    {
        CLevelObjectDesc * desc = _object_getter(i);

        compute_dummy(desc);

        CPlane plane(center, res);

        CVec3f p;
        _camera->CastRay(_pos, plane, p);

        p -= center;

        if (p.Length() < size)
        {
            LOG_ERR("true");
            return desc;
        }
    }

    LOG_ERR("false");
    return nullptr;
}

bool CDebugRenderer::InitTextures()
{
    mTexture1Diffuse = mGrengSystems->GetTextureManager().CreateTextureFromFile("assets/wall5.png");
    mTexture1Normal = mGrengSystems->GetTextureManager().CreateTextureFromFile("assets/wall2_normal.png");

    if (mTexture1Diffuse == nullptr ||
        mTexture1Normal == nullptr)
    {
        return false;
    }

    return true;
}

bool CDebugRenderer::InitShaders()
{
    greng::CVertexShader *vs = mGrengSystems->GetVertexShaderManager().CreateShaderFromFile("shaders/shader5.120.vs");
    greng::CFragmentShader *fs = mGrengSystems->GetFragmentShaderManager().CreateShaderFromFile("shaders/shader5.120.fs");
    mShaderProgram1 = mGrengSystems->GetShaderProgramManager().CreateProgram(vs, fs);
    vs = mGrengSystems->GetVertexShaderManager().CreateShaderFromFile("shaders/shader4.120.vs");
    fs = mGrengSystems->GetFragmentShaderManager().CreateShaderFromFile("shaders/shader4.120.fs");
    mShaderProgram2 = mGrengSystems->GetShaderProgramManager().CreateProgram(vs, fs);

    if (mShaderProgram1 == nullptr && mShaderProgram2 == nullptr)
    {
        return false;
    }

    return true;
}

greng::CMesh *CDebugRenderer::CreateMesh(const CVec2f *_vertices,
                                         unsigned int _vertices_count,
                                         float _z,
                                         float _depth) const
{
    if (_vertices == nullptr)
    {
        return nullptr;
    }

    greng::CMesh *m = nullptr;

    std::vector<greng::CVertex> mv;
    std::vector<unsigned int> mi;

    if (_vertices_count >= 3)
    {
        CVec3f min(_vertices[0], _z - 0.5f * _depth);
        CVec3f max(_vertices[0], _z + 0.5f * _depth);

        for (unsigned int k = 1; k < _vertices_count; k++)
        {
            min.mX = math::Min<float>(min.mX, _vertices[k].mX);
            min.mY = math::Min<float>(min.mY, _vertices[k].mY);
            max.mX = math::Max<float>(max.mX, _vertices[k].mX);
            max.mY = math::Max<float>(max.mY, _vertices[k].mY);
        }

        max.Vec2() -= min.Vec2();

        mv.resize(_vertices_count * 2 + _vertices_count * 4);

        // front and back faces

        for (unsigned int k = 0; k < _vertices_count; k++)
        {
            mv[k].mPos = CVec3f(_vertices[k], max.mZ);
            mv[_vertices_count + k].mPos = CVec3f(_vertices[k], min.mZ);

            mv[k].mUV = mv[k].mPos;
            mv[_vertices_count + k].mUV = mv[_vertices_count + k].mPos;

            mv[k].mUV -= min.Vec2();
            mv[_vertices_count + k].mUV -= min.Vec2();

            mv[k].mUV *= mTexCoordsScale;
            mv[_vertices_count + k].mUV *= mTexCoordsScale;
        }

        for (unsigned int k = 2; k < _vertices_count; k++)
        {
            mi.push_back(0);
            mi.push_back(k - 1);
            mi.push_back(k);
        }

        for (unsigned int k = 2; k < _vertices_count; k++)
        {
            mi.push_back(_vertices_count);
            mi.push_back(_vertices_count + k);
            mi.push_back(_vertices_count + k - 1);
        }

        // sides

        for (unsigned int k = 1; k < _vertices_count; k++)
        {
            mv[2 * _vertices_count + (k - 1) * 4 + 0].mPos = CVec3f(_vertices[k - 1], max.mZ);
            mv[2 * _vertices_count + (k - 1) * 4 + 1].mPos = CVec3f(_vertices[k - 1], min.mZ);
            mv[2 * _vertices_count + (k - 1) * 4 + 2].mPos = CVec3f(_vertices[k], min.mZ);
            mv[2 * _vertices_count + (k - 1) * 4 + 3].mPos = CVec3f(_vertices[k], max.mZ);

            CVec2f tmp = _vertices[k - 1];
            tmp -= _vertices[k];

            mv[2 * _vertices_count + (k - 1) * 4 + 0].mUV.Set(0, 0);
            mv[2 * _vertices_count + (k - 1) * 4 + 1].mUV.Set(0, math::Abs(max.mZ - min.mZ) * mTexCoordsScale);
            mv[2 * _vertices_count + (k - 1) * 4 + 2].mUV.Set(tmp.Length() * mTexCoordsScale, math::Abs(max.mZ - min.mZ) * mTexCoordsScale);
            mv[2 * _vertices_count + (k - 1) * 4 + 3].mUV.Set(tmp.Length() * mTexCoordsScale, 0);

            mi.push_back(2 * _vertices_count + (k - 1) * 4 + 0);
            mi.push_back(2 * _vertices_count + (k - 1) * 4 + 1);
            mi.push_back(2 * _vertices_count + (k - 1) * 4 + 2);
            mi.push_back(2 * _vertices_count + (k - 1) * 4 + 2);
            mi.push_back(2 * _vertices_count + (k - 1) * 4 + 3);
            mi.push_back(2 * _vertices_count + (k - 1) * 4 + 0);
        }
        mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 0].mPos = CVec3f(_vertices[_vertices_count - 1], max.mZ);
        mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 1].mPos = CVec3f(_vertices[_vertices_count - 1], min.mZ);
        mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 2].mPos = CVec3f(_vertices[0], min.mZ);
        mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 3].mPos = CVec3f(_vertices[0], max.mZ);

        CVec2f tmp = _vertices[_vertices_count - 1];
        tmp -= _vertices[0];

        mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 0].mUV.Set(0, 0);
        mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 1].mUV.Set(0, math::Abs(max.mZ - min.mZ) * mTexCoordsScale);
        mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 2].mUV.Set(tmp.Length() * mTexCoordsScale, math::Abs(max.mZ - min.mZ) * mTexCoordsScale);
        mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 3].mUV.Set(tmp.Length() * mTexCoordsScale, 0);

        mi.push_back(2 * _vertices_count + (_vertices_count - 1) * 4 + 0);
        mi.push_back(2 * _vertices_count + (_vertices_count - 1) * 4 + 1);
        mi.push_back(2 * _vertices_count + (_vertices_count - 1) * 4 + 2);
        mi.push_back(2 * _vertices_count + (_vertices_count - 1) * 4 + 2);
        mi.push_back(2 * _vertices_count + (_vertices_count - 1) * 4 + 3);
        mi.push_back(2 * _vertices_count + (_vertices_count - 1) * 4 + 0);

        m = mGrengSystems->GetMeshManager().CreateMeshFromVertices(&mv[0], mv.size(), &mi[0], mi.size());
    }

    return m;
}

} // namespace drash
