#ifndef GRENG_RENDERERBUFFEREXTENSION_H
#define GRENG_RENDERERBUFFEREXTENSION_H

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>

extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;

namespace greng
{

class CRendererBufferExtension
{
public:
    CRendererBufferExtension() = delete;

    static bool Init();
};

} // namespace greng

#endif // GRENG_RENDERERBUFFEREXTENSION_H
