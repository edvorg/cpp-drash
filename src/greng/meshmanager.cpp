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

    CVertex v1;
    CVertex v2;
    CVertex v3;
    CVertex v4;
    CVertex v5;
    CVertex v6;
    CVertex v7;
    CVertex v8;

    v1.mPos.Set(-1, -1, 1);
    v1.mUV.Set(0, 0);
    v1.mColor.Set(1, 1, 1, 1);

    v2.mPos.Set(-1, 1, 1);
    v2.mUV.Set(0, 1);
    v2.mColor.Set(1, 1, 1, 1);

    v3.mPos.Set(1, 1, 1);
    v3.mUV.Set(1, 1);
    v3.mColor.Set(1, 1, 1, 1);

    v4.mPos.Set(1, -1, 1);
    v4.mUV.Set(1, 0);
    v4.mColor.Set(1, 1, 1, 1);

    v5.mPos.Set(-1, -1, -1);
    v5.mUV.Set(0, 0);
    v5.mColor.Set(1, 1, 1, 1);

    v6.mPos.Set(-1, 1, -1);
    v6.mUV.Set(0, 1);
    v6.mColor.Set(1, 1, 1, 1);

    v7.mPos.Set(1, 1, -1);
    v7.mUV.Set(1, 1);
    v7.mColor.Set(1, 1, 1, 1);

    v8.mPos.Set(1, -1, -1);
    v8.mUV.Set(1, 0);
    v8.mColor.Set(1, 1, 1, 1);

    res->mVertices.push_back(v1);
    res->mVertices.push_back(v2);
    res->mVertices.push_back(v3);
    res->mVertices.push_back(v4);
    res->mVertices.push_back(v5);
    res->mVertices.push_back(v6);
    res->mVertices.push_back(v7);
    res->mVertices.push_back(v8);

    /// front face

    res->mIndices.push_back(0);
    res->mIndices.push_back(1);
    res->mIndices.push_back(3);
    res->mIndices.push_back(3);
    res->mIndices.push_back(1);
    res->mIndices.push_back(2);

    /// right face

    res->mIndices.push_back(3);
    res->mIndices.push_back(2);
    res->mIndices.push_back(7);
    res->mIndices.push_back(7);
    res->mIndices.push_back(2);
    res->mIndices.push_back(6);

    /// back face

    res->mIndices.push_back(7);
    res->mIndices.push_back(6);
    res->mIndices.push_back(4);
    res->mIndices.push_back(4);
    res->mIndices.push_back(6);
    res->mIndices.push_back(5);

    /// left face

    res->mIndices.push_back(4);
    res->mIndices.push_back(5);
    res->mIndices.push_back(0);
    res->mIndices.push_back(0);
    res->mIndices.push_back(5);
    res->mIndices.push_back(1);

    /// top face

    res->mIndices.push_back(1);
    res->mIndices.push_back(5);
    res->mIndices.push_back(2);
    res->mIndices.push_back(2);
    res->mIndices.push_back(5);
    res->mIndices.push_back(6);

    /// bottom face

    res->mIndices.push_back(4);
    res->mIndices.push_back(0);
    res->mIndices.push_back(7);
    res->mIndices.push_back(7);
    res->mIndices.push_back(0);
    res->mIndices.push_back(3);

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
