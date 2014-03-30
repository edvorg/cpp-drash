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

#ifndef GRENG_MESHMANAGER_H
#define GRENG_MESHMANAGER_H

#include "../misc/objectfactory.h"

namespace greng {

    class Mesh;
    class Vertex;

    class MeshManager final {
    public:
        constexpr static unsigned int meshesCountLimit = 1024;

        MeshManager();
        MeshManager(const MeshManager&) = delete;
        MeshManager(MeshManager&&) = delete;
        MeshManager& operator=(const MeshManager&) = delete;
        MeshManager& operator=(MeshManager&&) = delete;
        ~MeshManager();

        Mesh* CreateMesh();
        Mesh* CreateMeshFromObjFile(const char* _path);
        Mesh* CreateMeshQuad();
        Mesh* CreateMeshCube();
        Mesh* CreateMeshFromVertices(const Vertex* _vertices,
                                      unsigned int _vertices_count,
                                      const unsigned int* _indices,
                                      unsigned int _indices_count);
        bool DestroyMesh(Mesh* _mesh);

        void ComputeNormals(Mesh* _mesh);
        void ComputeTangentSpace(Mesh* _mesh);

    protected:
    private:
        drash::ObjectFactory<Mesh> meshFactory;
    };

} // namespace greng

#endif // GRENG_MESHMANAGER_H
