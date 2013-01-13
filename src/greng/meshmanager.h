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

#ifndef GRENG_MESHMANAGER_H
#define GRENG_MESHMANAGER_H

namespace greng
{

class CMesh;
class CVertex;

class CMeshManager final
{
public:
    constexpr static unsigned int mMeshesCountLimit = 1024;

    CMeshManager();
    CMeshManager(const CMeshManager &) = delete;
    CMeshManager(CMeshManager &&) = delete;
    CMeshManager &operator =(const CMeshManager &) = delete;
    CMeshManager &operator =(CMeshManager &&) = delete;
    ~CMeshManager();

    bool Init();
    void Release();

    CMesh *CreateMesh();
    CMesh *CreateMeshFromObjFile(const char *_path);
    CMesh *CreateMeshQuad();
    CMesh *CreateMeshCube();
    CMesh *CreateMeshFromVertices(const CVertex *_vertices,
                                  unsigned int _vertices_count,
                                  const unsigned int *_indices,
                                  unsigned int _indices_count);
    bool DestroyMesh(CMesh *_mesh);

    void ComputeNormals(CMesh *_mesh);

protected:
private:
    drash::CObjectFactory<CMesh> mMeshFactory;
};

} // namespace greng

#endif // GRENG_MESHMANAGER_H
