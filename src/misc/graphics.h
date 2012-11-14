#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "cvec2.h"
#include "math.h"

namespace drash
{

extern const float g_LayerWidth;

void DrawBodySide(const b2Vec2 &_v1,
              const b2Vec2 &_v2,
              const CInterval &_interval,
              const b2Color &_diffuse );

void DrawBody(const b2Vec2 *_vertices,
              unsigned int _count,
              const CInterval &_interval,
              const b2Color &_color);

}// namespace drash

#endif // GRAPHICS_H
