#ifndef MOOSE_GRAPHICS_COLORF_H
#define MOOSE_GRAPHICS_COLORF_H

#include <algorithm>
#include <cstdint>

namespace moose::graphics {
	struct ColorF {
		float r, g, b, a;
	};

	inline ColorF operator*(const ColorF &c, const float &s)  noexcept {
		return ColorF{ c.r*s, c.g*s, c.b*s, c.a*s };
	}
	
	inline ColorF operator*(const float &s, const ColorF &c) noexcept {
		return c * s;
	}

	inline ColorF operator+(const ColorF &c, const ColorF &o) noexcept {
		return ColorF{ c.r + o.r, c.g + o.g, c.b + o.b, c.a + o.a };
	}

	inline ColorF operator/(const ColorF &c, const float &s) noexcept {
		return c * (1.0f / s);
	}

	inline uint32_t packColor(const ColorF& c) noexcept {
		auto clamp = [](float x){ return uint8_t(std::max(0.0f, std::min(1.0f, x))*255.0f); };
		return (uint32_t(clamp(c.a)) << 24)
			 | (uint32_t(clamp(c.r)) << 16)
			 | (uint32_t(clamp(c.g)) <<  8)
			 | (uint32_t(clamp(c.b)) <<  0);
	}

}

#endif /* MOOSE_GRAPHICS_COLORF_H */

