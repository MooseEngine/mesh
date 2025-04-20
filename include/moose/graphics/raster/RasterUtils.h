#ifndef MOOSE_GRAPHICS_RASTER_UTILS_H
#define MOOSE_GRAPHICS_RASTER_UTILS_H

namespace moose::core {
	struct Vec2;
}

namespace moose::graphics::raster {
	inline float edgeFunction(const moose::core::Vec2& a,
							  const moose::core::Vec2& b,
							  const moose::core::Vec2& c) noexcept
	{
		return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
	}
}

#endif
