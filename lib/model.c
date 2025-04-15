#include <stdarg.h>
#include <math.h>

#include "model.h"

int isInsideTriangle(Vec3 p, Triangle t) {
	Bary3 baryCoords = getBarycentricCoordinates(p, t);
	return baryCoords.alpha >= 0 && baryCoords.beta >= 0 && baryCoords.gamma >= 0;
}

int isInsideTriangleFromBary(Bary3 bary) {
	return bary.alpha >= 0 && bary.beta >= 0 && bary.gamma >= 0;
}

Bary3 getBarycentricCoordinates(Vec3 p, Triangle t) {
	float denom = ((t.vertices[1].y - t.vertices[2].y) * (t.vertices[0].x - t.vertices[2].x) + (t.vertices[2].x - t.vertices[1].x) * (t.vertices[0].y - t.vertices[2].y));

	float alpha = ((t.vertices[1].y - t.vertices[2].y) * (p.x - t.vertices[2].x) + (t.vertices[2].x - t.vertices[1].x) * (p.y - t.vertices[2].y)) / denom;
	float beta  = ((t.vertices[2].y - t.vertices[0].y) * (p.x - t.vertices[2].x) + (t.vertices[0].x - t.vertices[2].x) * (p.y - t.vertices[2].y)) / denom;
	float gamma = 1.0f - alpha - beta;

	return (Bary3) { alpha, beta, gamma };
}

Color multiplyFloatColor(float f, Color c) {
	return (Color) {
		.A = f*c.A,
		.R = f*c.R,
		.G = f*c.G,
		.B = f*c.B,
	};
}

Color addColors(int count, ...) {
	va_list args;
	va_start(args, count);

	Color sumColor = { .A = 0, .R = 0, .G = 0, .B = 0 };

	for (int i = 0; i < count; i++) {
		Color c = va_arg(args, Color);
		sumColor.A += c.A;
		sumColor.R += c.R;
		sumColor.G += c.G;
		sumColor.B += c.B;
	}

	va_end(args);
	return sumColor;
}
