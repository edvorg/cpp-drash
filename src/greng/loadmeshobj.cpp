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

#include "loadmeshobj.h"
#include "mesh.h"
#include "../diag/logger.h"
#include <fstream>
#include <cstring>

namespace greng {

    using drash::Logger;
    using drash::Vec2f;

    void LoadVertices(Mesh* res, std::ifstream& f, unsigned int);
    void LoadTexCoords(std::vector<Vec2f>& uv, std::ifstream& f, unsigned int);
    void LoadFaces(Mesh* res, std::ifstream& f, unsigned int,
                   std::vector<Vec2f>& uv);

    bool LoadMeshObj(const char* _path, Mesh* _mesh) {
        std::ifstream f(_path);
        if (!f.is_open()) {
            LOG_ERR("LoadMeshObj(): failed to load " << _path);
            return false;
        }

        unsigned int fsize = 0;

        f.seekg(0, std::ios_base::end);
        fsize = f.tellg();
        f.seekg(0, std::ios_base::beg);

        if (fsize == 0) {
            LOG_ERR("loadModelOBJ(): wrong size of " << _path);
            return false;
        }

        std::vector<Vec2f> uv;

        LoadVertices(_mesh, f, fsize);
        LoadTexCoords(uv, f, fsize);
        LoadFaces(_mesh, f, fsize, uv);

        if (_mesh->materialOffsets.size() == 0) {
            _mesh->materialOffsets.push_back(0);
        }

        _mesh->materialOffsets.push_back(_mesh->indices.size());

        return true;
    }

    void LoadVertex(Vertex& v, const char* buf) {
        sscanf(buf, "%f %f %f", &v.pos.x, &v.pos.y, &v.pos.z);
        v.uV.x = 0;
        v.uV.y = 0;
        v.color.r = 1;
        v.color.g = 1;
        v.color.b = 1;
        v.color.a = 1;
    }

    void LoadVertices(Mesh* res, std::ifstream& f, unsigned int) {
        f.clear(std::ios_base::goodbit);
        f.seekg(0, std::ios_base::beg);

        while (!f.eof()) {
            char buf[1024] = { 0 };
            char buf1[64] = { 0 };

            f.getline(buf, 1024);
            sscanf(buf, "%s", buf1);
            if (strcmp(buf1, "v") == 0) {
                res->vertices.resize(res->vertices.size() + 1);
                LoadVertex(res->vertices.back(), buf + 2);
            }
        }
    }

    void LoadTexCoords() {}

    class face {
    public:
        unsigned int v1;
        unsigned int v2;
        unsigned int v3;
        unsigned int v4;
        unsigned int vt1;
        unsigned int vt2;
        unsigned int vt3;
        unsigned int vt4;
        bool triangle;
        bool tc;
    };

    void LoadFace(face& f, const char* buf) {
        char buf1[1024] = { 0 };
        char buf2[1024] = { 0 };
        char buf3[1024] = { 0 };
        char buf4[1024] = { 0 };
        sscanf(buf, "%s %s %s %s", buf1, buf2, buf3, buf4);

        memset(&f, 0, sizeof(Vertex));

        sscanf(buf1, "%d/%d", &f.v1, &f.vt1);
        sscanf(buf2, "%d/%d", &f.v2, &f.vt2);
        sscanf(buf3, "%d/%d", &f.v3, &f.vt3);
        sscanf(buf4, "%d/%d", &f.v4, &f.vt4);

        f.v1--;
        f.v2--;
        f.v3--;

        if (f.vt1 == 0 || f.vt2 == 0 || f.vt3 == 0) {
            f.tc = false;
        } else {
            f.tc = true;

            f.vt1--;
            f.vt2--;
            f.vt3--;

            if (f.vt4) {
                f.vt4--;
            }
        }

        if (f.v4 == 0) {
            f.triangle = true;
        } else {
            f.triangle = false;

            f.v4--;
        }
    }

    void LoadFaces(Mesh* res, std::ifstream& f, unsigned int,
                   std::vector<Vec2f>& uv) {
        f.clear(std::ios_base::goodbit);
        f.seekg(0, std::ios_base::beg);

        while (!f.eof()) {
            char buf[1024] = { 0 };
            char buf1[64] = { 0 };

            f.getline(buf, 1024);
            sscanf(buf, "%s", buf1);
            if (strcmp(buf1, "f") == 0) {
                face fc;
                LoadFace(fc, buf + 2);

                if (fc.triangle == true) {
                    res->vertices.push_back(res->vertices[fc.v1]);
                    if (fc.tc == true) {
                        res->vertices.back().uV = uv[fc.vt1];
                    }
                    res->vertices.push_back(res->vertices[fc.v2]);
                    if (fc.tc == true) {
                        res->vertices.back().uV = uv[fc.vt2];
                    }
                    res->vertices.push_back(res->vertices[fc.v3]);
                    if (fc.tc == true) {
                        res->vertices.back().uV = uv[fc.vt3];
                    }

                    res->indices.push_back(res->vertices.size() - 3);
                    res->indices.push_back(res->vertices.size() - 2);
                    res->indices.push_back(res->vertices.size() - 1);
                } else {
                    res->vertices.push_back(res->vertices[fc.v1]);
                    if (fc.tc == true) {
                        res->vertices.back().uV = uv[fc.vt1];
                    }
                    res->vertices.push_back(res->vertices[fc.v2]);
                    if (fc.tc == true) {
                        res->vertices.back().uV = uv[fc.vt2];
                    }
                    res->vertices.push_back(res->vertices[fc.v4]);
                    if (fc.tc == true) {
                        res->vertices.back().uV = uv[fc.vt4];
                    }
                    res->vertices.push_back(res->vertices[fc.v4]);
                    if (fc.tc == true) {
                        res->vertices.back().uV = uv[fc.vt4];
                    }
                    res->vertices.push_back(res->vertices[fc.v2]);
                    if (fc.tc == true) {
                        res->vertices.back().uV = uv[fc.vt2];
                    }
                    res->vertices.push_back(res->vertices[fc.v3]);
                    if (fc.tc == true) {
                        res->vertices.back().uV = uv[fc.vt3];
                    }

                    res->indices.push_back(res->vertices.size() - 6);
                    res->indices.push_back(res->vertices.size() - 5);
                    res->indices.push_back(res->vertices.size() - 4);
                    res->indices.push_back(res->vertices.size() - 3);
                    res->indices.push_back(res->vertices.size() - 2);
                    res->indices.push_back(res->vertices.size() - 1);
                }
            } else if (strcmp(buf1, "usemtl") == 0) {
                char buf2[1024] = { 0 };
                sscanf(buf + 7, "%s", buf2);

                res->materialOffsets.push_back(res->indices.size());
                res->materialNames.push_back(std::string(buf2));
            }
        }
    }

    void LoadTexCoord(Vec2f& tc, const char* buf) {
        sscanf(buf, "%f %f", &tc.x, &tc.y);
        tc.y *= -1;
    }

    void LoadTexCoords(std::vector<Vec2f>& uv, std::ifstream& f,
                       unsigned int) {
        f.clear(std::ios_base::goodbit);
        f.seekg(0, std::ios_base::beg);

        while (!f.eof()) {
            char buf[1024] = { 0 };
            char buf1[64] = { 0 };

            f.getline(buf, 1024);
            sscanf(buf, "%s", buf1);
            if (strcmp(buf1, "vt") == 0) {
                uv.resize(uv.size() + 1);
                LoadTexCoord(uv.back(), buf + 3);
            }
        }
    }
}
