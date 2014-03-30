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

#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>
#include "shaderprogrammanager.h"
#include "shaderprogram.h"
#include "vertexshader.h"
#include "fragmentshader.h"

namespace greng {

    using drash::Logger;

    ShaderProgramManager::ShaderProgramManager()
        : programFactory(programsCountLimit, "ShaderProgram") {}

    ShaderProgramManager::~ShaderProgramManager() {
        while (programFactory.EnumObjects() != 0) {
            DestroyProgram(programFactory.GetObjects()[0]);
        }
    }

    ShaderProgram* ShaderProgramManager::CreateProgram() {
        ShaderProgram* res = programFactory.CreateObject();

        if (res == nullptr) {
            return nullptr;
        }

        res->programId = glCreateProgram();

        if (res->programId == 0) {
            programFactory.DestroyObject(res);
            res = nullptr;
        }

        return res;
    }

    ShaderProgram* ShaderProgramManager::CreateProgram(VertexShader* _vs,
                                                       FragmentShader* _fs) {
        if (_vs == nullptr || _fs == nullptr) {
            return nullptr;
        }

        ShaderProgram* res = CreateProgram();

        if (res == nullptr) {
            return nullptr;
        }

        glAttachShader(res->programId, _vs->vertexShaderId);
        glAttachShader(res->programId, _fs->vertexShaderId);
        glLinkProgram(res->programId);

        int status = GL_FALSE;

        glGetProgramiv(res->programId, GL_LINK_STATUS, &status);

        if (status == GL_FALSE) {
            const int buffer_size = 2048;
            char buffer[buffer_size];
            int length = 0;

            glGetProgramInfoLog(res->programId, buffer_size - 1, &length,
                                buffer);

            LOG_ERR(
                "ShaderProgramManager::CreateProgram(): glLinkProgram failed");
            LOG_ERR("Message: " << buffer);

            DestroyProgram(res);

            return nullptr;
        }

        glUseProgram(res->programId);
        glValidateProgram(res->programId);

        status = GL_FALSE;

        glGetProgramiv(res->programId, GL_VALIDATE_STATUS, &status);

        if (status == GL_FALSE) {
            const int buffer_size = 2048;
            char buffer[buffer_size];
            int length = 0;

            glGetProgramInfoLog(res->programId, buffer_size - 1, &length,
                                buffer);

            LOG_ERR("ShaderProgramManager::CreateProgram(): glValidateProgram "
                    "failed");
            LOG_ERR("Message: " << buffer);

            DestroyProgram(res);

            return nullptr;
        }

        glUseProgram(0);

        return res;
    }

    bool ShaderProgramManager::DestroyProgram(ShaderProgram* _program) {
        if (programFactory.IsObject(_program) == false) {
            LOG_ERR("ShaderProgramManager::DestroyProgram(): invalid program "
                    "taken");
            return false;
        }

        glDeleteProgram(_program->programId);

        programFactory.DestroyObject(_program);

        return false;
    }

} // namespace greng
