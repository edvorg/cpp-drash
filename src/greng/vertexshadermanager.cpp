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
#include "vertexshadermanager.h"
#include "vertexshader.h"

namespace greng
{

using drash::CLogger;

CVertexShaderManager::CVertexShaderManager():
    mShaderFactory(mShadersCountLimit, "CVertexShader")
{
}

CVertexShaderManager::~CVertexShaderManager()
{
}

CVertexShader *CVertexShaderManager::CreateShader()
{
    CVertexShader *res = mShaderFactory.CreateObject();

    if (res == nullptr)
    {
        return nullptr;
    }

    res->mVertexShaderId = glCreateShader(GL_VERTEX_SHADER);

    if (res->mVertexShaderId == 0)
    {
        mShaderFactory.DestroyObject(res);
        return nullptr;
    }

    return res;
}

bool CVertexShaderManager::DestroyShader(CVertexShader *_shader)
{
    if (mShaderFactory.IsObject(_shader) == false)
    {
        LOG_ERR("CVertexShaderManager::DestroyShader(): invalid shader taken");
        return false;
    }

    glDeleteShader(_shader->mVertexShaderId);
    _shader->mVertexShaderId = 0;

    mShaderFactory.DestroyObject(_shader);

    return true;
}

} // namespace greng
