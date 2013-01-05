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
#include "shaderprogrammanager.h"
#include "shaderprogram.h"

namespace greng
{

using drash::CLogger;

CShaderProgramManager::CShaderProgramManager():
    mProgramFactory(mProgramsCountLimit, "CShaderProgram")
{
}

CShaderProgramManager::~CShaderProgramManager()
{
    while (mProgramFactory.EnumObjects() != 0)
    {
        DestroyProgram(mProgramFactory.GetObjects()[0]);
    }
}

CShaderProgram *CShaderProgramManager::CreateProgram()
{
    CShaderProgram *res = mProgramFactory.CreateObject();

    if (res == nullptr)
    {
        return nullptr;
    }

    res->mProgramId = glCreateProgram();

    if (res->mProgramId == 0)
    {
        mProgramFactory.DestroyObject(res);
        res = nullptr;
    }

    return res;
}

bool CShaderProgramManager::DestroyProgram(CShaderProgram *_program)
{
    if (mProgramFactory.IsObject(_program))
    {
        LOG_ERR("CShaderProgramManager::DestroyProgram(): invalid program taken");
        return false;
    }

    glDeleteProgram(_program->mProgramId);

    mProgramFactory.DestroyObject(_program);

    return false;
}

} // namespace greng
