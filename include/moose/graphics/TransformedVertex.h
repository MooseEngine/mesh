#ifndef MOOSE_GRAPHICS_TRANSFORMED_VERTEX_H
#define MOOSE_GRAPHICS_TRANSFORMED_VERTEX_H

#include "moose/graphics/ColorF.h"

namespace moose::graphics {
	struct TransformedVertex {
		core::Vec2 screenPos;	// for raster bounds and edge tests
		float depth;			// ndc.z
		float oneOverW;			// 1 / clip.w
		ColorF colorOverW;		// color * oneOverW
	};
}


#endif /* MOOSE_GRAPHICS_TRANSFORMED_VERTEX_H */
