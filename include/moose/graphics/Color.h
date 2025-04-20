#ifndef MOOSE_GRAPHICS_COLOR_H
#define MOOSE_GRAPHICS_COLOR_H

#include <cstdint>

namespace moose::graphics {
	struct Color {
		uint8_t r, g, b, a;

		constexpr Color(uint8_t r=255,
						uint8_t g=255,
						uint8_t b=255,
						uint8_t a=255) noexcept
		: r(r), g(g), b(b), a(a) {}

		constexpr uint32_t toARGB() const noexcept {
			return	(a << 24) |
					(r << 16) |
					(g << 8 ) |
					(b << 0 );
		}
	};

	inline Color operator*(const Color &c, const float &s) noexcept {

	}
}

#endif /* MOOSE_GRAPHICS_COLOR_H */

