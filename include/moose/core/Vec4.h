#ifndef MOOSE_CORE_VEC4_H
#define MOOSE_CORE_VEC4_H

namespace moose::core {
	struct Vec4 {
		float x, y, z, w;
	};

	inline Vec4 operator/(const Vec4 &v, float &s) noexcept {
		return Vec4{
			v.x / s,
			v.y / s,
			v.z / s,
			v.w / s,
		};
	}
	inline Vec4 operator/=(const Vec4 &v, float &s) noexcept {
		return v / s;
	}

} // namespace moose::core

#endif /* MOOSE_CORE_VEC4_H */
