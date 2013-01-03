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
