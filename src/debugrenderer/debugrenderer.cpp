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
#include "../greng/greng.h"
#include "../misc/plane.h"
#include "../misc/ray.h"
#include "../levelmanager/leveldesc.h"
#include "../scene/geometrymanager.h"

namespace greng {
    class Mesh;
}

namespace drash {

    DebugRenderer::DebugRenderer(greng::Greng& _greng, Scene& _scene,
                                 GeometryManager& _geometry_manager)
        : greng(_greng), scene(_scene), geometryManager(_geometry_manager) {
        InitTextures();
        InitShaders();
    }

    void DebugRenderer::Render() const {
        if (camera == nullptr) {
            LOG_ERR("DebugRenderer::Render(): camera is not set");
            return;
        }

        if (light == nullptr && spotLight1 == nullptr) {
            LOG_ERR("DebugRenderer::Render(): neither light nor spotLight1 "
                    "is set");
            return;
        }

        unsigned int ic = scene.EnumObjects();

        for (unsigned int i = 0; i < ic; i++) {
            SceneObject* o = scene.GetObjects()[i];
            unsigned int jc = o->EnumFigures();

            for (unsigned int j = 0; j < jc; j++) {
                Figure* f = o->GetFigures()[j];

                std::vector<greng::Vertex> mv;
                std::vector<unsigned int> mi;

                greng::Mesh* m = CreateMesh(f->GetVertices(), f->EnumVertices(),
                                            f->GetZ(), f->GetDepth());

                if (m != nullptr) {
                    greng.GetMeshManager().ComputeNormals(m);
                    greng.GetMeshManager().ComputeTangentSpace(m);

                    Matrix4f rot;
                    MatrixRotationZ(rot, o->GetAngle());

                    Matrix4f trans;
                    MatrixTranslation(trans, o->GetPos());

                    Matrix4f model;
                    MatrixMultiply(trans, rot, model);

                    Matrix4f model_view;
                    MatrixMultiply(GetCamera()->GetViewMatrix(), model,
                                   model_view);

                    greng::Texture* textures[3] = { texture1Diffuse,
                                                    texture1Normal,
                                                    texture1Specular };

                    auto program = shaderProgram;
                    if (!program) program = light == nullptr ? shaderProgram1 : shaderProgram2;
                    greng.GetRenderer().RenderMesh(
                        m, 0, textures, 3,
                        program,
                        &model, nullptr, &model_view,
                        &camera->GetProjectionMatrix(), light, spotLight1,
                        &camera->GetPos().Get(), FrameBuffer);

                    greng.GetMeshManager().DestroyMesh(m);
                }

                for (unsigned int k = 0; k < jc; k++) {
                    auto compute_centroid = [](const Vec2f * _vertices,
                                               unsigned int _count)->Vec2f {
                        Vec2f res;

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
                            Vec3f c1(o->GetWorldPoint(compute_centroid(
                                         f->GetVertices(), f->EnumVertices())),
                                     o->GetPosZ() + f->GetZ());
                            Vec3f c2(o->GetWorldPoint(compute_centroid(
                                         o->GetFigures()[k]->GetVertices(),
                                         o->GetFigures()[k]->EnumVertices())),
                                     o->GetPosZ() + o->GetFigures()[k]->GetZ());

                            greng.GetRenderer().DrawLine(
                                *camera, c1, c2, 2, Color4f(1, 0, 0, 1), false);
                        } else if (o->GetDestructionGraph()[k * jc + j] != 0) {
                            Vec3f c1(o->GetWorldPoint(compute_centroid(
                                         f->GetVertices(), f->EnumVertices())),
                                     o->GetPosZ() + f->GetZ());
                            Vec3f c2(o->GetWorldPoint(compute_centroid(
                                         o->GetFigures()[k]->GetVertices(),
                                         o->GetFigures()[k]->EnumVertices())),
                                     o->GetPosZ() + o->GetFigures()[k]->GetZ());

                            greng.GetRenderer().DrawLine(
                                *camera, c1, c2, 2, Color4f(1, 0, 0, 1), false);
                        }
                    }
                }
            }
        }
    }

    void DebugRenderer::RenderObject(const SceneObjectGeometry& _geometry,
                                     const SceneObjectParams& _params) {
        for (unsigned int i = 0; i < _geometry.figures.size(); i++) {
            greng::Mesh* m =
                CreateMesh(&_geometry.figures[i].vertices[0],
                           _geometry.figures[i].vertices.size(),
                           _geometry.figures[i].z, _geometry.figures[i].depth);

            if (m != nullptr) {
                greng.GetMeshManager().ComputeNormals(m);
                greng.GetMeshManager().ComputeTangentSpace(m);

                Matrix4f rot;
                MatrixRotationZ(rot, _params.angle);

                Matrix4f trans;
                MatrixTranslation(trans, _params.pos);

                Matrix4f model;
                MatrixMultiply(trans, rot, model);

                Matrix4f model_view;
                MatrixMultiply(GetCamera()->GetViewMatrix(), model, model_view);

                greng::Texture* textures[3] = { texture1Diffuse, texture1Normal,
                                                texture1Specular };

                auto program = shaderProgram;
                if (!program) program = light == nullptr ? shaderProgram1 : shaderProgram2;
                greng.GetRenderer().RenderMesh(
                    m, 0, textures, 3,
                    program, &model,
                    nullptr, &model_view, &camera->GetProjectionMatrix(), light,
                    spotLight1, &camera->GetPos().Get(), FrameBuffer);

                greng.GetMeshManager().DestroyMesh(m);
            }
        }
    }

    Figure* DebugRenderer::FindFigure(const greng::Camera& _camera,
                                      const Vec2f& _pos) const {
        Figure* res = nullptr;

        unsigned int i = 0;
        bool brk = false;
        float z_nearest = 0;

        for (i = 0; i < scene.EnumObjects(); i++) {
            SceneObject* cur_obj = scene.GetObjects()[i];

            for (unsigned int j = 0; j < cur_obj->EnumFigures(); j++) {
                Figure* cur_fgr = cur_obj->GetFigures()[j];

                Plane plane;
                plane.SetNormal(Vec3f(0, 0, 1));
                plane.SetPoint(
                    Vec3f(0, 0, cur_obj->GetPosZ() + cur_fgr->GetZ()));

                Vec3f pos;
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
            SceneObject* cur_obj = scene.GetObjects()[i];

            for (unsigned int j = 0; j < cur_obj->EnumFigures(); j++) {
                Figure* cur_fgr = cur_obj->GetFigures()[j];

                Plane plane;
                plane.SetNormal(Vec3f(0, 0, 1));
                plane.SetPoint(
                    Vec3f(0, 0, cur_obj->GetPosZ() + cur_fgr->GetZ()));

                Vec3f pos;
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

    SceneObject* DebugRenderer::FindObject(const greng::Camera& _camera,
                                           const Vec2f& _pos) const {
        SceneObject* res = nullptr;

        unsigned int i = 0;
        bool brk = false;
        float z_nearest = 0;

        for (i = 0; i < scene.EnumObjects(); i++) {
            SceneObject* cur_obj = scene.GetObjects()[i];

            for (unsigned int j = 0; j < cur_obj->EnumFigures(); j++) {
                Figure* cur_fgr = cur_obj->GetFigures()[j];

                Plane plane;
                plane.SetNormal(Vec3f(0, 0, 1));
                plane.SetPoint(
                    Vec3f(0, 0, cur_obj->GetPosZ() + cur_fgr->GetZ()));

                Vec3f pos;
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
            SceneObject* cur_obj = scene.GetObjects()[i];

            for (unsigned int j = 0; j < cur_obj->EnumFigures(); j++) {
                Figure* cur_fgr = cur_obj->GetFigures()[j];

                Plane plane;
                plane.SetNormal(Vec3f(0, 0, 1));
                plane.SetPoint(
                    Vec3f(0, 0, cur_obj->GetPosZ() + cur_fgr->GetZ()));

                Vec3f pos;
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

    LevelObjectDesc* DebugRenderer::FindObject(
        const greng::Camera& _camera, const Vec2f& _pos,
        std::function<LevelObjectDesc*(unsigned int)> _object_getter,
        unsigned int _objects_count) {
        Vec4f v(0, 0, -1, 1);
        Vec4f res;
        MatrixMultiply(_camera.GetAntiRotationMatrix(), v, res);

        res.Normalize();

        LOG_INFO("view " << res.x << ' ' << res.y << ' ' << res.z);

        Vec3f center(0);
        float size = 0;

        auto compute_dummy = [this, &center, &size](LevelObjectDesc* _desc) {
            Vec3f min(0, 0, 0);
            Vec3f max(0, 0, 0);

            center = 0;
            size = 0;

            SceneObjectGeometry* _geometry =
                geometryManager.GetGeometry(_desc->geometryName);

            if (_geometry == nullptr) {
                return;
            }

            for (auto i = _geometry->figures.begin();
                 i != _geometry->figures.end(); i++) {
                for (auto j = i->vertices.begin(); j != i->vertices.end();
                     j++) {
                    max = Vec3f(*j, 0);
                    min = Vec3f(*j, 0);
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
            LevelObjectDesc* desc = _object_getter(i);

            compute_dummy(desc);

            Plane plane(center, res);

            Vec3f p;
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

    bool DebugRenderer::InitTextures() {
        texture1Diffuse = greng.GetTextureManager().CreateTextureFromFile(
            "assets/floor/diffuse.png");
        texture1Normal = greng.GetTextureManager().CreateTextureFromFile(
            "assets/floor/normal.png");
        texture1Specular = greng.GetTextureManager().CreateTextureFromFile(
            "assets/floor/specular.png");

        if (texture1Diffuse == nullptr || texture1Normal == nullptr ||
            texture1Specular == nullptr) {
            return false;
        }

        return true;
    }

    bool DebugRenderer::InitShaders() {
        greng::VertexShader* vs =
            greng.GetVertexShaderManager().CreateShaderFromFile(
                "shaders/shader6.120.vs");
        greng::FragmentShader* fs =
            greng.GetFragmentShaderManager().CreateShaderFromFile(
                "shaders/shader6.120.fs");
        shaderProgram1 = greng.GetShaderProgramManager().CreateProgram(vs, fs);
        vs = greng.GetVertexShaderManager().CreateShaderFromFile(
            "shaders/shader7.120.vs");
        fs = greng.GetFragmentShaderManager().CreateShaderFromFile(
            "shaders/shader7.120.fs");
        shaderProgram2 = greng.GetShaderProgramManager().CreateProgram(vs, fs);

        if (shaderProgram1 == nullptr && shaderProgram2 == nullptr) {
            return false;
        }

        return true;
    }

    greng::Mesh* DebugRenderer::CreateMesh(const Vec2f* _vertices,
                                           unsigned int _vertices_count,
                                           float _z, float _depth) const {
        if (_vertices == nullptr) {
            return nullptr;
        }

        greng::Mesh* m = nullptr;

        std::vector<greng::Vertex> mv;
        std::vector<unsigned int> mi;

        if (_vertices_count >= 3) {
            Vec3f min(_vertices[0], _z - 0.5f * _depth);
            Vec3f max(_vertices[0], _z + 0.5f * _depth);

            for (unsigned int k = 1; k < _vertices_count; k++) {
                min.x = math::Min<float>(min.x, _vertices[k].x);
                min.y = math::Min<float>(min.y, _vertices[k].y);
                max.x = math::Max<float>(max.x, _vertices[k].x);
                max.y = math::Max<float>(max.y, _vertices[k].y);
            }

            max.AsVec2() -= min.AsVec2();

            mv.resize(_vertices_count * 2 + _vertices_count * 4);

            // front and back faces

            for (unsigned int k = 0; k < _vertices_count; k++) {
                mv[k].pos = Vec3f(_vertices[k], max.z);
                mv[_vertices_count + k].pos = Vec3f(_vertices[k], min.z);

                mv[k].uV = mv[k].pos;
                mv[_vertices_count + k].uV = mv[_vertices_count + k].pos;

                mv[k].uV -= min.AsVec2();
                mv[_vertices_count + k].uV -= min.AsVec2();

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
                    Vec3f(_vertices[k - 1], max.z);
                mv[2 * _vertices_count + (k - 1) * 4 + 1].pos =
                    Vec3f(_vertices[k - 1], min.z);
                mv[2 * _vertices_count + (k - 1) * 4 + 2].pos =
                    Vec3f(_vertices[k], min.z);
                mv[2 * _vertices_count + (k - 1) * 4 + 3].pos =
                    Vec3f(_vertices[k], max.z);

                Vec2f tmp = _vertices[k - 1];
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
                Vec3f(_vertices[_vertices_count - 1], max.z);
            mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 1].pos =
                Vec3f(_vertices[_vertices_count - 1], min.z);
            mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 2].pos =
                Vec3f(_vertices[0], min.z);
            mv[2 * _vertices_count + (_vertices_count - 1) * 4 + 3].pos =
                Vec3f(_vertices[0], max.z);

            Vec2f tmp = _vertices[_vertices_count - 1];
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

            m = greng.GetMeshManager().CreateMeshFromVertices(
                &mv[0], mv.size(), &mi[0], mi.size());
        }

        return m;
    }

} // namespace drash
