#ifndef MOOSE_GRAPHICS_SCREEN_TRIANGLE_H
#define MOOSE_GRAPHICS_SCREEN_TRIANGLE_H

#include "moose/graphics/TransformedVertex.h"

namespace moose::graphics {
	struct ScreenTriangle {
		TransformedVertex v0, v1, v2;
		float invArea; // 1.0f / edgeFunction(v0.pos2D, v1.pos2D, v2.pos2D)
	};
}


#endif /* MOOSE_GRAPHICS_SCREEN_TRIANGLE_H */
