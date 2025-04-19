#ifndef MOOSE_CORE_MAT4_H
#define MOOSE_CORE_MAT4_H

#include "Vec4.h"

namespace moose::core {
	struct Mat4 {
		float m[4][4];

		inline Vec4 transform(const Vec4 &v) const noexcept;
	};

	inline Vec4 operator*(const Mat4 &M, const Vec4 &v) noexcept {
		return Vec4 {
			M.m[0][0]*v.x + M.m[0][1]*v.y + M.m[0][2]*v.z + M.m[0][3]*v.w,
			M.m[1][0]*v.x + M.m[1][1]*v.y + M.m[1][2]*v.z + M.m[1][3]*v.w,
			M.m[2][0]*v.x + M.m[2][1]*v.y + M.m[2][2]*v.z + M.m[2][3]*v.w,
			M.m[3][0]*v.x + M.m[3][1]*v.y + M.m[3][2]*v.z + M.m[3][3]*v.w
		};
	}
} // namespace moose::core

#endif /* MOOSE_CORE_MAT4_H */



