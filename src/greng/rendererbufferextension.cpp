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

#include "rendererbufferextension.h"

namespace greng
{

PFNGLGENBUFFERSPROC glGenBuffers = 0;
PFNGLBINDBUFFERPROC glBindBuffer = 0;
PFNGLBUFFERDATAPROC glBufferData = 0;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = 0;

bool CRendererBufferExtension::Init()
{
    #define GET_EXT(ext, type) ext = reinterpret_cast<type>(glXGetProcAddress(reinterpret_cast<const GLubyte*>(#ext)));\
                               if (ext == 0) return false;
    GET_EXT(glGenBuffers, PFNGLGENBUFFERSPROC);
    GET_EXT(glBindBuffer, PFNGLBINDBUFFERPROC);
    GET_EXT(glBufferData, PFNGLBUFFERDATAPROC);
    GET_EXT(glDeleteBuffers, PFNGLDELETEBUFFERSPROC);
    #undef GET_EXT

    return true;
}

} // namespace greng
