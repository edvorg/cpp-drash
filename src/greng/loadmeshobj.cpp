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

#include "loadmeshobj.h"
#include "mesh.h"
#include "../diag/logger.h"
#include "../misc/vec2.h"
#include <fstream>
#include <vector>

namespace greng
{

using drash::CLogger;
using drash::CVec2f;

void LoadVertices(CMesh* res, std::ifstream& f, unsigned int fsize);
void LoadTexCoords(std::vector<CVec2f>& uv, std::ifstream& f, unsigned int fsize);
void LoadFaces(CMesh* res, std::ifstream& f, unsigned int fsize, std::vector<CVec2f>& uv);

bool LoadMeshObj(const char *_path, CMesh *_mesh)
{    
    std::ifstream f(_path);
    if (!f.is_open())
    {
        LOG_ERR("LoadMeshObj(): failed to load "<<_path);
        return NULL;
    }

    unsigned int fsize = 0;

    f.seekg(0, std::ios_base::end);
    fsize = f.tellg();
    f.seekg(0, std::ios_base::beg);

    if (fsize == 0)
    {
        LOG_ERR("loadModelOBJ(): wrong size of "<<_path);
        return NULL;
    }

    std::vector<CVec2f> uv;

    LoadVertices(_mesh, f, fsize);
    LoadTexCoords(uv, f, fsize);
    LoadFaces(_mesh, f, fsize, uv);

    if (_mesh->mMaterialOffsets.size() == 0)
    {
        _mesh->mMaterialOffsets.push_back(0);
    }

    _mesh->mMaterialOffsets.push_back(_mesh->mIndices.size());

    return true;
}

void LoadVertex(CVertex& v, const char* buf)
{
    sscanf(buf, "%f %f %f", &v.mPos.mX, &v.mPos.mY, &v.mPos.mZ);
    v.mUV.mX = 0;
    v.mUV.mY = 0;
    v.mColor.mR = 1;
    v.mColor.mG = 1;
    v.mColor.mB = 1;
    v.mColor.mA = 1;
}

void LoadVertices(CMesh *res, std::ifstream& f, unsigned int fsize)
{
    f.clear(std::ios_base::goodbit);
    f.seekg(0, std::ios_base::beg);

    while (!f.eof())
    {
        char buf[1024] ={0};
        char buf1[64] = {0};

        f.getline(buf, 1024);
        sscanf(buf, "%s", buf1);
        if (strcmp(buf1, "v") == 0)
        {
            res->mVertices.resize(res->mVertices.size()+1);
            LoadVertex(res->mVertices.back(), buf+2);
        }
    }
}

void LoadTexCoords()
{
}

class face
{
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

void LoadFace(face& f, const char* buf)
{
    char buf1[1024] = {0};
    char buf2[1024] = {0};
    char buf3[1024] = {0};
    char buf4[1024] = {0};
    sscanf(buf, "%s %s %s %s", buf1, buf2, buf3, buf4);

    memset(&f, 0, sizeof(CVertex));

    sscanf(buf1, "%d/%d", &f.v1, &f.vt1);
    sscanf(buf2, "%d/%d", &f.v2, &f.vt2);
    sscanf(buf3, "%d/%d", &f.v3, &f.vt3);
    sscanf(buf4, "%d/%d", &f.v4, &f.vt4);

    f.v1--;
    f.v2--;
    f.v3--;

    if (f.vt1 == 0 || f.vt2 == 0 || f.vt3 == 0)
    {
        f.tc = false;
    }
    else
    {
        f.tc = true;

        f.vt1--;
        f.vt2--;
        f.vt3--;

        if (f.vt4)
        {
            f.vt4--;
        }
    }

    if (f.v4 == 0)
    {
        f.triangle = true;
    }
    else
    {
        f.triangle = false;

        f.v4--;
    }
}

void LoadFaces(CMesh *res, std::ifstream& f, unsigned int fsize, std::vector<CVec2f> &uv)
{
    f.clear(std::ios_base::goodbit);
    f.seekg(0, std::ios_base::beg);

    while (!f.eof())
    {
        char buf[1024] ={0};
        char buf1[64] = {0};

        f.getline(buf, 1024);
        sscanf(buf, "%s", buf1);
        if (strcmp(buf1, "f") == 0)
        {
            face fc;
            LoadFace(fc, buf+2);

            if (fc.triangle == true)
            {
                res->mVertices.push_back(res->mVertices[fc.v1]);
                if (fc.tc == true)
                {
                    res->mVertices.back().mUV = uv[fc.vt1];
                }
                res->mVertices.push_back(res->mVertices[fc.v2]);
                if (fc.tc == true)
                {
                    res->mVertices.back().mUV = uv[fc.vt2];
                }
                res->mVertices.push_back(res->mVertices[fc.v3]);
                if (fc.tc == true)
                {
                    res->mVertices.back().mUV = uv[fc.vt3];
                }

                res->mIndices.push_back(res->mVertices.size()-3);
                res->mIndices.push_back(res->mVertices.size()-2);
                res->mIndices.push_back(res->mVertices.size()-1);
            }
            else
            {
                res->mVertices.push_back(res->mVertices[fc.v1]);
                if (fc.tc == true)
                {
                    res->mVertices.back().mUV = uv[fc.vt1];
                }
                res->mVertices.push_back(res->mVertices[fc.v2]);
                if (fc.tc == true)
                {
                    res->mVertices.back().mUV = uv[fc.vt2];
                }
                res->mVertices.push_back(res->mVertices[fc.v4]);
                if (fc.tc == true)
                {
                    res->mVertices.back().mUV = uv[fc.vt4];
                }
                res->mVertices.push_back(res->mVertices[fc.v4]);
                if (fc.tc == true)
                {
                    res->mVertices.back().mUV = uv[fc.vt4];
                }
                res->mVertices.push_back(res->mVertices[fc.v2]);
                if (fc.tc == true)
                {
                    res->mVertices.back().mUV = uv[fc.vt2];
                }
                res->mVertices.push_back(res->mVertices[fc.v3]);
                if (fc.tc == true)
                {
                    res->mVertices.back().mUV = uv[fc.vt3];
                }

                res->mIndices.push_back(res->mVertices.size()-6);
                res->mIndices.push_back(res->mVertices.size()-5);
                res->mIndices.push_back(res->mVertices.size()-4);
                res->mIndices.push_back(res->mVertices.size()-3);
                res->mIndices.push_back(res->mVertices.size()-2);
                res->mIndices.push_back(res->mVertices.size()-1);
            }
        }
        else if (strcmp(buf1, "usemtl") == 0)
        {
            char buf2[1024] = {0};
            sscanf(buf+7, "%s", buf2);

            res->mMaterialOffsets.push_back(res->mIndices.size());
            res->mMaterialNames.push_back(std::string(buf2));
        }
    }
}

void LoadTexCoord(CVec2f& tc, const char* buf)
{
    sscanf(buf, "%f %f", &tc.mX, &tc.mY);
    tc.mY *= -1;
}

void LoadTexCoords(std::vector<CVec2f> &uv, std::ifstream& f, unsigned int fsize)
{
    f.clear(std::ios_base::goodbit);
    f.seekg(0, std::ios_base::beg);

    while (!f.eof())
    {
        char buf[1024] ={0};
        char buf1[64] = {0};

        f.getline(buf, 1024);
        sscanf(buf, "%s", buf1);
        if (strcmp(buf1, "vt") == 0)
        {
            uv.resize(uv.size()+1);
            LoadTexCoord(uv.back(), buf+3);
        }
    }
}

}
