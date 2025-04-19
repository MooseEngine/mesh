#include <stdio.h>
#include "moose/core/Mat4.h"
#include "moose/core/Vec4.h"

int main() {
	moose::core::Mat4 m = moose::core::Mat4{{
		{1, 0, 0, 0},
		{1, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	}};
	
	moose::core::Vec4 v = moose::core::Vec4{
		3, 6, 9, 1
	};

	v = m * v;

	printf("Vector:\n");
	printf("[%f, %f, %f, %f]\n", v.x, v.y, v.z, v.w);
	
	return 0;
}
