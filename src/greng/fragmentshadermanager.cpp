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
#include "fragmentshadermanager.h"
#include "fragmentshader.h"
#include <cstring>

namespace greng
{

using drash::CLogger;

CFragmentShaderManager::CFragmentShaderManager():
    mShaderFactory(mShadersCountLimit, "CFragmentShader")
{
}

CFragmentShaderManager::~CFragmentShaderManager()
{
    while (mShaderFactory.EnumObjects() != 0)
    {
        mShaderFactory.DestroyObject(mShaderFactory.GetObjects()[0]);
    }
}

CFragmentShader *CFragmentShaderManager::CreateShader()
{
    CFragmentShader *res = mShaderFactory.CreateObject();

    if (res == nullptr)
    {
        return nullptr;
    }

    res->mVertexShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    if (res->mVertexShaderId == 0)
    {
        mShaderFactory.DestroyObject(res);
        return nullptr;
    }

    return res;
}

CFragmentShader *CFragmentShaderManager::CreateShaderDummy()
{
    CFragmentShader *res = CreateShader();

    if (res == nullptr)
    {
        return nullptr;
    }

    const char *source = "#version 120\n\n"
    "void main(void)\n"
    "{\n"
    "gl_FragColor = vec4(0, 1, 0, 1);\n"
    "}\n";

    int len = strlen(source);

    glShaderSource(res->mVertexShaderId, 1, &source, &len);

    glCompileShader(res->mVertexShaderId);

    int status = GL_FALSE;

    glGetShaderiv(res->mVertexShaderId, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        const int buffer_size = 128;
        char buffer[buffer_size];
        int length = 0;

        glGetShaderInfoLog(res->mVertexShaderId, buffer_size - 1, &length, buffer);

        LOG_ERR("CFragmentShaderManager::CreateShaderDummy(): glCompileShader failed");
        LOG_ERR("Message: "<<buffer);

        DestroyShader(res);
        res = nullptr;
    }

    return res;
}

bool CFragmentShaderManager::DestroyShader(CFragmentShader *_shader)
{
    if (mShaderFactory.IsObject(_shader) == false)
    {
        LOG_ERR("CFragmentShaderManager::DestroyShader(): invalid shader taken");
        return false;
    }

    glDeleteShader(_shader->mVertexShaderId);
    _shader->mVertexShaderId = 0;

    mShaderFactory.DestroyObject(_shader);

    return true;
}

} // namespace greng
