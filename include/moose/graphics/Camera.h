#ifndef MOOSE_GRAPHICS_CAMERA_H
#define MOOSE_GRAPHICS_CAMERA_H

#include "moose/core/Mat4.h"

namespace moose::graphics {
	class Camera {
	public:
		core::Mat4 &view() const noexcept;
		core::Mat4 &proj() const noexcept;
	private:
		core::Mat4 viewMatrix, projectionMatrix;
	};
} // namespace moose::graphics

#endif /* MOOSE_GRAPHICS_CAMERA_H */
