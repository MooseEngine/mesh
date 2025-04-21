#ifndef MOOSE_GRAPHICS_SCREEN_TRIANGLE_H
#define MOOSE_GRAPHICS_SCREEN_TRIANGLE_H

#include "moose/graphics/ClipVertex.h"

namespace moose::graphics {
	struct ScreenTriangle {
		ClipVertex v0, v1, v2;
	};
}


#endif /* MOOSE_GRAPHICS_SCREEN_TRIANGLE_H */
