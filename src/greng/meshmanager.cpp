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

#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include "meshmanager.h"
#include "mesh.h"
#include "../diag/logger.h"
#include "loadmeshobj.h"
#include <cstring>
#include "../misc/vec4.h"
#include "../misc/matrix4.h"
#include "../misc/math.h"

namespace greng {

    using drash::Logger;

    MeshManager::MeshManager() : meshFactory(meshesCountLimit, "Mesh") {}

    MeshManager::~MeshManager() {
        while (meshFactory.EnumObjects() != 0) {
            DestroyMesh(meshFactory.GetObjects()[0]);
        }
    }

    Mesh* MeshManager::CreateMesh() {
        Mesh* res = meshFactory.CreateObject();

        if (res == nullptr) {
            return nullptr;
        }

        glGenBuffers(1, &res->vertexBufferId);
        glGenBuffers(1, &res->indexBufferId);

        bool fail = false;

        if (res->vertexBufferId == 0) {
            if (res->indexBufferId != 0) {
                glDeleteBuffers(1, &res->indexBufferId);
                res->indexBufferId = 0;
            }
            fail = true;
        } else if (res->indexBufferId == 0) {
            glDeleteBuffers(1, &res->vertexBufferId);
            res->vertexBufferId = 0;
            fail = true;
        }

        if (fail == true) {
            meshFactory.DestroyObject(res);
            res = nullptr;
        }

        return res;
    }

    Mesh* MeshManager::CreateMeshFromObjFile(const char* _path) {
        Mesh* res = CreateMesh();

        if (res == nullptr) {
            return nullptr;
        }

        if (LoadMeshObj(_path, res) == false) {
            LOG_ERR(
                "MeshManager::CreateMeshFromObjFile(): unable to load mesh \""
                << _path << "\"");
            DestroyMesh(res);
            return nullptr;
        }

        glBindBuffer(GL_ARRAY_BUFFER, res->vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * res->vertices.size(),
                     &res->vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res->indexBufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(unsigned int) * res->indices.size(),
                     &res->indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        return res;
    }

    Mesh* MeshManager::CreateMeshQuad() {
        Mesh* res = CreateMesh();

        if (res == nullptr) {
            return nullptr;
        }

        Vertex v1;
        Vertex v2;
        Vertex v3;
        Vertex v4;

        v1.pos.Set(-1, -1, 0);
        v1.uV.Set(0, 0);
        v1.normal.Set(0, 0, 1);
        v2.pos.Set(-1, 1, 0);
        v2.uV.Set(0, 1);
        v2.normal.Set(0, 0, 1);
        v3.pos.Set(1, 1, 0);
        v3.uV.Set(1, 1);
        v3.normal.Set(0, 0, 1);
        v4.pos.Set(1, -1, 0);
        v4.uV.Set(1, 0);
        v4.normal.Set(0, 0, 1);

        res->vertices.push_back(v1);
        res->vertices.push_back(v2);
        res->vertices.push_back(v3);
        res->vertices.push_back(v4);

        res->indices.push_back(0);
        res->indices.push_back(1);
        res->indices.push_back(3);
        res->indices.push_back(3);
        res->indices.push_back(1);
        res->indices.push_back(2);

        res->materialOffsets.push_back(0);
        res->materialOffsets.push_back(res->indices.size());

        glBindBuffer(GL_ARRAY_BUFFER, res->vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * res->vertices.size(),
                     &res->vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res->indexBufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(unsigned int) * res->indices.size(),
                     &res->indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        return res;
    }

    Mesh* MeshManager::CreateMeshCube() {
        Mesh* res = CreateMesh();

        if (res == nullptr) {
            return nullptr;
        }

        drash::Vec2f angles[6];
        angles[0].Set(0, 0);
        angles[1].Set(0, M_PI / 2);
        angles[2].Set(M_PI, 0);
        angles[3].Set(0, 3 * M_PI / 2);
        angles[4].Set(-M_PI / 2.0, 0);
        angles[5].Set(M_PI / 2.0, 0);

        Vertex v1;
        Vertex v2;
        Vertex v3;
        Vertex v4;

        v1.uV.Set(0, 0);
        v2.uV.Set(0, 1);
        v3.uV.Set(1, 1);
        v4.uV.Set(1, 0);

        for (unsigned int i = 0; i < 6; i++) {
            unsigned int basei = i * 4;
            res->indices.push_back(basei + 0);
            res->indices.push_back(basei + 1);
            res->indices.push_back(basei + 3);
            res->indices.push_back(basei + 3);
            res->indices.push_back(basei + 1);
            res->indices.push_back(basei + 2);

            drash::Vec4f p1(-1, -1, 1, 1);
            drash::Vec4f p2(-1, 1, 1, 1);
            drash::Vec4f p3(1, 1, 1, 1);
            drash::Vec4f p4(1, -1, 1, 1);
            drash::Vec4f n1(0, 0, 1, 1);

            drash::Vec4f rp1 = p1;
            drash::Vec4f rp2 = p2;
            drash::Vec4f rp3 = p3;
            drash::Vec4f rp4 = p4;
            drash::Vec4f rn1 = n1;

            drash::Matrix4f m;
            if (drash::math::Abs(angles[i].x) > 0.0001) {
                drash::MatrixRotationX(m, angles[i].x);
                drash::MatrixMultiply(m, p1, rp1);
                drash::MatrixMultiply(m, p2, rp2);
                drash::MatrixMultiply(m, p3, rp3);
                drash::MatrixMultiply(m, p4, rp4);
                drash::MatrixMultiply(m, n1, rn1);
            } else if (drash::math::Abs(angles[i].y) > 0.0001) {
                drash::MatrixRotationY(m, angles[i].y);
                drash::MatrixMultiply(m, p1, rp1);
                drash::MatrixMultiply(m, p2, rp2);
                drash::MatrixMultiply(m, p3, rp3);
                drash::MatrixMultiply(m, p4, rp4);
                drash::MatrixMultiply(m, n1, rn1);
            }

            v1.pos = rp1;
            v1.normal = rn1;
            v2.pos = rp2;
            v2.normal = rn1;
            v3.pos = rp3;
            v3.normal = rn1;
            v4.pos = rp4;
            v4.normal = rn1;

            res->vertices.push_back(v1);
            res->vertices.push_back(v2);
            res->vertices.push_back(v3);
            res->vertices.push_back(v4);
        }

        res->materialOffsets.push_back(0);
        res->materialOffsets.push_back(res->indices.size());

        glBindBuffer(GL_ARRAY_BUFFER, res->vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * res->vertices.size(),
                     &res->vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res->indexBufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(unsigned int) * res->indices.size(),
                     &res->indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        return res;
    }

    Mesh* MeshManager::CreateMeshFromVertices(const Vertex* _vertices,
                                                unsigned int _vertices_count,
                                                const unsigned int* _indices,
                                                unsigned int _indices_count) {
        if (_vertices == nullptr || _vertices_count == 0) {
            LOG_ERR("MeshManager::CreateMeshFromVertices(): _vertices must be "
                    "specified");
            return nullptr;
        }

        if (_indices == nullptr || _indices_count == 0) {
            LOG_ERR("MeshManager::CreateMeshFromVertices(): _indices must be "
                    "specified");
            return nullptr;
        }

        Mesh* res = CreateMesh();

        if (res == nullptr) {
            return nullptr;
        }

        res->vertices.resize(_vertices_count);
        memcpy(&res->vertices[0], _vertices, sizeof(Vertex) * _vertices_count);
        res->indices.resize(_indices_count);
        memcpy(&res->indices[0], _indices,
               sizeof(unsigned int) * _indices_count);

        res->materialOffsets.push_back(0);
        res->materialOffsets.push_back(_indices_count);

        glBindBuffer(GL_ARRAY_BUFFER, res->vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * res->vertices.size(),
                     &res->vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res->indexBufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(unsigned int) * res->indices.size(),
                     &res->indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        return res;
    }

    bool MeshManager::DestroyMesh(Mesh* _mesh) {
        if (meshFactory.IsObject(_mesh) == false) {
            return false;
        }

        glDeleteBuffers(1, &_mesh->vertexBufferId);
        glDeleteBuffers(1, &_mesh->indexBufferId);
        _mesh->vertexBufferId = 0;
        _mesh->indexBufferId = 0;

        meshFactory.DestroyObject(_mesh);

        return true;
    }

    void MeshManager::ComputeNormals(Mesh* _mesh) {
        if (_mesh == nullptr) {
            LOG_ERR("MeshManager::ComputeNormals() invalid mesh taken");
            return;
        }

        unsigned int triangles_count = _mesh->indices.size() / 3;

        for (unsigned int i = 0; i < triangles_count; i++) {
            drash::Vec3f v1 = _mesh->vertices[_mesh->indices[i * 3]].pos;
            drash::Vec3f v2 = _mesh->vertices[_mesh->indices[i * 3 + 1]].pos;
            drash::Vec3f v3 = _mesh->vertices[_mesh->indices[i * 3 + 2]].pos;

            v2 -= v1;
            v3 -= v1;

            drash::Vec3Cross(v2, v3, v1);

            v1.Normalize();

            _mesh->vertices[_mesh->indices[i * 3]].normal = v1;
            _mesh->vertices[_mesh->indices[i * 3 + 1]].normal = v1;
            _mesh->vertices[_mesh->indices[i * 3 + 2]].normal = v1;
        }

        glBindBuffer(GL_ARRAY_BUFFER, _mesh->vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _mesh->vertices.size(),
                     &_mesh->vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void MeshManager::ComputeTangentSpace(Mesh* _mesh) {
        if (_mesh == nullptr) {
            LOG_ERR("MeshManager::ComputeTangentSpace() invalid mesh taken");
            return;
        }

        unsigned int triangles_count = _mesh->indices.size() / 3;

        for (unsigned int i = 0; i < triangles_count; i++) {
            Vertex& v1 = _mesh->vertices[_mesh->indices[i * 3]];
            Vertex& v2 = _mesh->vertices[_mesh->indices[i * 3 + 1]];
            Vertex& v3 = _mesh->vertices[_mesh->indices[i * 3 + 2]];

            drash::Vec3f q1 = v2.pos;
            drash::Vec3f q2 = v3.pos;

            q1 -= v1.pos;
            q2 -= v1.pos;

            drash::Vec2f st1 = v2.uV;
            drash::Vec2f st2 = v3.uV;

            st1 -= v1.uV;
            st2 -= v1.uV;

            drash::Vec3f tangent;
            drash::Vec3f binormal;

            tangent.x = st2.y * q1.x - st1.y * q2.x;
            tangent.y = st2.y * q1.y - st1.y * q2.y;
            tangent.z = st2.y * q1.z - st1.y * q2.z;

            binormal.x = -st2.x * q1.x + st1.x * q2.x;
            binormal.y = -st2.x * q1.y + st1.x * q2.y;
            binormal.z = -st2.x * q1.z + st1.x * q2.z;

            float k = 1.0 / (st1.x * st2.y - st2.x * st1.y);

            tangent *= k;
            binormal *= k;

            tangent.Normalize();
            binormal.Normalize();

            v1.tangent = tangent;
            v2.tangent = tangent;
            v3.tangent = tangent;

            v1.binormal = binormal;
            v2.binormal = binormal;
            v3.binormal = binormal;
        }

        glBindBuffer(GL_ARRAY_BUFFER, _mesh->vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _mesh->vertices.size(),
                     &_mesh->vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

} // namespace greng
