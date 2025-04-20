#ifndef MOOSE_CORE_VEC3_H
#define MOOSE_CORE_VEC3_H

namespace moose::core {
	struct Vec3 {
		float x, y, z;
	};

	inline Vec3 operator*(const Vec3 &v, const float &s) {
		return Vec3{ v.x * s, v.y * s, v.z * s };
	}
	inline Vec3 operator*=(const Vec3 &v, const float &s) {
		return v * s;
	}
}

#endif /* MOOSE_CORE_VEC3_H */

