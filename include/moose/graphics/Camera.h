#ifndef MOOSE_GRAPHICS_CAMERA_H
#define MOOSE_GRAPHICS_CAMERA_H

#include "moose/core/Mat4.h"
#include "moose/core/Vec3.h"

namespace moose::core {
	struct Vec3;
	struct Mat4;
}

namespace moose::graphics {
	class Camera {
	public:
		Camera(const core::Vec3& eye,
			   const core::Vec3& target,
			   const core::Vec3& up,
			   float fovYdeg, float aspect, float zNear, float zFar);

		const core::Mat4 &view() const noexcept;
		const core::Mat4 &proj() const noexcept;

	private:
		core::Vec3 eye;     // stored by value
		core::Vec3 target;  // stored by value
		core::Vec3 up;      // stored by value

		float  fovYdeg, aspect, zNear, zFar;
		core::Mat4 viewMatrix, projMatrix;

		void updateMatrices();
		void updateViewMatrix();
		void updateProjMatrix();
	};
} // namespace moose::graphics

#endif /* MOOSE_GRAPHICS_CAMERA_H */
