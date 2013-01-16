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

#include <GL/glew.h>
#include "meshmanager.h"
#include "mesh.h"
#include "../diag/logger.h"
#include "loadmeshobj.h"
#include <cstring>

namespace greng
{

using drash::CLogger;

CMeshManager::CMeshManager():
    mMeshFactory(mMeshesCountLimit, "CMesh")
{
}

CMeshManager::~CMeshManager()
{
    while (mMeshFactory.EnumObjects() != 0)
    {
        DestroyMesh(mMeshFactory.GetObjects()[0]);
    }
}

bool CMeshManager::Init()
{
    Release();

    return true;
}

void CMeshManager::Release()
{
}

CMesh *CMeshManager::CreateMesh()
{
    CMesh *res = mMeshFactory.CreateObject();

    if (res == nullptr)
    {
        return nullptr;
    }

    glGenBuffers(1, &res->mVertexBufferId);
    glGenBuffers(1, &res->mIndexBufferId);

    bool fail = false;

    if (res->mVertexBufferId == 0)
    {
        if (res->mIndexBufferId != 0)
        {
            glDeleteBuffers(1, &res->mIndexBufferId);
            res->mIndexBufferId = 0;
        }
        fail = true;
    }
    else if (res->mIndexBufferId == 0)
    {
        glDeleteBuffers(1, &res->mVertexBufferId);
        res->mVertexBufferId = 0;
        fail = true;
    }

    if (fail == true)
    {
        mMeshFactory.DestroyObject(res);
        res = nullptr;
    }

    return res;
}

CMesh *CMeshManager::CreateMeshFromObjFile(const char *_path)
{
    CMesh *res = CreateMesh();

    if (res == nullptr)
    {
        return nullptr;
    }

    if (LoadMeshObj(_path, res) == false)
    {
        LOG_ERR("CMeshManager::CreateMeshFromObjFile(): unable to load mesh \""<<_path<<"\"");
        DestroyMesh(res);
        return nullptr;
    }

    glBindBuffer(GL_ARRAY_BUFFER, res->mVertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CVertex) * res->mVertices.size(), &res->mVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res->mIndexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * res->mIndices.size(), &res->mIndices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return res;
}

CMesh *CMeshManager::CreateMeshQuad()
{
    CMesh *res = CreateMesh();

    if (res == nullptr)
    {
        return nullptr;
    }

    CVertex v1;
    CVertex v2;
    CVertex v3;
    CVertex v4;

    v1.mPos.Set(-1, -1, 0);
    v1.mUV.Set(0, 0);
    v1.mNormal.Set(0, 0, 1);
    v2.mPos.Set(-1, 1, 0);
    v2.mUV.Set(0, 1);
    v2.mNormal.Set(0, 0, 1);
    v3.mPos.Set(1, 1, 0);
    v3.mUV.Set(1, 1);
    v3.mNormal.Set(0, 0, 1);
    v4.mPos.Set(1, -1, 0);
    v4.mUV.Set(1, 0);
    v4.mNormal.Set(0, 0, 1);

    res->mVertices.push_back(v1);
    res->mVertices.push_back(v2);
    res->mVertices.push_back(v3);
    res->mVertices.push_back(v4);

    res->mIndices.push_back(0);
    res->mIndices.push_back(1);
    res->mIndices.push_back(3);
    res->mIndices.push_back(3);
    res->mIndices.push_back(1);
    res->mIndices.push_back(2);

    res->mMaterialOffsets.push_back(0);
    res->mMaterialOffsets.push_back(res->mIndices.size());

    glBindBuffer(GL_ARRAY_BUFFER, res->mVertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CVertex) * res->mVertices.size(), &res->mVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res->mIndexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * res->mIndices.size(), &res->mIndices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return res;
}

CMesh *CMeshManager::CreateMeshCube()
{
    CMesh *res = CreateMesh();

    if (res == nullptr)
    {
        return nullptr;
    }

    drash::CVec2f angles[6];
    angles[0].Set(0, 0);
    angles[1].Set(0, M_PI / 2);
    angles[2].Set(M_PI, 0);
    angles[3].Set(0, 3 * M_PI / 2);
    angles[4].Set(-M_PI / 2.0, 0);
    angles[5].Set(M_PI / 2.0, 0);

    CVertex v1;
    CVertex v2;
    CVertex v3;
    CVertex v4;

    v1.mUV.Set(0, 0);
    v2.mUV.Set(0, 1);
    v3.mUV.Set(1, 1);
    v4.mUV.Set(1, 0);

    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned int basei = i * 4;
        res->mIndices.push_back(basei + 0);
        res->mIndices.push_back(basei + 1);
        res->mIndices.push_back(basei + 3);
        res->mIndices.push_back(basei + 3);
        res->mIndices.push_back(basei + 1);
        res->mIndices.push_back(basei + 2);

        drash::CVec4f p1(-1, -1, 1, 1);
        drash::CVec4f p2(-1,  1, 1, 1);
        drash::CVec4f p3( 1,  1, 1, 1);
        drash::CVec4f p4( 1, -1, 1, 1);
        drash::CVec4f n1(0, 0, 1, 1);

        drash::CVec4f rp1 = p1;
        drash::CVec4f rp2 = p2;
        drash::CVec4f rp3 = p3;
        drash::CVec4f rp4 = p4;
        drash::CVec4f rn1 = n1;

        drash::CMatrix4f m;
        if (drash::math::Abs(angles[i].mX) > 0.0001)
        {
            drash::MatrixRotationX(m, angles[i].mX);
            drash::MatrixMultiply(m, p1, rp1);
            drash::MatrixMultiply(m, p2, rp2);
            drash::MatrixMultiply(m, p3, rp3);
            drash::MatrixMultiply(m, p4, rp4);
            drash::MatrixMultiply(m, n1, rn1);
        }
        else if (drash::math::Abs(angles[i].mY) > 0.0001)
        {
            drash::MatrixRotationY(m, angles[i].mY);
            drash::MatrixMultiply(m, p1, rp1);
            drash::MatrixMultiply(m, p2, rp2);
            drash::MatrixMultiply(m, p3, rp3);
            drash::MatrixMultiply(m, p4, rp4);
            drash::MatrixMultiply(m, n1, rn1);
        }

        v1.mPos = rp1;
        v1.mNormal = rn1;
        v2.mPos = rp2;
        v2.mNormal = rn1;
        v3.mPos = rp3;
        v3.mNormal = rn1;
        v4.mPos = rp4;
        v4.mNormal = rn1;

        res->mVertices.push_back(v1);
        res->mVertices.push_back(v2);
        res->mVertices.push_back(v3);
        res->mVertices.push_back(v4);
    }

    res->mMaterialOffsets.push_back(0);
    res->mMaterialOffsets.push_back(res->mIndices.size());

    glBindBuffer(GL_ARRAY_BUFFER, res->mVertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CVertex) * res->mVertices.size(), &res->mVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res->mIndexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * res->mIndices.size(), &res->mIndices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return res;
}

CMesh *CMeshManager::CreateMeshFromVertices(const CVertex *_vertices,
                                            unsigned int _vertices_count,
                                            const unsigned int *_indices,
                                            unsigned int _indices_count)
{
    if (_vertices == nullptr || _vertices_count == 0)
    {
        LOG_ERR("CMeshManager::CreateMeshFromVertices(): _vertices must be specified");
        return nullptr;
    }

    if (_indices == nullptr || _indices_count == 0)
    {
        LOG_ERR("CMeshManager::CreateMeshFromVertices(): _indices must be specified");
        return nullptr;
    }

    CMesh *res = CreateMesh();

    if (res == nullptr)
    {
        return nullptr;
    }

    res->mVertices.resize(_vertices_count);
    memcpy(&res->mVertices[0], _vertices, sizeof(CVertex) * _vertices_count);
    res->mIndices.resize(_indices_count);
    memcpy(&res->mIndices[0], _indices, sizeof(unsigned int) * _indices_count);

    res->mMaterialOffsets.push_back(0);
    res->mMaterialOffsets.push_back(_indices_count);

    glBindBuffer(GL_ARRAY_BUFFER, res->mVertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CVertex) * res->mVertices.size(), &res->mVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, res->mIndexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * res->mIndices.size(), &res->mIndices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return res;
}

bool CMeshManager::DestroyMesh(CMesh *_mesh)
{
    if (mMeshFactory.IsObject(_mesh) == false)
    {
        return false;
    }

    glDeleteBuffers(1, &_mesh->mVertexBufferId);
    glDeleteBuffers(1, &_mesh->mIndexBufferId);
    _mesh->mVertexBufferId = 0;
    _mesh->mIndexBufferId = 0;

    mMeshFactory.DestroyObject(_mesh);

    return true;
}

void CMeshManager::ComputeNormals(CMesh *_mesh)
{
    if (_mesh == nullptr)
    {
        LOG_ERR("CMeshManager::ComputeNormals() invalid mesh taken");
        return;
    }

    unsigned int triangles_count = _mesh->mIndices.size() / 3;

    for (unsigned int i = 0; i < triangles_count; i++)
    {
        drash::CVec3f v1 = _mesh->mVertices[_mesh->mIndices[i * 3]].mPos;
        drash::CVec3f v2 = _mesh->mVertices[_mesh->mIndices[i * 3 + 1]].mPos;
        drash::CVec3f v3 = _mesh->mVertices[_mesh->mIndices[i * 3 + 2]].mPos;

        v2 -= v1;
        v3 -= v1;

        drash::Vec3Cross(v2, v3, v1);

        v1.Normalize();

        _mesh->mVertices[_mesh->mIndices[i * 3]].mNormal = v1;
        _mesh->mVertices[_mesh->mIndices[i * 3 + 1]].mNormal = v1;
        _mesh->mVertices[_mesh->mIndices[i * 3 + 2]].mNormal = v1;
    }

    glBindBuffer(GL_ARRAY_BUFFER, _mesh->mVertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CVertex) * _mesh->mVertices.size(), &_mesh->mVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CMeshManager::ComputeTangentSpace(CMesh *_mesh)
{
    if (_mesh == nullptr)
    {
        LOG_ERR("CMeshManager::ComputeTangentSpace() invalid mesh taken");
        return;
    }

    unsigned int triangles_count = _mesh->mIndices.size() / 3;

    for (unsigned int i = 0; i < triangles_count; i++)
    {
        CVertex &v1 = _mesh->mVertices[_mesh->mIndices[i * 3]];
        CVertex &v2 = _mesh->mVertices[_mesh->mIndices[i * 3 + 1]];
        CVertex &v3 = _mesh->mVertices[_mesh->mIndices[i * 3 + 2]];

        CVec3f q1 = v2.mPos;
        CVec3f q2 = v3.mPos;

        q1 -= v1.mPos;
        q2 -= v1.mPos;

        CVec2f st1 = v2.mUV;
        CVec2f st2 = v3.mUV;

        st1 -= v1.mUV;
        st2 -= v1.mUV;

        CVec3f tangent;
        CVec3f binormal;

        tangent.mX = st2.mY * q1.mX - st1.mY * q2.mX;
        tangent.mY = st2.mY * q1.mY - st1.mY * q2.mY;
        tangent.mZ = st2.mY * q1.mZ - st1.mY * q2.mZ;

        binormal.mX = - st2.mX * q1.mX + st1.mX * q2.mX;
        binormal.mY = - st2.mX * q1.mY + st1.mX * q2.mY;
        binormal.mZ = - st2.mX * q1.mZ + st1.mX * q2.mZ;

        float k = 1.0 / (st1.mX * st2.mY - st2.mX * st1.mY);

        tangent *= k;
        binormal *= k;

        tangent.Normalize();
        binormal.Normalize();

        v1.mTangent = tangent;
        v2.mTangent = tangent;
        v3.mTangent = tangent;

        v1.mBinormal = binormal;
        v2.mBinormal = binormal;
        v3.mBinormal = binormal;
    }

    glBindBuffer(GL_ARRAY_BUFFER, _mesh->mVertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CVertex) * _mesh->mVertices.size(), &_mesh->mVertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace greng
