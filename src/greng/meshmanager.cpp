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

#include <GL/glew.h>
#include "meshmanager.h"
#include "mesh.h"
#include "../diag/logger.h"
#include "loadmeshobj.h"
#include "../misc/vec4.h"
#include "../misc/matrix4.h"
#include "../misc/math.h"

namespace greng
{

using drash::CLogger;

CMeshManager::CMeshManager()
{
    for (unsigned int i = 0; i < mMeshesCountLimit; i++)
    {
        mMeshes[i] = nullptr;
    }
}

CMeshManager::~CMeshManager()
{
    for (unsigned int i = 0; i < mMeshesCount; i++)
    {
        glDeleteBuffers(1, &mMeshes[i]->mVertexBufferId);
        glDeleteBuffers(1, &mMeshes[i]->mIndexBufferId);
        mMeshes[i]->mVertexBufferId = 0;
        mMeshes[i]->mIndexBufferId = 0;
        delete mMeshes[i];
        mMeshes[i] = nullptr;
    }
}

CMesh *CMeshManager::CreateMesh()
{
    if (mMeshesCount >= mMeshesCountLimit)
    {
        LOG_ERR("CMeshManager::CreateMesh(): meshes count exceedes it's limit");
        return nullptr;
    }

    CMesh *res = new CMesh();

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
        delete res;
        return nullptr;
    }
    else
    {
        return mMeshes[mMeshesCount++] = res;
    }
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
        delete res;
        res = nullptr;
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
    v1.mColor.Set(1, 1, 1, 1);

    v2.mPos.Set(-1, 1, 0);
    v2.mUV.Set(0, 1);
    v2.mColor.Set(1, 1, 1, 1);

    v3.mPos.Set(1, 1, 0);
    v3.mUV.Set(1, 1);
    v3.mColor.Set(1, 1, 1, 1);

    v4.mPos.Set(1, -1, 0);
    v4.mUV.Set(1, 0);
    v4.mColor.Set(1, 1, 1, 1);

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

        drash::CVec4f rp1 = p1;
        drash::CVec4f rp2 = p2;
        drash::CVec4f rp3 = p3;
        drash::CVec4f rp4 = p4;

        drash::CMatrix4f m;
        if (drash::math::Abs(angles[i].mX) > 0.0001)
        {
            drash::MatrixRotationX(m, angles[i].mX);
            drash::MatrixMultiply(m, p1, rp1);
            drash::MatrixMultiply(m, p2, rp2);
            drash::MatrixMultiply(m, p3, rp3);
            drash::MatrixMultiply(m, p4, rp4);
        }
        else if (drash::math::Abs(angles[i].mY) > 0.0001)
        {
            drash::MatrixRotationY(m, angles[i].mY);
            drash::MatrixMultiply(m, p1, rp1);
            drash::MatrixMultiply(m, p2, rp2);
            drash::MatrixMultiply(m, p3, rp3);
            drash::MatrixMultiply(m, p4, rp4);
        }

        v1.mPos = rp1;
        v2.mPos = rp2;
        v3.mPos = rp3;
        v4.mPos = rp4;

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

} // namespace greng
