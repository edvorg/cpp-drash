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

#ifndef GRENG_SHADERPROGRAMMANAGER_H
#define GRENG_SHADERPROGRAMMANAGER_H

#include "../misc/objectfactory.h"

namespace greng {

    class VertexShader;
    class FragmentShader;
    class ShaderProgram;

    class ShaderProgramManager {
    public:
        constexpr static unsigned int programsCountLimit = 32;

        ShaderProgramManager();
        ShaderProgramManager(const ShaderProgramManager&) = delete;
        ShaderProgramManager(ShaderProgramManager&&) = delete;
        ShaderProgramManager& operator=(const ShaderProgramManager&) = delete;
        ShaderProgramManager& operator=(ShaderProgramManager&&) = delete;
        ~ShaderProgramManager();

        ShaderProgram* CreateProgram();
        ShaderProgram* CreateProgram(VertexShader* _vs, FragmentShader* _fs);
        bool DestroyProgram(ShaderProgram* _program);

    protected:
    private:
        drash::ObjectFactory<ShaderProgram> programFactory;
    };

} // namespace greng

#endif // GRENG_SHADERPROGRAMMANAGER_H
