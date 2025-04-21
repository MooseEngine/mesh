#include <cmath>

#include "moose/graphics/Camera.h"

#define PI 3.14159265f

namespace moose::graphics {
	Camera::Camera(const core::Vec3& eye,
				   const core::Vec3& target,
				   const core::Vec3& up,
				   float fovYdeg, float aspect, float zNear, float zFar) :
				   eye(eye),
				   target(target),
				   up(up),
				   fovYdeg(fovYdeg),
				   aspect(aspect),
				   zNear(zNear),
				   zFar(zFar)
	{
	   updateMatrices();
	}
/* PUBLIC */
		const core::Mat4 &Camera::view() const noexcept { return this->viewMatrix; }
		const core::Mat4 &Camera::proj() const noexcept { return this->projMatrix; }

/* PRIVATE */
	void Camera::updateMatrices() {
		updateViewMatrix();
		updateProjMatrix();
	}

	void Camera::updateViewMatrix() {
		this->viewMatrix = {0};
		// forward vector: the direction from the camera to the position of the target
		core::Vec3 forward = core::normalizeVec3(this->target - this->eye);

		// right vector: the vector perpendicular to the forward direction and the given up vector
		// swap this->up and forward if you intend for the camera to be facing the +z axis instead of -z.
		core::Vec3 right = core::normalizeVec3(core::crossVec3(this->up, forward)); // TODO: implement crossVec3
		
		// true up vector: the actual up direction in the camera's space
		core::Vec3 trueUp = core::crossVec3(right, forward);

		// Rotation
		// First row: right vector
		this->viewMatrix.m[0][0] = right.x;
		this->viewMatrix.m[0][1] = right.y;
		this->viewMatrix.m[0][2] = right.z;
		this->viewMatrix.m[0][3] = -core::dotVec3(right, this->eye); // TODO: implement dotVec3

		// Second row: up vector
		this->viewMatrix.m[1][0] = trueUp.x;
		this->viewMatrix.m[1][1] = trueUp.y;
		this->viewMatrix.m[1][2] = trueUp.z;
		this->viewMatrix.m[1][3] = -core::dotVec3(trueUp, this->eye);

		// Depending on which convention you use, some people use -forward and some people use forward
		// Third row: negative forward vector (so that the camera looks along the -z axis)
		core::Vec3 negativeF = -1.0f * forward;
		this->viewMatrix.m[2][0] = negativeF.x;
		this->viewMatrix.m[2][1] = negativeF.y;
		this->viewMatrix.m[2][2] = negativeF.z;
		this->viewMatrix.m[2][3] = core::dotVec3(forward, this->eye);

		// Fourth row: homogeneous coordinates
		this->viewMatrix.m[3][0] = 0.0f;
		this->viewMatrix.m[3][1] = 0.0f;
		this->viewMatrix.m[3][2] = 0.0f;
		this->viewMatrix.m[3][3] = 1.0f;
	}

	void Camera::updateProjMatrix() {
		this->projMatrix = {0};

		float fovYrad = fovYdeg * (PI/180.0f);
		float tanHalfFOV = tanf(fovYrad / 2.0f);

		this->projMatrix.m[0][0] = 1.0f / (this->aspect * tanHalfFOV);
		this->projMatrix.m[1][1] = 1.0f / tanHalfFOV;
		this->projMatrix.m[2][2] = -(this->zFar + this->zNear) / (this->zFar - this->zNear);
		this->projMatrix.m[3][3] = 0.0f;

		this->projMatrix.m[2][3] = -(2.0f * this->zFar * this->zNear) / (this->zFar - this->zNear);
		this->projMatrix.m[3][2] = -1.0f;
	}
}
