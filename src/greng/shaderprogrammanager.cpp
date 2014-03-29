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

#include <GL/glew.h>
#include "shaderprogrammanager.h"
#include "shaderprogram.h"
#include "vertexshader.h"
#include "fragmentshader.h"

namespace greng {

    using drash::CLogger;

    CShaderProgramManager::CShaderProgramManager()
        : mProgramFactory(mProgramsCountLimit, "CShaderProgram") {}

    CShaderProgramManager::~CShaderProgramManager() {
        while (mProgramFactory.EnumObjects() != 0) {
            DestroyProgram(mProgramFactory.GetObjects()[0]);
        }
    }

    bool CShaderProgramManager::Init() {
        Release();

        return true;
    }

    void CShaderProgramManager::Release() {}

    CShaderProgram *CShaderProgramManager::CreateProgram() {
        CShaderProgram *res = mProgramFactory.CreateObject();

        if (res == nullptr) {
            return nullptr;
        }

        res->mProgramId = glCreateProgram();

        if (res->mProgramId == 0) {
            mProgramFactory.DestroyObject(res);
            res = nullptr;
        }

        return res;
    }

    CShaderProgram *CShaderProgramManager::CreateProgram(CVertexShader *_vs,
                                                         CFragmentShader *_fs) {
        if (_vs == nullptr || _fs == nullptr) {
            return nullptr;
        }

        CShaderProgram *res = CreateProgram();

        if (res == nullptr) {
            return nullptr;
        }

        glAttachShader(res->mProgramId, _vs->mVertexShaderId);
        glAttachShader(res->mProgramId, _fs->mVertexShaderId);
        glLinkProgram(res->mProgramId);

        int status = GL_FALSE;

        glGetProgramiv(res->mProgramId, GL_LINK_STATUS, &status);

        if (status == GL_FALSE) {
            const int buffer_size = 2048;
            char buffer[buffer_size];
            int length = 0;

            glGetProgramInfoLog(res->mProgramId, buffer_size - 1, &length,
                                buffer);

            LOG_ERR(
                "CShaderProgramManager::CreateProgram(): glLinkProgram failed");
            LOG_ERR("Message: " << buffer);

            DestroyProgram(res);

            return nullptr;
        }

        glUseProgram(res->mProgramId);
        glValidateProgram(res->mProgramId);

        status = GL_FALSE;

        glGetProgramiv(res->mProgramId, GL_VALIDATE_STATUS, &status);

        if (status == GL_FALSE) {
            const int buffer_size = 2048;
            char buffer[buffer_size];
            int length = 0;

            glGetProgramInfoLog(res->mProgramId, buffer_size - 1, &length,
                                buffer);

            LOG_ERR("CShaderProgramManager::CreateProgram(): glValidateProgram "
                    "failed");
            LOG_ERR("Message: " << buffer);

            DestroyProgram(res);

            return nullptr;
        }

        glUseProgram(0);

        return res;
    }

    bool CShaderProgramManager::DestroyProgram(CShaderProgram *_program) {
        if (mProgramFactory.IsObject(_program) == false) {
            LOG_ERR("CShaderProgramManager::DestroyProgram(): invalid program "
                    "taken");
            return false;
        }

        glDeleteProgram(_program->mProgramId);

        mProgramFactory.DestroyObject(_program);

        return false;
    }

} // namespace greng
