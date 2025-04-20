#ifndef MOOSE_GRAPHICS_VERTEX_H
#define MOOSE_GRAPHICS_VERTEX_H

#include "moose/graphics/ColorF.h"
#include "moose/core/Vec3.h"

namespace moose::graphics {

	struct Vertex {
		core::Vec3 position;
		core::Vec3 normal;
		// core::Vec2 textureCoordinate;
		ColorF color;
	};
}

#endif /* MOOSE_GRAPHICS_VERTEX_H */

