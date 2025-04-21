#ifndef MOOSE_CORE_VEC3_H
#define MOOSE_CORE_VEC3_H

#include <cmath>
namespace moose::core {
	struct Vec3 {
		float x, y, z;
	};

	inline float dotVec3(const Vec3 &a, const Vec3 &b) noexcept {
		return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	}

	inline Vec3 crossVec3(const Vec3 &a, const Vec3 &b) noexcept {
		return Vec3{
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}

	inline Vec3 normalizeVec3(const Vec3 &v) noexcept {
		float mag = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
		return Vec3{ v.x/mag, v.y/mag, v.z/mag };
	}

	inline Vec3 operator-(const Vec3 &a, const Vec3 &b) {
		return { a.x - b.x, a.y - b.y, a.z - b.z };
	}

	inline Vec3 operator*(const Vec3 &v, const float &s) {
		return Vec3{ v.x * s, v.y * s, v.z * s };
	}
	inline Vec3 operator*(const float &s, const Vec3 &v) {
		return v * s;
	}

	inline Vec3 operator*=(const Vec3 &v, const float &s) {
		return v * s;
	}
}

#endif /* MOOSE_CORE_VEC3_H */

