#ifndef MODEL_H
#define MODEL_H

#include "linalg.h"
static const int NUM_TRIANGLE_VERTICES = 3;

typedef struct { float A, R, G, B; } Color;

typedef struct {
	Vec3 vertices[3];
	float reciprocalDepths[3];
	Color colors[3];
	Color icolors[3];
} Triangle;


/**
 * @brief Uses barycentric coordinates to determine whether a given vertex is within a triangle.
 *
 * @param c The vertex to test.
 * @param t The triangle in question.
 *
 * @return Whether the vertex is in the triangle. 0: no, 1: yes.
 */
int isInsideTriangle(Vec3 c, Triangle t);

/**
 * @brief Uses barycentric coordinates to determine whether a given vertex is within a triangle.
 *
 *
 *
 * @return Whether the vertex is in the triangle. 0: no, 1: yes.
 */
int isInsideTriangleFromBary(Bary3 bary);

/**
 * @brief computes the barycentric coordinates $\alpha$, $\beta$, and $\gamma$ for a given point $P$. See `notes/barycentric_coords.md` for more detail.
 *
 * If the barycentric coordinates are all greater than or equal to 0, then `p` is within `t`. These coordinates also, however, provide a weight indicating distance
 * from the center of the triangle.
 *
 * @return	A Vec3 containing the barycentric coordinates for the point `p` in the triangle `t`.
 */
Bary3 getBarycentricCoordinates(Vec3 p, Triangle t);

/**
 * @brief	Multiplies `f` and `c`, distributing the multiplication across each component of c.
 *
 * @return	The color which is the product of `f` and `c`.
 */
Color multiplyFloatColor(float f, Color c);

Color addColors(int count, ...);

#endif /* MODEL_H */
