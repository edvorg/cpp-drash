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
#include "vertexshadermanager.h"
#include "vertexshader.h"
#include <cstring>

namespace greng {

    using drash::CLogger;

    CVertexShaderManager::CVertexShaderManager()
        : shaderFactory(shadersCountLimit, "CVertexShader") {}

    CVertexShaderManager::~CVertexShaderManager() {
        while (shaderFactory.EnumObjects() != 0) {
            DestroyShader(shaderFactory.GetObjects()[0]);
        }
    }

    CVertexShader* CVertexShaderManager::CreateShader() {
        CVertexShader* res = shaderFactory.CreateObject();

        if (res == nullptr) {
            return nullptr;
        }

        res->vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

        if (res->vertexShaderId == 0) {
            shaderFactory.DestroyObject(res);
            return nullptr;
        }

        return res;
    }

    CVertexShader* CVertexShaderManager::CreateShaderDummy() {
        CVertexShader* res = CreateShader();

        if (res == nullptr) {
            return nullptr;
        }

        const char* source =
            "#version 120\n\n"
            "uniform mat4 gModelViewMatrix;\n"
            "uniform mat4 gProjMatrix;\n\n"
            "void main(void)\n"
            "{\n"
            "gl_Position = gProjMatrix * gModelViewMatrix * gl_Vertex;\n"
            "}\n";

        return CreateShaderFromSource(source);
    }

    CVertexShader*
    CVertexShaderManager::CreateShaderFromSource(const char* _source) {
        if (_source == nullptr) {
            return nullptr;
        }

        CVertexShader* res = CreateShader();

        if (res == nullptr) {
            return nullptr;
        }

        int len = strlen(_source);

        glShaderSource(res->vertexShaderId, 1, &_source, &len);

        glCompileShader(res->vertexShaderId);

        int status = GL_FALSE;

        glGetShaderiv(res->vertexShaderId, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE) {
            const int buffer_size = 2048;
            char buffer[buffer_size];
            int length = 0;

            glGetShaderInfoLog(res->vertexShaderId, buffer_size - 1, &length,
                               buffer);

            LOG_ERR("CVertexShaderManager::CreateShaderFromSource(): "
                    "glCompileShader failed");
            LOG_ERR("Message: " << buffer);

            DestroyShader(res);
            res = nullptr;
        }

        return res;
    }

    CVertexShader*
    CVertexShaderManager::CreateShaderFromFile(const char* _path) {
        if (_path == nullptr) {
            return nullptr;
        }

        std::ifstream in(_path);

        if (in.is_open() == false) {
            LOG_ERR("CVertexShaderManager::CreateShaderFromFile(): unable to "
                    "load vertex shader \""
                    << _path << "\"");
            return nullptr;
        }

        const unsigned int buffer_size = 4096;
        char buffer[buffer_size] = "";
        in.read(buffer, buffer_size - 1);

        return CreateShaderFromSource(buffer);
    }

    bool CVertexShaderManager::DestroyShader(CVertexShader* _shader) {
        if (shaderFactory.IsObject(_shader) == false) {
            LOG_ERR(
                "CVertexShaderManager::DestroyShader(): invalid shader taken");
            return false;
        }

        glDeleteShader(_shader->vertexShaderId);
        _shader->vertexShaderId = 0;

        shaderFactory.DestroyObject(_shader);

        return true;
    }

} // namespace greng
