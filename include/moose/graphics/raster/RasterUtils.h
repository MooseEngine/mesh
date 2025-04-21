#ifndef MOOSE_GRAPHICS_RASTER_UTILS_H
#define MOOSE_GRAPHICS_RASTER_UTILS_H

#include "moose/core/Vec2.h"
#include "moose/graphics/raster/Bary3.h"
#include "moose/graphics/ScreenTriangle.h"

namespace moose::graphics {
	struct ScreenTriangle;
}

namespace moose::graphics::raster {
	inline Bary3 computeBarycentrics(ScreenTriangle &t, core::Vec2 &p) noexcept {
		core::Vec2 bv0 = t.v2.screenPos - t.v0.screenPos;
		core::Vec2 bv1 = t.v1.screenPos - t.v0.screenPos;
		core::Vec2 bv2 = p - t.v0.screenPos;

		float b00 = core::dotVec2(bv0, bv0);
		float b01 = core::dotVec2(bv0, bv1);
		float b11 = core::dotVec2(bv1, bv1);
		float b20 = core::dotVec2(bv2, bv0);
		float b21 = core::dotVec2(bv2, bv1);

		float denom = (b00*b11) - (b01*b01);

		float gamma = (b11*b20 - b01*b21) / denom;
		float beta = (b00*b21 - b01*b20) / denom;
		return Bary3{
			1 - beta - gamma, beta, gamma
		};
	}
}

#endif
