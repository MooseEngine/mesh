#ifndef MOOSE_CORE_VEC2_H
#define MOOSE_CORE_VEC2_H

namespace moose::core {
	struct Vec2 {
		float x, y;
	};

	inline float dotVec2(const Vec2 &a, const Vec2 &b) noexcept {
		return (a.x * b.x) + (a.y * b.y);
	}

	inline Vec2 operator-(const Vec2 &a, const Vec2 &b) {
		return { a.x - b.x, a.y - b.y };
	}
}

#endif /* MOOSE_CORE_VEC2_H */
