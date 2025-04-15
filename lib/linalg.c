#include "linalg.h"

#include <math.h>

const float PI = 3.14159265f;

/**
 * @brief multiplies a 4x4 matrix with a 4-dimensional vector. The general formula is:
 *  M =	| a b c d |		*	v = | x |
 *		| e f g h |             | y |
 *		| i j k l |             | z |
 *		| m n o p |             | w |
 *
 *	result is u,
 *
 *  u = | (a*x + b*y + c*z + d*w) |
 *      | (e*x + f*y + g*z + h*w) |
 *      | (i*x + j*y + k*z + l*w) |
 *      | (m*x + n*y + o*z + p*w) |
 *
 * For multi-dimensional arrays, we access them as `m[row][column]`
 *
 * @param	mat	A 4x4 matrix
 * @param	vec	A 4-d vector
 *
 * @return mat * vec, a 4d matrix of type `Vec4`
 */
Vec4 multiplyMat4Vec4(Mat4 mat, Vec4 vec) {
	Vec4 result;
	result.x = (mat.m[0][0]*vec.x) + (mat.m[0][1]*vec.y) + (mat.m[0][2]*vec.z) + (mat.m[0][3]*vec.w);
	result.y = (mat.m[1][0]*vec.x) + (mat.m[1][1]*vec.y) + (mat.m[1][2]*vec.z) + (mat.m[1][3]*vec.w);
	result.z = (mat.m[2][0]*vec.x) + (mat.m[2][1]*vec.y) + (mat.m[2][2]*vec.z) + (mat.m[2][3]*vec.w);
	result.w = (mat.m[3][0]*vec.x) + (mat.m[3][1]*vec.y) + (mat.m[3][2]*vec.z) + (mat.m[3][3]*vec.w);
	return result;
}

Vec3 subtractVec3(Vec3 a, Vec3 b) {
	return (Vec3) { a.x - b.x, a.y - b.y, a.z - b.z };
}

Vec3 normalizeVec3(Vec3 v) {
	float mag = sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
	return (Vec3) { v.x/mag, v.y/mag, v.z/mag };
}

Vec3 crossVec3(Vec3 a, Vec3 b) {
	return (Vec3) {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

float dotVec3(Vec3 a, Vec3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 negativeVec3(Vec3 v) {
	return (Vec3) { -v.x, -v.y, -v.z };
}

/**
 * @brief	Creates a LookAt (View) Matrix. Conceptually, this is the inverse of the camera's own transformation.
 *	Letting right vector = s, forward vector = f, true up vector = u, camera position vector = P
 *
 *	|  s_x	 x_y	 x_z	-dot(s, P) |
 *	|  u_x	 u_y	 u_z	-dot(u, P) |
 *	| -f_x	-f_y	-f_z	 dot(f, P) |
 *	|  0	 0		 0		 1		   |
 *
 *	Explanation:
 *		* The upper left 3x3 block using the vectors s, u, and -f handle the rotation of the world
 *			so that the camera's coordinate system aligns with a standard orientation. In this case,
 *			it aligns with the negative z-axis.
 *		* The rightmost column handles the translation of the world, shifting the world coordinates
 *			so that the camera becomes centered at the origin.
 *
 * @param	camera	The camera which does the viewing.
 *
 * @return	The view matrix built based on the camera data.
 */
Mat4 getViewMatrix(Camera camera) {
	Mat4 view = {0};
	// forward vector: the direction from the camera to the position of the target
	Vec3 forward = normalizeVec3(subtractVec3(camera.target, camera.position));

	// right vector: the vector perpendicular to the forward direction and the given up vector
	// swap camera.up and forward if you intend for the camera to be facing the +z axis instead of -z.
	Vec3 right = normalizeVec3(crossVec3(camera.up, forward));
	
	// true up vector: the actual up direction in the camera's space
	Vec3 trueUp = crossVec3(right, forward);

	// Rotation
	// First row: right vector
	view.m[0][0] = right.x;
	view.m[0][1] = right.y;
	view.m[0][2] = right.z;
	view.m[0][3] = -dotVec3(right, camera.position);

	// Second row: up vector
	view.m[1][0] = trueUp.x;
	view.m[1][1] = trueUp.y;
	view.m[1][2] = trueUp.z;
	view.m[1][3] = -dotVec3(trueUp, camera.position);

	// Depending on which convention you use, some people use -forward and some people use forward
	// Third row: negative forward vector (so that the camera looks along the -z axis)
	Vec3 negativeF = negativeVec3(forward);
	view.m[2][0] = negativeF.x;
	view.m[2][1] = negativeF.y;
	view.m[2][2] = negativeF.z;
	view.m[2][3] = dotVec3(forward, camera.position);

	// Fourth row: homogeneous coordinates
	view.m[3][0] = 0.0f;
	view.m[3][1] = 0.0f;
	view.m[3][2] = 0.0f;
	view.m[3][3] = 1.0f;

	return view;
}

/**
 * @brief creates a projection matrix.
 */
Mat4 getProjectionMatrix(float fov, float aspect, float near, float far) {
	Mat4 proj = {0};
	float tanHalfFOV = tanf(fov / 2.0f);

	proj.m[0][0] = 1.0f / (aspect * tanHalfFOV);
	proj.m[1][1] = 1.0f / tanHalfFOV;
	proj.m[2][2] = -(far + near) / (far - near);
	proj.m[3][3] = 0.0f;

	proj.m[2][3] = -(2.0f * far * near) / (far - near);
	proj.m[3][2] = -1.0f;

	return proj;
}

Mat4 getIdentityMatrix() {
	return (Mat4) { .m = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	}};
}

Mat4 getModelMatrix() {
	return getIdentityMatrix();
}

float deg2rad(float deg) {
	return deg * (PI/180.0f);
}

float edgeFunction(Vec3 c, Vec3 A, Vec3 B) {
	return (c.y - A.y) * (B.x - A.x) - (c.x - A.x) * (B.y - A.y);
}
