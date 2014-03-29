// DRASH_LICENSE_BEGIN
/*

drash GPL Source Code
Copyright (C) 2012-2014 Edward Knyshov, Yuriy Shatilin.

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
#include "../scene/geometrymanager.h"

namespace greng {
    class CMesh;
}

namespace drash {

    CDebugRenderer::CDebugRenderer(greng::CGrengSystemsSet& _greng_systems,
                                   CScene& _scene,
                                   CGeometryManager& _geometry_manager)
        : grengSystems(_greng_systems), scene(_scene),
          geometryManager(_geometry_manager) {
        InitTextures();
        InitShaders();
    }

    void CDebugRenderer::Render() const {
        if (camera == nullptr) {
            LOG_ERR("CDebugRenderer::Render(): camera is not set");
            return;
        }

        if (light == nullptr && spotLight1 == nullptr) {
            LOG_ERR("CDebugRenderer::Render(): neither light nor spotLight1 "
                    "is set");
            return;
        }

        unsigned int ic = scene.EnumObjects();

        for (unsigned int i = 0; i < ic; i++) {
            CSceneObject* o = scene.GetObjects()[i];
            unsigned int jc = o->EnumFigures();

            for (unsigned int j = 0; j < jc; j++) {
                CFigure* f = o->GetFigures()[j];

                std::vector<greng::CVertex> mv;
                std::vector<unsigned int> mi;

                greng::CMesh* m =
                    CreateMesh(f->GetVertices(), f->EnumVertices(), f->GetZ(),
                               f->GetDepth());

                if (m != nullptr) {
                    grengSystems.GetMeshManager().ComputeNormals(m);
                    grengSystems.GetMeshManager().ComputeTangentSpace(m);

                    CMatrix4f rot;
                    MatrixRotationZ(rot, o->GetAngle());

                    CMatrix4f trans;
                    MatrixTranslation(trans, o->GetPos());

                    CMatrix4f model;
                    MatrixMultiply(trans, rot, model);

                    CMatrix4f model_view;
                    MatrixMultiply(GetCamera()->GetViewMatrix(), model,
                                   model_view);

                    greng::CTexture* textures[2] = { texture1Diffuse,
                                                     texture1Normal };

                    grengSystems.GetRenderer().RenderMesh(
                        m, 0, textures, 2,
                        light == nullptr ? shaderProgram1 : shaderProgram2,
                        &model, nullptr, &model_view,
                        &camera->GetProjectionMatrix(), light, spotLight1,
                        &camera->GetPos().Get());

                    grengSystems.GetMeshManager().DestroyMesh(m);
                }

                for (unsigned int k = 0; k < jc; k++) {
                    auto compute_centroid = [](const CVec2f * _vertices,
                                               unsigned int _count)->CVec2f {
                        CVec2f res;

                        if (_count != 0) {
                            res = _vertices[0];

                            for (unsigned int l = 1; l < _count; l++) {
                                res += _vertices[l];
                            }

                            res /= _count;
                        }

                        return res;
                    };

                    if (o->GetDestructionGraph().size() == jc * jc) {
                        if (o->GetDestructionGraph()[j * jc + k] != 0) {
                            CVec3f c1(o->GetWorldPoint(compute_centroid(
                                          f->GetVertices(), f->EnumVertices())),
                                      o->GetPosZ() + f->GetZ());
                            CVec3f c2(o->GetWorldPoint(compute_centroid(
                                          o->GetFigures()[k]->GetVertices(),
                                          o->GetFigures()[k]->EnumVertices())),
                                      o->GetPosZ() +
                                          o->GetFigures()[k]->GetZ());

                            grengSystems.GetRenderer().DrawLine(
                                *camera, c1, c2, 2, CColor4f(1, 0, 0, 1),
                                false);
                        } else if (o->GetDestructionGraph()[k * jc + j] != 0) {
                            CVec3f c1(o->GetWorldPoint(compute_centroid(
                                          f->GetVertices(), f->EnumVertices())),
                                      o->GetPosZ() + f->GetZ());
                            CVec3f c2(o->GetWorldPoint(compute_centroid(
                                          o->GetFigures()[k]->GetVertices(),
                                          o->GetFigures()[k]->EnumVertices())),
                                      o->GetPosZ() +
                                          o->GetFigures()[k]->GetZ());

                            grengSystems.GetRenderer().DrawLine(
                                *camera, c1, c2, 2, CColor4f(1, 0, 0, 1),
                                false);
                        }
                    }
                }
            }
        }
    }

    void CDebugRenderer::RenderObject(const CSceneObjectGeometry& _geometry,
                                      const CSceneObjectParams& _params) {
        for (unsigned int i = 0; i < _geometry.figures.size(); i++) {
            greng::CMesh* m = CreateMesh(&_geometry.figures[i].vertices[0],
                                         _geometry.figures[i].vertices.size(),
                                         _geometry.figures[i].z,
                                         _geometry.figures[i].depth);

            if (m != nullptr) {
                grengSystems.GetMeshManager().ComputeNormals(m);
                grengSystems.GetMeshManager().ComputeTangentSpace(m);

                CMatrix4f rot;
                MatrixRotationZ(rot, _params.angle);

                CMatrix4f trans;
                MatrixTranslation(trans, _params.pos);

                CMatrix4f model;
                MatrixMultiply(trans, rot, model);

                CMatrix4f model_view;
                MatrixMultiply(GetCamera()->GetViewMatrix(), model, model_view);

                greng::CTexture* textures[2] = { texture1Diffuse,
                                                 texture1Normal };

                grengSystems.GetRenderer().RenderMesh(
                    m, 0, textures, 2,
                    light == nullptr ? shaderProgram1 : shaderProgram2,
                    &model, nullptr, &model_view,
                    &camera->GetProjectionMatrix(), light, spotLight1,
                    &camera->GetPos().Get());

                grengSystems.GetMeshManager().DestroyMesh(m);
            }
        }
    }

    CFigure* CDebugRenderer::FindFigure(const greng::CCamera& _camera,
                                        const CVec2f& _pos) const {
        CFigure* res = nullptr;

        unsigned int i = 0;
        bool brk = false;
        float z_nearest = 0;

        for (i = 0; i < scene.EnumObjects(); i++) {
            CSceneObject* cur_obj = scene.GetObjects()[i];

            for (unsigned int j = 0; j < cur_obj->EnumFigures(); j++) {
                CFigure* cur_fgr = cur_obj->GetFigures()[j];

                CPlane plane;
                plane.SetNormal(CVec3f(0, 0, 1));
                plane.SetPoint(
                    CVec3f(0, 0, cur_obj->GetPosZ() + cur_fgr->GetZ()));

                CVec3f pos;
                _camera.CastRay(_pos, plane, pos);

                if (cur_fgr->TestPoint(pos)) {
                    res = cur_fgr;
                    pos -= _camera.GetPos().Get();
                    z_nearest = pos.LengthSquared();
                    brk = true;
                }

                if (brk == true) {
                    break;
                }
            }

            if (brk == true) {
                break;
            }
        }

        for (; i < scene.EnumObjects(); i++) {
            CSceneObject* cur_obj = scene.GetObjects()[i];

            for (unsigned int j = 0; j < cur_obj->EnumFigures(); j++) {
                CFigure* cur_fgr = cur_obj->GetFigures()[j];

                CPlane plane;
                plane.SetNormal(CVec3f(0, 0, 1));
                plane.SetPoint(
                    CVec3f(0, 0, cur_obj->GetPosZ() + cur_fgr->GetZ()));

                CVec3f pos;
                _camera.CastRay(_pos, plane, pos);

                if (cur_fgr->TestPoint(pos)) {
                    pos -= _camera.GetPos().Get();

                    float z = pos.LengthSquared();

                    if (z_nearest > z) {
                        res = cur_fgr;
                        z_nearest = z;
                    }
                }
            }
        }

        return res;
    }

    CSceneObject* CDebugRenderer::FindObject(const greng::CCamera& _camera,
                                             const CVec2f& _pos) const {
        CSceneObject* res = nullptr;

        unsigned int i = 0;
        bool brk = false;
        float z_nearest = 0;

        for (i = 0; i < scene.EnumObjects(); i++) {
            CSceneObject* cur_obj = scene.GetObjects()[i];

            for (unsigned int j = 0; j < cur_obj->EnumFigures(); j++) {
                CFigure* cur_fgr = cur_obj->GetFigures()[j];

                CPlane plane;
                plane.SetNormal(CVec3f(0, 0, 1));
                plane.SetPoint(
                    CVec3f(0, 0, cur_obj->GetPosZ() + cur_fgr->GetZ()));

                CVec3f pos;
                _camera.CastRay(_pos, plane, pos);

                if (cur_fgr->TestPoint(pos)) {
                    res = cur_obj;
                    pos -= _camera.GetPos().Get();
                    z_nearest = pos.LengthSquared();
                    brk = true;
                }

                if (brk == true) {
                    break;
                }
            }

            if (brk == true) {
                break;
            }
        }

        for (; i < scene.EnumObjects(); i++) {
            CSceneObject* cur_obj = scene.GetObjects()[i];

            for (unsigned int j = 0; j < cur_obj->EnumFigures(); j++) {
                CFigure* cur_fgr = cur_obj->GetFigures()[j];

                CPlane plane;
                plane.SetNormal(CVec3f(0, 0, 1));
                plane.SetPoint(
                    CVec3f(0, 0, cur_obj->GetPosZ() + cur_fgr->GetZ()));

                CVec3f pos;
                _camera.CastRay(_pos, plane, pos);

                if (cur_fgr->TestPoint(pos)) {
                    pos -= _camera.GetPos().Get();

                    float z = pos.LengthSquared();

                    if (z_nearest > z) {
                        res = cur_obj;
                        z_nearest = z;
                    }
                }
            }
        }

        return res;
    }

    CLevelObjectDesc* CDebugRenderer::FindObject(
        const greng::CCamera& _camera, const CVec2f& _pos,
        std::function<CLevelObjectDesc*(unsigned int)> _object_getter,
        unsigned int _objects_count) {
        CVec4f v(0, 0, -1, 1);
        CVec4f res;
        MatrixMultiply(_camera.GetAntiRotationMatrix(), v, res);

        res.Normalize();

        LOG_INFO("view " << res.x << ' ' << res.y << ' ' << res.z);

        CVec3f center(0);
        float size = 0;

        auto compute_dummy = [this, &center, &size](CLevelObjectDesc* _desc) {
            CVec3f min(0, 0, 0);
            CVec3f max(0, 0, 0);

            center = 0;
            size = 0;

            CSceneObjectGeometry* _geometry =
                geometryManager.GetGeometry(_desc->geometryName);

            if (_geometry == nullptr) {
                return;
            }

            for (auto i = _geometry->figures.begin();
                 i != _geometry->figures.end(); i++) {
                for (auto j = i->vertices.begin(); j != i->vertices.end();
                     j++) {
                    max = CVec3f(*j, 0);
                    min = CVec3f(*j, 0);
                }
            }

            for (auto i = _geometry->figures.begin();
                 i != _geometry->figures.end(); i++) {
                for (auto j = i->vertices.begin(); j != i->vertices.end();
                     j++) {
                    max.x = math::Max(max.x, j->x);
                    max.y = math::Max(max.y, j->y);
                    max.z = math::Max(max.z, math::Abs(i->depth));

                    min.x = math::Min(min.x, j->x);
                    min.y = math::Min(min.y, j->y);
                    min.z = math::Min(min.z, -math::Abs(i->depth));
                }
            }

            center = max;
            center += min;
            center *= 0.5f;

            center += _desc->params.pos;

            max -= min;

            size = max.Length() * 0.5f * 0.8f;
        };

        for (unsigned int i = 0; i < _objects_count; i++) {
            CLevelObjectDesc* desc = _object_getter(i);

            compute_dummy(desc);

            CPlane plane(center, res);

            CVec3f p;
            _camera.CastRay(_pos, plane, p);

            p -= center;

            if (p.Length() < size) {
                LOG_ERR("true");
                return desc;
            }
        }

        LOG_ERR("false");
        return nullptr;
    }

    bool CDebugRenderer::InitTextures() {
        texture1Diffuse =
            grengSystems.GetTextureManager().CreateTextureFromFile(
                "assets/floor/diffuse.png");
        texture1Normal =
            grengSystems.GetTextureManager().CreateTextureFromFile(
                "assets/floor/normal.png");

        if (texture1Diffuse == nullptr || texture1Normal == nullptr) {
            return false;
        }

        return true;
    }

    bool CDebugRenderer::InitShaders() {
        greng::CVertexShader* vs =
            grengSystems.GetVertexShaderManager().CreateShaderFromFile(
                "shaders/shader5.120.vs");
        greng::CFragmentShader* fs =
            grengSystems.GetFragmentShaderManager().CreateShaderFromFile(
                "shaders/shader5.120.fs");
        shaderProgram1 =
            grengSystems.GetShaderProgramManager().CreateProgram(vs, fs);
        vs = grengSystems.GetVertexShaderManager().CreateShaderFromFile(
            "shaders/shader4.120.vs");
        fs = grengSystems.GetFragmentShaderManager().CreateShaderFromFile(
            "shaders/shader4.120.fs");
        shaderProgram2 =
            grengSystems.GetShaderProgramManager().CreateProgram(vs, fs);

        if (shaderProgram1 == nullptr && shaderProgram2 == nullptr) {
            return false;
        }

        return true;
    }

    greng::CMesh* CDebugRenderer::CreateMesh(const CVec2f* _vertices,
                                             unsigned int _vertices_count,
                                             float _z, float _depth) const {
        if (_vertices == nullptr) {
            return nullptr;
        }

        greng::CMesh* m = nullptr;

        std::vector<greng::CVertex> mv;
        std::vector<unsigned int> mi;

        if (_vertices_count >= 3) {
            CVec3f min(_vertices[0], _z - 0.5f * _depth);
            CVec3f max(_vertices[0], _z + 0.5f * _depth);

            for (unsigned int k = 1; k < _vertices_count; k++) {
                min.x = math::Min<float>(min.x, _vertices[k].x);
                min.y = math::Min<float>(min.y, _vertices[k].y);
                max.x = math::Max<float>(max.x, _vertices[k].x);
                max.y = math::Max<float>(max.y, _vertices[k].y);
            }

            max.Vec2() -= min.Vec2();

            mv.resize(_vertices_count * 2 + _vertices_count * 4);

            // front and back faces

            for (unsigned int k = 0; k < _vertices_count; k++) {
                mv[k].pos = CVec3f(_vertices[k], max.z);
                mv[_vertices_count + k].pos = CVec3f(_vertices[k], min.z);

                mv[k].uV = mv[k].pos;
                mv[_vertices_count + k].uV = mv[_vertices_count + k].pos;

                mv[k].uV -= min.Vec2();
                mv[_vertices_count + k].uV -= min.Vec2();

                mv[k].uV *= texCoordsScale;
                mv[_vertices_count + k].uV *= texCoordsScale;
            }

            for (unsigned int k = 2; k < _vertices_count; k++) {
                mi.push_back(0);
                mi.push_back(k - 1);
                mi.push_back(k);
            }

            for (unsigned int k = 2; k < _vertices_count; k++) {
                mi.push_back(_vertices_count);
                mi.push_back(_vertices_count + k);
                mi.push_back(_vertices_count + k - 1);
            }

            // sides

            for (unsigned int k = 1; k < _vertices_count; k++) {
                mv[2 * _vertices_count + (k - 1) * 4 + 0].pos =
                    CVec3f(_vertices[k - 1], max.z);
                mv[2 * _vertices_count + (k - 1) * 4 + 1].pos =
                    CVec3f(_vertices[k - 1], min.z);
                mv[2 * _vertices_count + (k - 1) * 4 + 2].pos =
                    CVec3f(_vertices[k], min.z);
                mv[2 * _vertices_count + (k - 1) * 4 + 3].pos =
                    CVec3f(_vertices[k], max.z);

                CVec2f tmp = _vertices[k - 1];
                tmp -= _vertices[k];

                mv[2 * _vertices_count + (k - 1) * 4 + 0].uV.Set(0, 0);
                mv[2 * _vertices_count + (k - 1) * 4 + 1]
                    .uV.Set(0, math::Abs(max.z - min.z) * texCoordsScale);
                mv[2 * _vertices_count + (k - 1) * 4 + 2]
                    .uV.Set(tmp.Length() * texCoordsScale,
                             math::Abs(max.z - min.z) * texCoordsScale);
                mv[2 * _vertices_count + (k - 1) * 4 + 3]
                    .uV.Set(tmp.Length() * texCoordsScale, 0);

                mi.push_back(2 * _vertices_count + (k - 1) * 4 + 0);
                mi.push_back(2 * _vertices_count + (k - 1) * 4 + 1);
                mi.push_back(2 * _vertices_count + (k - 1) * 4 + 2);
                mi.push_back(2 * _vertices_count + (k - 1) * 4 + 2);
                mi.push_back(2 * _vertices_count + (k - 1) * 4 + 3);
                mi.push_back(2 * _vertices_count + (k - 1) * 4 + 0);
            }
            mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 0].pos =
                CVec3f(_vertices[_vertices_count - 1], max.z);
            mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 1].pos =
                CVec3f(_vertices[_vertices_count - 1], min.z);
            mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 2].pos =
                CVec3f(_vertices[0], min.z);
            mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 3].pos =
                CVec3f(_vertices[0], max.z);

            CVec2f tmp = _vertices[_vertices_count - 1];
            tmp -= _vertices[0];

            mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 0]
                .uV.Set(0, 0);
            mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 1]
                .uV.Set(0, math::Abs(max.z - min.z) * texCoordsScale);
            mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 2]
                .uV.Set(tmp.Length() * texCoordsScale,
                         math::Abs(max.z - min.z) * texCoordsScale);
            mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 3]
                .uV.Set(tmp.Length() * texCoordsScale, 0);

            mi.push_back(2 * _vertices_count + (_vertices_count - 1) * 4 + 0);
            mi.push_back(2 * _vertices_count + (_vertices_count - 1) * 4 + 1);
            mi.push_back(2 * _vertices_count + (_vertices_count - 1) * 4 + 2);
            mi.push_back(2 * _vertices_count + (_vertices_count - 1) * 4 + 2);
            mi.push_back(2 * _vertices_count + (_vertices_count - 1) * 4 + 3);
            mi.push_back(2 * _vertices_count + (_vertices_count - 1) * 4 + 0);

            m = grengSystems.GetMeshManager().CreateMeshFromVertices(
                &mv[0], mv.size(), &mi[0], mi.size());
        }

        return m;
    }

} // namespace drash
